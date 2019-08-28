/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/7/2019 11:24:24 PM
 */

#include "CLayerPanel.h"
#include <qpainter.h>
#include <qstatictext.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qevent.h>
#include <qscrollarea.h>

#include "Interface/MainWindow/MainWindow.h"
#include "Interface/EditLayer/EditLayer.h"
#include "Widgets/LayerArea/CLayerArea.h"
#include "Projects/CProject.h"

CLayerPanel::CLayerPanel(QWidget * Parent) : QWidget(Parent)
{
	setObjectName("CLayerPanel");
	setStyleSheet(
		MainWindow::GetGlobalStyle() +
		"QWidget#CLayerPanel {"
			"background: #323232;"
			"color: rgb(200, 200, 200);"
		"}"
		"QPushButton {"
			"border: none;"
			"background-color: rgb(50, 50, 50);"
			"color: rgb(200, 200, 200);"
		"}"
		"QPushButton:hover {"
			//"border: 1px solid rgb(50, 50, 50);"
			"background-color: rgb(66, 66, 66);"
			"color: rgb(255, 255, 255);"
		"}"
		"QPushButton:hover:pressed {"
			"background-color: rgb(25, 25, 25);"
		"}"); // AAAAAAAAAAAAAAAAA

	m_name = new QLabel("Layers", this);
	m_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_name->setStyleSheet("color: rgb(200, 200, 200); font: 11px \"Tahoma\";");
	m_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Policy::Maximum);
	m_name->setMargin(3);

	m_area = new QScrollArea(this);
	m_area->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
	m_area->setLayoutDirection(Qt::RightToLeft); // Get scrollbar on the left
	m_area->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Policy::Preferred);

	QPalette pal = m_area->palette();
	pal.setBrush(QPalette::ColorRole::Window, QColor(30, 30, 30));
	m_area->setPalette(pal);

	m_layers = new CLayerArea(this);

	m_buttons = new QWidget(this), m_add = new QPushButton("+", m_buttons), m_del = new QPushButton("-", m_buttons);
	m_buttons->setFixedHeight(16);
	m_add->setMinimumSize(16, 16);
	m_del->setMinimumSize(16, 16);

	m_lbuttons = new QHBoxLayout(m_buttons);
	m_lbuttons->setAlignment(Qt::AlignmentFlag::AlignBottom | Qt::AlignmentFlag::AlignLeft);
	m_lbuttons->setMargin(0);
	m_lbuttons->setSpacing(0);
	m_lbuttons->addWidget(m_add);
	m_lbuttons->addWidget(m_del);

	m_layout = new QVBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_name);
	m_layout->addWidget(m_area);
	m_layout->addWidget(m_buttons);

	QHBoxLayout* full = new QHBoxLayout(m_layers);
	full->setAlignment(Qt::AlignmentFlag::AlignRight);
	m_layers->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Preferred);
	m_layers->setLayout(full);
	m_area->setWidget(m_layers);

	connect(m_add, &QPushButton::released, this, &CLayerPanel::AddLayer);
	connect(m_del, &QPushButton::released, this, &CLayerPanel::DelLayer);
	m_add->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
}

void CLayerPanel::AddLayer()
{
	if (CProject* proj = CProject::ActiveProject())
		EditLayer::Open(proj);
}

void CLayerPanel::DelLayer()
{
	if (CProject* proj = CProject::ActiveProject())
		proj->RemoveLayer(proj->ActiveLayer());
}