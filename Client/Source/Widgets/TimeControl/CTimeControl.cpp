/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 9/19/2019 10:07:34 AM
 */

#include "CTimeControl.h"
#include "Projects/CProject.h"

CTimeControl::CTimeControl(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(24);

	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);

	m_svgplay = QIcon(":/Icons/Play.svg");
	m_svgpause = QIcon(":/Icons/Pause.svg");

	m_play = new QPushButton(this), m_bkstep = new QPushButton(this), m_fwdstep = new QPushButton(this);

	QSize btnsize(20, 20), iconsize = btnsize / 2;
	m_play->setFixedSize(btnsize);
	m_bkstep->setFixedSize(btnsize);
	m_fwdstep->setFixedSize(btnsize);

	m_play->setIcon(m_svgplay);
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

	connect(m_play, &QPushButton::pressed, [this] { ButtonEvent(m_play); });
	connect(m_bkstep, &QPushButton::pressed, [this] { ButtonEvent(m_bkstep); });
	connect(m_fwdstep, &QPushButton::pressed, [this] { ButtonEvent(m_fwdstep); });

	CProject::Listen([this](CProjectEvent* e) { ProjectEvent(e); });
}

void CTimeControl::ButtonEvent(QPushButton * Btn)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	if (Btn == m_play)
			proj->IsPlaying() ? proj->Pause() : proj->Play();
	else if (Btn == m_bkstep || Btn == m_fwdstep)
	{
		int step = Btn == m_bkstep ? -1 : 1;
		size_t frame = proj->ActiveFrame() + step;
		if (frame >= 0 && frame <= proj->LastFrame()->Index())
			proj->SetActiveFrame(frame);
	}
}

void CTimeControl::ProjectEvent(CProjectEvent * Event)
{
	if (!Event->Project() || Event->Project() != CProject::ActiveProject())
		return;

	switch (Event->Action())
	{
	case CProjectEvent::Play:
	case CProjectEvent::Pause:
		m_play->setIcon(Event->Action() == CProjectEvent::Play ? m_svgpause : m_svgplay);
	}
}
