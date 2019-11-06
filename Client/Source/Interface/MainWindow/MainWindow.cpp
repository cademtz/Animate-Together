#include "MainWindow.h"
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qshortcut.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qevent.h>

#include "Client/CClient.h"
#include "Projects/CProject.h"
#include "Widgets/Canvas/CCanvas.h"
#include "Widgets/ToolBar/CToolBar.h"
#include "Widgets/TitleBar/CTitleBar.h"
#include "Widgets/Timeline/CTimeline.h"
#include "Widgets/MiniPalette/CMiniPalette.h"
#include "Interface/ColorPicker/ColorPicker.h"
#include "Interface/Login/CLogin.h"

QString MainWindow::m_globalstyle;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QFile f(":style/style.qss");
	if (!f.exists())
	{
		QMessageBox msg;
		msg.setText("Failed to load global style sheet");
		msg.exec();
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		m_globalstyle = ts.readAll();
		qApp->setStyleSheet(m_globalstyle);
	}

	ui.setupUi(this);

	m_toolbar = new CToolBar(ui.centralWidget);
	m_history = new CHistoryPanel(ui.mainframe);
	m_layerpanel = new CLayerPanel(ui.mainframe);
	m_timeline = new CTimeline(ui.mainframe);

	ui.w_panels->setFixedWidth(150);
	l_panels = new QVBoxLayout(ui.w_panels);
	l_panels->setSpacing(0);
	l_panels->setMargin(0);
	l_panels->addWidget(m_history);
	l_panels->addWidget(m_layerpanel);

	m_titlebar = new CTitleBar(this, QColor(40, 40, 40));

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);

	QShortcut* bcolor = new QShortcut(Qt::CTRL + Qt::Key_B, this);
	connect(bcolor, &QShortcut::activated, [] { CToolBar::getPalette()->PickColor(); });

	m_canvas = new CCanvas(ui.mainframe);

	l_mainlayout = new QHBoxLayout(ui.mainframe);
	l_mainlayout->setSpacing(0);
	l_mainlayout->setMargin(0);
	l_mainlayout->addWidget(ui.w_panels);
	l_mainlayout->addWidget(m_canvas);

	ui.tool_stack->insertWidget(0, m_toolbar);
	ui.tool_stack->insertWidget(0, m_titlebar);
	ui.tool_stack->addWidget(m_timeline);
	resize(width(), height() + m_titlebar->height() + m_toolbar->height() + m_timeline->minimumHeight());

	CUndoStack::Listen([this](const CUndoAction* Undo) { UndoStackEvent(Undo); });
	CClient::Listen([this](const CBaseMsg* Msg) { ClientEvent(Msg); });
}

MainWindow::~MainWindow()
{
	if (CProject* proj = CProject::ActiveProject())
	{
		CProject::SetActiveProject(0);
		delete proj;
	}
}

CLayer * MainWindow::ActiveLayer() const
{
	if (CProject* proj = CProject::ActiveProject())
		if (CLayer* layer = proj->ActiveLayer())
			return layer;
	return nullptr;
}
void MainWindow::TogglePanel(QWidget * Panel)
{
	auto& w = Get();
	for (size_t i = 0; i < w.l_panels->count(); i++)
	{
		auto item = w.l_panels->itemAt(i);
		if (item->widget() == Panel)
		{
			item->widget()->hide();
			w.l_panels->removeItem(item);
			return;
		}
	}
	Panel->show();
	w.l_panels->addWidget(Panel);
}

void MainWindow::UndoStackEvent(const CUndoAction* Undo)
{
	m_history->update();
}

void MainWindow::ClientEvent(const CBaseMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::LoginMsg:
		CLogin::Open(((CLoginMsg*)Msg)->Flags() & CLoginMsg::PassFlag);
		return;
	}
}

void MainWindow::Undo() {
	if (CProject* proj = CProject::ActiveProject())
		proj->Undos().UndoLast();
}
void MainWindow::Redo() {
	if (CProject* proj = CProject::ActiveProject())
		proj->Undos().RedoLast();
}

void MainWindow::FillLayer()
{
	if (CProject* proj = CProject::ActiveProject())
		if (CLayer* layer = proj->ActiveLayer())
			ColorPicker::Open(Qt::GlobalColor::white, [proj, layer](ColorPicker* picker) {
			proj->Undos().Push(new CUndoFill(*layer->Pixmap(), picker->m_color));
			layer->Pixmap()->fill(picker->m_color);
		});
}

void MainWindow::closeEvent(QCloseEvent * Event)
{
	QMessageBox msg(QMessageBox::Warning,
		"Warning", "All unsaved progress will be lost. Continue?",
		QMessageBox::Yes | QMessageBox::No);
	msg.setDefaultButton(QMessageBox::No);
	msg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	if (msg.exec() != QMessageBox::Yes)
		Event->ignore();
}

void MainWindow::paintEvent(QPaintEvent * Event)
{
	if (isMaximized())
		return; // We only need to draw the frame

	QPainter paint(this);
	paint.setPen(QColor(50, 50, 50));
	paint.drawRect(0, 0, width() - 1, height() - 1);
}