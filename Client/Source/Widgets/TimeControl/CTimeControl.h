/*	Description:
 *		A set of buttons to control the time and playback of an animation
 *
 *	Author: Hold on!
 *	Created: 9/19/2019 10:05:47 AM
 */

#ifndef CTimeControl_H
#define CTimeControl_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qpushbutton.h>
#include <qlayout.h>

class CProjectEvent;

class CTimeControl : public QWidget
{
	QBoxLayout* m_layout;
	QPushButton* m_play, * m_bkstep, * m_fwdstep;
	QIcon m_svgplay, m_svgpause;

public:
	CTimeControl(QWidget* Parent = 0);

private:
	void ButtonEvent(QPushButton* Btn);
	void ProjectEvent(CProjectEvent* Event);
};

#endif // CTimeControl_H