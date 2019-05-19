/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/18/2019 6:09:30 PM
 */

#include "CTitleBar.h"
#include <qevent.h>
#include <qpainter.h>
#include <qstatictext.h>
#include <qmenu.h>
#include <qmenubar.h>
#include <qstyle.h>
#include <qcommonstyle.h>
#include "Interface/CreateProject/CreateProject.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

void CTitleBar::SaveProject(e_export Type)
{
	if (CProject* proj = MainWindow::Get().GetActiveProj())
		proj->Export(Type);
}

CTitleBar::CTitleBar(QMainWindow * Window, QColor Background) : QWidget(Window)
{
	m_parent = Window;
	m_font = QFont("Arial", 10);
	m_fButtons = QFont("Arial", 14, 0);
	setFixedHeight(24);
	setStyleSheet("\
	QPushButton { border: none; color: rgb(160, 160, 160); }			\
	QPushButton:hover:!pressed { background-color: rgb(64, 64, 64); }	\
	QPushButton:hover:pressed { background-color: rgb(20, 110, 230); }");

	m_menubar = new QMenuBar(this);
	m_close = new QPushButton("X", this);
	m_close->setFont(m_fButtons);
	connect(m_close, &QPushButton::released, this, &CTitleBar::CloseParent);
	
	m_minmax = new QPushButton(QString::fromWCharArray(L"\x25A1"), this);
	m_minmax->setFont(m_fButtons);
	connect(m_minmax, &QPushButton::released, this, &CTitleBar::MinMaxParent);

	m_hide = new QPushButton("_", this);
	m_hide->setFont(m_fButtons);
	connect(m_hide, &QPushButton::released, this, &CTitleBar::HideParent);

	m_close->setStyleSheet(styleSheet());
	m_minmax->setStyleSheet(styleSheet());
	m_hide->setStyleSheet(styleSheet());

	m_menubar->setFont(m_font);
	m_menubar->setStyleSheet(
		"QMenuBar {"
			"font-weight: bold;"
			"color: rgb(160, 160, 160);"
			"selection-background-color: rgb(0, 104, 204);"
		"}"
		"QMenu {"
			"font-weight: bold;"
			"color: rgb(200, 200, 200);"
			"background-color: rgb(25, 25, 25);"
			"selection-background-color: rgb(0, 104, 204);"
		"}"
		"QMenuBar:selected, QMenu:hover {"
			"background-color: rgb(0, 104, 204);"
			"color: rgb(255, 255, 255);"
		"}");

	QMenu* file = new QMenu("File", m_menubar), * edit = new QMenu("Edit", m_menubar), * layer = new QMenu("Layer", m_menubar);
	QAction* newproj = new QAction("New"), *save = new QAction("Save as..."), *exp = new QAction("Export layers"),
		*undo = new QAction("Undo"), *redo = new QAction("Redo"), * fill = new QAction("Fill");

	connect(save, &QAction::triggered, [] { SaveProject(e_export::flat); });
	connect(exp, &QAction::triggered, [] { SaveProject(e_export::layers); });
	connect(fill, &QAction::triggered, [] { if (CLayer* layer = MainWindow::Get().GetActiveLayer()) layer->Fill(); });
	connect(undo, &QAction::triggered, &MainWindow::Undo);
	connect(redo, &QAction::triggered, &MainWindow::Redo);
	connect(newproj, &QAction::triggered, &CreateProject::Open);

	fill->setShortcut(Qt::CTRL + Qt::Key_F);
	save->setShortcut(Qt::CTRL + Qt::Key_S);
	undo->setShortcut(Qt::CTRL + Qt::Key_Z);
	redo->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);

	file->addActions({ newproj, save, exp });
	file->insertSeparator(save);
	edit->addActions({ undo, redo });
	layer->addAction(fill);

	m_menubar->addMenu(file);
	m_menubar->addMenu(edit);
	m_menubar->addMenu(layer);

	SetBackground(Background);
}

void CTitleBar::SetBackground(const QColor & Col)
{
	m_background = Col;
	setStyleSheet("background-color: " + m_background.name());
}

void CTitleBar::mouseDoubleClickEvent(QMouseEvent * Event)
{
	//MinMaxParent();
}

void CTitleBar::mousePressEvent(QMouseEvent * Event)
{
	if (Event->button() == Qt::MouseButton::LeftButton)
	{
		m_cursor = Event->globalPos() - m_parent->geometry().topLeft();
		Event->accept();
	}
}

void CTitleBar::mouseMoveEvent(QMouseEvent * Event)
{
	if (Event->buttons() & Qt::MouseButton::LeftButton)
	{
		if (!m_parent->isMaximized() || Event->globalPos().y() > height())
		{
			if (m_parent->isMaximized())
			{
				m_parent->showNormal();
				m_cursor = QPoint(m_oldsize.width() / 2, m_oldsize.height() / 2);
			}
			m_parent->move(Event->globalPos() - m_cursor);
		}
		Event->accept();
	}
}

void CTitleBar::resizeEvent(QResizeEvent * Event)
{
	m_menubar->setGeometry(120, 3, width() - 120, m_menubar->height());
	m_close->setGeometry(geometry().width() - geometry().height(), 0, geometry().height(), geometry().height());
	m_minmax->setGeometry(geometry().width() - geometry().height() * 2, 0, geometry().height(), geometry().height());
	m_hide->setGeometry(geometry().width() - geometry().height() * 3, 0, geometry().height(), geometry().height());
}

void CTitleBar::paintEvent(QPaintEvent * Event)
{
	QPainter paint(this);

	static QStaticText title = QStaticText(m_parent->windowTitle());
	paint.setPen(QColor(180, 180, 180));
	paint.setFont(m_font);
	paint.drawStaticText(6, 3, title);
}

void CTitleBar::CloseParent()
{
	m_parent->close();
}

void CTitleBar::MinMaxParent()
{
	if (!m_parent->isMaximized())
	{
		m_oldsize = size();
		m_parent->showMaximized();
	}
	else
		m_parent->showNormal();
}

void CTitleBar::HideParent()
{
	m_parent->showMinimized();
}