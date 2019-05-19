#include "MainWindow.h"
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qshortcut.h>
#include <qpainter.h>
#include <qlayout.h>
#include <qevent.h>

#include "Projects/CProject.h"
#include "Widgets/Canvas/CCanvas.h"
#include "Widgets/ToolBar/CToolBar.h"
#include "Widgets/History/CHistory.h"
#include "Widgets/TitleBar/CTitleBar.h"
#include "Widgets/Timeline/CTimeline.h"
#include "Widgets/LayerList/CLayerList.h"
#include "Widgets/MiniPalette/CMiniPalette.h"
#include "Interface/ColorPicker/ColorPicker.h"

QString MainWindow::m_globalstyle;

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	QFile f(":style/style.qss");
	if (!f.exists())
	{
		QMessageBox msg;
		msg.setText("ghey");
		msg.exec();
	}
	else
	{
		f.open(QFile::ReadOnly | QFile::Text);
		QTextStream ts(&f);
		m_globalstyle = ts.readAll();
		//qApp->setStyleSheet(ts.readAll());
	}

	ui.setupUi(this);

	m_toolbar = new CToolBar(ui.centralWidget);
	m_history = new CHistory(this, ui.mainframe);
	m_layerlist = new CLayerList(this, ui.mainframe);
	//m_timeline = new CTimeline(ui.mainframe);

	l_panels = new QVBoxLayout(ui.w_panels);
	l_panels->setSpacing(5);
	l_panels->setMargin(0);
	l_panels->addWidget(m_history);
	l_panels->addWidget(m_layerlist);

	m_titlebar = new CTitleBar(this, QColor(40, 40, 40));

	setWindowFlags(Qt::Window | Qt::FramelessWindowHint);

	QCoreApplication::setAttribute(Qt::AA_CompressHighFrequencyEvents);

	QShortcut* bcolor = new QShortcut(Qt::CTRL + Qt::Key_B, this);
	connect(bcolor, &QShortcut::activated, [] { CToolBar::getPalette()->pickColor(); });

	m_canvas = new CCanvas(ui.mainframe);

	l_mainlayout = new QHBoxLayout(ui.mainframe);
	l_mainlayout->setSpacing(0);
	l_mainlayout->setMargin(0);
	l_mainlayout->addWidget(ui.w_panels);
	l_mainlayout->addWidget(m_canvas);

	ui.tool_stack->insertWidget(0, m_toolbar);
	ui.tool_stack->insertWidget(0, m_titlebar);
	//ui.tool_stack->addWidget(m_timeline);
	resize(width(), height() + m_titlebar->height() + m_toolbar->height());// + m_timeline->minimumHeight());

	CUndoStack::Listen([this](const CUndoAction* Undo) { UndoStackEvent(Undo); });
	CProject::Listen([this](e_layerevent Event) { LayerEvent(Event); });
}

MainWindow::~MainWindow()
{
	if (CProject* proj = GetActiveProj())
	{
		SetActiveProj(0);
		delete proj;
	}
}

void MainWindow::SetActiveProj(CProject * Project)
{
	m_proj = Project;
	m_canvas->SetProj(Project);
}

CLayer * MainWindow::GetActiveLayer() const
{
	if (CProject* proj = GetActiveProj())
		if (CLayer* layer = proj->GetActiveLayer())
			return layer;
	return nullptr;
}

void MainWindow::LayerEvent(e_layerevent Event)
{
	m_layerlist->update();
}

void MainWindow::UndoStackEvent(const CUndoAction* Undo)
{
	m_history->update();
}

void MainWindow::Undo() {
	if (CProject* proj = Get().GetActiveProj())
		proj->GetUndos().UndoLast();
}
void MainWindow::Redo() {
	if (CProject* proj = Get().GetActiveProj())
		proj->GetUndos().RedoLast();
}

void MainWindow::FillLayer()
{
	if (CProject* proj = GetActiveProj())
		if (CLayer* layer = proj->GetActiveLayer())
			ColorPicker::Open(Qt::GlobalColor::white, [proj, layer](ColorPicker* picker) {
			proj->GetUndos().Push(new CUndoLayerFill(*layer->GetPixmap(), picker->m_color));
			layer->GetPixmap()->fill(picker->m_color);
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
	QLinearGradient grad = QLinearGradient(0, 0, 0, height() / 2);
	grad.setSpread(QGradient::ReflectSpread);
	grad.setColorAt(0, QColor(0, 128, 255));
	grad.setColorAt(1, QColor(100, 150, 255));
	paint.setPen(isActiveWindow() ? QPen(grad, 1) : QColor(32, 100, 160));
	paint.drawRect(0, 0, width() - 1, height() - 1);
}