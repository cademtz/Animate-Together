/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 5/11/2019 4:25:14 PM
 */

#include "CTimeline.h"
#include "Widgets/FrameList/CFrameList.h"
#include "Widgets/TimeControl/CTimeControl.h"

CTimeline::CTimeline(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(100);
	m_layers = new QWidget(this), m_framelist = new CFrameList(this), m_timecontrol = new CTimeControl(this);
	
	m_layers->setMinimumSize(100, 32);
	m_layers->setFixedWidth(200);
	m_framelist->setMinimumSize(m_layers->minimumSize());

	setStyleSheet("background-color: rgb(50, 50, 50);");

	QFrame* hdiv = new QFrame(this), * vdiv_time = new QFrame(this);
	hdiv->setFrameShape(QFrame::VLine);
	hdiv->setStyleSheet("color: black;");
	hdiv->setContentsMargins(0, 1, 0, 1);
	hdiv->setFixedWidth(1);
	vdiv_time->setFrameShape(QFrame::HLine);
	vdiv_time->setStyleSheet("color: #202020;");
	vdiv_time->setFixedHeight(1);

	m_timearea = new QVBoxLayout();
	m_timearea->addWidget(m_timecontrol);
	m_timearea->addWidget(vdiv_time);
	m_timearea->addWidget(m_framelist);

	m_layout = new QHBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	m_layout->addWidget(m_layers);
	m_layout->addWidget(hdiv);
	m_layout->addLayout(m_timearea, 0);
}