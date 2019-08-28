/*	Description:
 *	Displays a list of frames from a layer
 *
 *	Author: Hold on!
 *	Created: 6/14/2019 7:31:50 PM
 */

#ifndef CFrameList_H
#define CFrameList_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsview.h>
#include <qgraphicsgridlayout.h>

class CLayer;
class CProjectEvent;
class CFrameList : public QGraphicsView
{
	QLayout* m_layout;
	QGraphicsItem* m_scrubbar;
	QGraphicsRectItem* m_playhead;
	QGraphicsLineItem* m_playline;
	QGraphicsGridLayout* m_grid;

public:
	CFrameList(QWidget* Parent = nullptr);

	void Scrub(QMouseEvent* Event);

	void ProjectEvent(CProjectEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
};

#endif // CFrameList_H