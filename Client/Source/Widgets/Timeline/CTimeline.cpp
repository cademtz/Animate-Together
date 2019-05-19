/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 5/11/2019 4:25:14 PM
 */

#include "CTimeline.h"

CTimeline::CTimeline(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(100);
	m_layers = new QWidget(this), m_frames = new QWidget(this);
	
	m_layers->setMinimumSize(100, 32);
	m_layers->setFixedWidth(200);
	m_layers->setStyleSheet("background-color: rgb(50, 50, 50);");
	m_frames->setStyleSheet(m_layers->styleSheet());
	m_frames->setMinimumSize(m_layers->minimumSize());

	m_divide = new QFrame(this);
	m_divide->setFrameShape(QFrame::VLine);
	m_divide->setStyleSheet("background-color: rgb(25, 25, 25); color: rgb(25, 25, 25);");

	m_layout = new QHBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	m_layout->addWidget(m_layers);
	m_layout->addWidget(m_divide);
	m_layout->addWidget(m_frames);
}

