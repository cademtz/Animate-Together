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
#include <qlayout.h>

#include "Interface/CreateProject/CreateProject.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

void CTitleBar::SaveProject(e_export Type)
{
	if (CProject* proj = CProject::ActiveProject())
		proj->Export(Type);
}

CTitleBar::CTitleBar(QMainWindow * Window, QColor Background) : QWidget(Window)
{
	m_parent = Window;
	m_fButtons = QFont("Arial", 13, QFont::Bold);
	setFixedHeight(22);
	setStyleSheet(
		"QPushButton { border: none; color: rgb(160, 160, 160); }"
		"QPushButton:hover:!pressed { background-color: rgb(64, 64, 64); }"
		"QPushButton:hover:pressed { background-color: rgb(20, 110, 230); }");

	m_name = new QLabel("Animate Together", this);
	m_name->setContentsMargins(9, 0, 3, 0);
	QPalette pal = m_name->palette();
	pal.setBrush(QPalette::Foreground, QColor(150, 150, 150));
	m_name->setPalette(pal);
	m_name->setFont(QFont("Tahoma", 8));

	m_menubar = new QMenuBar(this);
	m_menubar->setContentsMargins(10, 0, 10, 0);

	m_close = new QPushButton("x", this);
	m_close->setFont(m_fButtons);
	m_close->setStyleSheet(styleSheet());
	m_close->setFixedSize(height() * 1.5, height());
	connect(m_close, &QPushButton::released, this, &CTitleBar::CloseParent);
	
	m_minmax = new QPushButton("o", this); // TO DO: Maybe actual icons, and not this hacky stuff?
	m_minmax->setFont(m_fButtons);
	m_minmax->setStyleSheet(styleSheet());
	m_minmax->setFixedSize(m_close->size());
	connect(m_minmax, &QPushButton::released, this, &CTitleBar::MinMaxParent);

	m_hide = new QPushButton("-", this);
	m_hide->setFont(m_fButtons);
	m_hide->setStyleSheet(styleSheet());
	m_hide->setFixedSize(m_close->size());
	connect(m_hide, &QPushButton::released, this, &CTitleBar::HideParent);

	m_layout = new QHBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setAlignment(Qt::AlignVCenter);

	m_layout->addWidget(m_name);
	m_layout->addWidget(m_menubar);
	m_layout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding));
	m_layout->addWidget(m_hide);
	m_layout->addWidget(m_minmax);
	m_layout->addWidget(m_close);

	m_menubar->setStyleSheet(
		"QMenuBar {"
			"color: rgb(175, 175, 175);"
			"selection-background-color: rgb(28, 28, 28);"
			"selection-border: 1px solid white;"
		"}"
		"QMenu {"
			"color: rgb(220, 220, 220);"
			"background-color: rgb(28, 28, 28);"
			"selection-background-color: rgb(50, 50, 50);"
			"border: 1px solid rgb(50, 50, 50);"
		"}"
		"QMenuBar:selected, QMenu:hover {"
			"background-color: rgb(55, 55, 55);"
			"color: rgb(255, 255, 255);"
		"}");

	QMenu
		*file = new QMenu("File", m_menubar), *edit = new QMenu("Edit", m_menubar), *layer = new QMenu("Layer", m_menubar),
		*window = new QMenu("Window", m_menubar);
	QAction
		*newproj = new QAction("New", this), *save = new QAction("Save as...", this), *exp = new QAction("Export layers", this),
		*undo = new QAction("Undo", this), *redo = new QAction("Redo", this), *fill = new QAction("Fill", this), *history = new QAction("History", this),
		*layers = new QAction("Layers", this);

	connect(save, &QAction::triggered, [] { SaveProject(e_export::flat); });
	connect(exp, &QAction::triggered, [] { SaveProject(e_export::layers); });
	connect(fill, &QAction::triggered, [] { if (CLayer* layer = MainWindow::Get().ActiveLayer()) layer->Fill(); });
	connect(undo, &QAction::triggered, &MainWindow::Undo);
	connect(redo, &QAction::triggered, &MainWindow::Redo);
	connect(newproj, &QAction::triggered, &CreateProject::Open);
	connect(history, &QAction::triggered, &MainWindow::ToggleHistory);
	connect(layers, &QAction::triggered, &MainWindow::ToggleLayers);

	fill->setShortcut(Qt::CTRL + Qt::Key_F);
	save->setShortcut(Qt::CTRL + Qt::Key_S);
	undo->setShortcut(Qt::CTRL + Qt::Key_Z);
	redo->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_Z);

	file->addActions({ newproj, save, exp });
	file->insertSeparator(save);
	edit->addActions({ undo, redo });
	layer->addAction(fill);
	window->addActions({ history, layers });

	m_menubar->addMenu(file);
	m_menubar->addMenu(edit);
	m_menubar->addMenu(layer);
	m_menubar->addMenu(window);

	m_menubar->move(120, 3);
	m_menubar->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);

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
	//m_menubar->setGeometry(120, 3, width() - 120, m_menubar->height());
	//m_close->setGeometry(geometry().width() - geometry().height(), 0, geometry().height(), geometry().height());
	//m_minmax->setGeometry(geometry().width() - geometry().height() * 2, 0, geometry().height(), geometry().height());
	//m_hide->setGeometry(geometry().width() - geometry().height() * 3, 0, geometry().height(), geometry().height());
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