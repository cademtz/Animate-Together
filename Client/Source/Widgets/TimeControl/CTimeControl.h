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

class CTimeControl : public QWidget
{
	QBoxLayout* m_layout;
	QPushButton* m_play, * m_bkstep, * m_fwdstep;

public:
	CTimeControl(QWidget* Parent = 0);
};

#endif // CTimeControl_H