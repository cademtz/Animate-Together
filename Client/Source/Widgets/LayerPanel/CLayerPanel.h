/*	Description:
 *		A panel that displays LayerArea similar to paint software
 *
 *	Author: Hold on!
 *	Created: 3/7/2019 11:22:53 PM
 */

#ifndef CLayerPanel_H
#define CLayerPanel_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qscrollbar.h>
#include <qlayout.h>
#include <qlabel>

class MainWindow;
class QPushButton;
class QHBoxLayout;
class QScrollArea;
class CLayerArea;
class CLayer;

class CLayerPanel : public QWidget
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

	QHBoxLayout* m_lbuttons;
	QVBoxLayout* m_layout;
	QScrollArea* m_area;
	CLayerArea* m_layers;
	QWidget* m_buttons;
	QLabel* m_name;
	QPushButton* m_add, * m_del;
	MainWindow* m_parent;

public:
	CLayerPanel(MainWindow* Window, QWidget* Parent = 0);

	void AddLayer();
	void DelLayer();
};

#endif // CLayerPanel_H