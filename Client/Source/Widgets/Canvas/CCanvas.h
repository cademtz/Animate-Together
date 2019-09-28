/*	Description:
 *		Custom widget that takes user input and manipulates given pixmap data
 *
 *	Author: Hold on!
 *	Created: 2/28/2019 12:29:45 AM
 */

#ifndef CCanvas_H
#define CCanvas_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qpen.h>
#include "Interface/EditBrush/EditBrush.h"

class CProjectEvent;
class CLayerEvent;
class CFrameEvent;
class CProject;
class CLayer;
enum class e_tool;

class CCanvas : public QWidget
{
	Q_OBJECT

public:
	CCanvas(QWidget* Parent = 0);

	// - Updates only the drawing region
	void UpdateCanvas();

	void setColor(const QColor &c) {
		if (c.isValid())
			m_color = c;
	}
	inline QColor color() const { return m_color; }
	void setTool(e_tool Tool);
	inline const e_tool tool() { return m_tool; }
	void setTabletDevice(QTabletEvent *event);
	inline CProject* GetProj() const { return m_proj; }

protected:
	void ProjectEvent(CProjectEvent* Event);
	void LayerEvent(CLayerEvent* Event);
	void FrameEvent(CFrameEvent* Event);

	void tabletEvent(QTabletEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void mousePressEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void wheelEvent(QWheelEvent* Event);

private:
	enum class e_drawmode
	{
		none,
		down,
		released,
		update
	};

	void paintPixmap(QPainter &painter, QTabletEvent *event);
	void SkinTheOnion(QPainter &Paint, CLayer *Layer);
	qreal pressureToWidth(qreal pressure);
	void updateBrush(const QTabletEvent *event);
	void updateCursor(qreal Pressure);

	void TabletPress(QTabletEvent* Event, CLayer* Layer);
	void TabletMove(QTabletEvent* Event, CLayer* Layer);
	void TabletRelease(QTabletEvent* Event, CLayer* Layer);
	void stepBrush(int Step);
	BrushScale* getBrush();

	CProject* m_proj = 0;

	e_tool m_tool;
	BrushScale m_brushScale, m_eraserScale;
	QColor m_color;
	QBrush m_brush;
	QPen m_pen;

	QPoint m_scroll, m_pmouse;
	bool m_dragging;

	QImage m_buffer;
	QPixmap m_pixcanvas;
	QImage m_imgcanvas;
	e_drawmode m_drawmode;

	struct Point {
		QPointF pos;
		qreal pressure;
		qreal rotation;
	} lastPoint;
};

#endif // CCanvas_H