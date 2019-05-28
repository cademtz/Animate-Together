/*	Description:
 *		Widget that displays an interactive layer list
 *
 *	Author: Hold on!
 *	Created: 5/27/2019 10:30:44 AM
 */

#ifndef CLayerArea_H
#define CLayerArea_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>

class CLayer;

class CLayerArea : public QWidget
{
	enum class e_drag
	{
		null = 0,
		layer,
		hide
	};
	e_drag m_drag = e_drag::null;

	bool m_focused = false, m_pressed = false;
	int m_insert = -1;
	QPoint m_cursor;

	void DrawItems(QPainter& paint);
	void DrawItem(QPainter& paint, const CLayer* Layer, const QRect& region);

	CLayer* SelectLayer(QMouseEvent* Event);

public:
	CLayerArea(QWidget* Parent = 0);
	QRect GetLayerRegion();

protected:
	void resizeEvent(QResizeEvent* Event);

	void paintEvent(QPaintEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void mouseDoubleClickEvent(QMouseEvent* Event);
};

#endif // CLayerArea_H