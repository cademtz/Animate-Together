/*	Description:
 *		Widget that displays layer frames and provides interaction
 *
 *	Author: Hold on!
 *	Created: 3/17/2019 3:27:04 PM
 */

#ifndef CTimeline_H
#define CTimeline_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qframe.h>
#include <qlayout.h>

class CTimeline : public QWidget
{
	QHBoxLayout* m_layout;
	QWidget* m_layers, * m_frames;
	QFrame* m_divide;

public:
	CTimeline(QWidget* Parent);
};

#endif // CTimeline_H