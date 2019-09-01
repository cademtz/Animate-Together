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

class CLayer;
class CFrameEvent;
class CLayerEvent;
class CProjectEvent;
class CGraphicsWidget;
class CGraphicsScrubBar;
class QGraphicsLinearLayout;

class CFrameList : public QGraphicsView
{
	CGraphicsScrubBar* m_scrubbar;
	QGraphicsRectItem* m_playhead;
	QGraphicsLineItem* m_playline;

	QGraphicsWidget* m_widget;
	QGraphicsLinearLayout* m_rows;

	void SceneWidthChanged();

public:
	CFrameList(QWidget* Parent = nullptr);

	void FrameEvent(CFrameEvent* Event);
	void LayerEvent(CLayerEvent* Event);
	void Scrub(QMouseEvent* Event);

	void ProjectEvent(CProjectEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
};

#endif // CFrameList_H