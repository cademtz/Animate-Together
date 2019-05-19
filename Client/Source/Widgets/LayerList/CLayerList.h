/*	Description:
 *		A panel that displays the current project's layers and provides user interaction
 *
 *	Author: Hold on!
 *	Created: 3/7/2019 11:22:53 PM
 */

#ifndef CLayerList_H
#define CLayerList_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qlabel.h>

class MainWindow;
class QPushButton;
class QHBoxLayout;
class CLayer;

class CLayerList : public QWidget
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

	QHBoxLayout* m_lbuttons, * m_ltop;
	QVBoxLayout* m_layout;
	QScrollBar* m_scroll;
	QWidget* m_layers, * m_buttons, * m_top;
	QLabel* m_name;
	QPushButton* m_add, * m_del;
	MainWindow* m_parent;

	void DrawItems(QPainter& paint);
	void DrawItem(QPainter& paint, const CLayer* Layer, const QRect& region);

	CLayer* SelectLayer(QMouseEvent* Event);

public:
	CLayerList(MainWindow* Window, QWidget* Parent = 0);

	void AddLayer();
	void DelLayer();

	QRect GetListRegion();
	QRect GetLayerRegion();

protected:
	void paintEvent(QPaintEvent* Event);
	void wheelEvent(QWheelEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void mouseDoubleClickEvent(QMouseEvent* Event);
};

#endif // CLayerList_H