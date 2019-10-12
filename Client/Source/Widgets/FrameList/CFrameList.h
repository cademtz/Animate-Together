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

class CFrameEvent;
class CLayerEvent;
class CLayerLayout;
class CProjectEvent;
class CGraphicsScrubBar;
class QGraphicsLinearLayout;

class CFrameList : public QGraphicsView
{
public:
	CFrameList(QWidget* Parent = nullptr);

protected:
	void mousePressEvent(QMouseEvent* Event) { MouseEvent(Event); }
	void mouseReleaseEvent(QMouseEvent* Event) { MouseEvent(Event); }
	void mouseMoveEvent(QMouseEvent* Event) { MouseEvent(Event); }

private:
	enum class e_drag
	{
		None = 0,
		Clicked,
		Dragging
	};

	e_drag m_drag;
	bool m_selecting = false;
	QRect m_boxselect;

	CGraphicsScrubBar* m_scrubbar;
	QGraphicsRectItem* m_playhead, * m_boxoverlay;
	QGraphicsLineItem* m_playline;

	QGraphicsWidget* m_widget;
	QGraphicsLinearLayout* m_rows;
	CLayerLayout* m_layers;

	void UpdateScrub();
	void ProjectEvent(CProjectEvent* Event);
	void FrameEvent(CFrameEvent* Event);
	void LayerEvent(CLayerEvent* Event);
	void MouseEvent(QMouseEvent* Event);
	void ShortcutEvent(const QShortcut* Shortcut);

	bool Scrub(QMouseEvent* Event);
	bool Drag(QMouseEvent* Event);
	bool Select(QMouseEvent* Event);

};

#endif // CFrameList_H