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
#include <vector>
#include "Projects/CProject.h"

class CFrameList;
class CLayerEvent;
class CTimeControl;

class CTimeline : public QWidget
{
	QHBoxLayout* m_layout;
	QVBoxLayout* m_timearea;
	QWidget* m_layers;
	CFrameList* m_framelist;
	CTimeControl* m_timecontrol;

public:
	CTimeline(QWidget* Parent);
};

#endif // CTimeline_H