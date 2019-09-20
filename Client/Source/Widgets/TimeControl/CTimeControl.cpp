/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 9/19/2019 10:07:34 AM
 */

#include "CTimeControl.h"

CTimeControl::CTimeControl(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(24);

	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	m_play = new QPushButton(this), m_bkstep = new QPushButton(this), m_fwdstep = new QPushButton(this);

	QSize btnsize(20, 20), iconsize = btnsize / 2;
	m_play->setFixedSize(btnsize);
	m_bkstep->setFixedSize(btnsize);
	m_fwdstep->setFixedSize(btnsize);

	m_play->setIcon(QIcon(":/Icons/Play.svg"));
	m_fwdstep->setIcon(QIcon(":/Icons/FwdStep.svg"));
	m_bkstep->setIcon(QIcon(":/Icons/BackStep.svg"));
	m_play->setIconSize(iconsize);
	m_fwdstep->setIconSize(iconsize);
	m_bkstep->setIconSize(iconsize);
	
	setStyleSheet(
		"QPushButton {"
			"color: white;"
			"border: none;"
			"background-color: none"
		"}"
		"QPushButton:hover {"
			"border: 1px solid #4B4B4B;"
			"background-color: #3C3C3C;"
		"}"
		"QPushButton:checked, QPushButton:pressed {"
			"background-color: #191919;"
			"border: 1px solid #4B4B4B;"
		"}");
			
	m_layout->addWidget(m_bkstep);
	m_layout->addWidget(m_play);
	m_layout->addWidget(m_fwdstep);
}
