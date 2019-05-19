/*	Description:
 *		Has a layout for adding a bunch of tools and stuff
 *
 *	Author: Hold on!
 *	Created: 4/6/2019 1:50:32 PM
 */

#ifndef CToolBar__H
#define CToolBar__H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qlayout.h>
#include <list>
#include <functional>

class CTool;
class CMiniPalette;

enum class e_tool {
	null = 0,
	Brush, Eraser, Eyedrop, Line, Rect
};

typedef std::function<void(e_tool)> ToolEvent_t;

class CToolBar : public QWidget
{
	QGridLayout* layout;
	CTool* m_brush, *m_eraser, *m_eyedrop, *m_rect;
	static CMiniPalette* m_palette;

	static std::list<ToolEvent_t> m_listeners;
public:
	CToolBar(QWidget* Parent);

	inline static CMiniPalette* getPalette() { return m_palette; }

	// - Adds a pointer to your function to the listener list
	// - Listeners are called when the tool is switched
	static inline void Listen(ToolEvent_t Func) { m_listeners.push_back(Func); }
	// - Calls all event listeners
	static void ToolEvent(e_tool Event);
};

#endif // CToolBar__H