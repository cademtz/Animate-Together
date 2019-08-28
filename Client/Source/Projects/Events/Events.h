/*	Description:
 *		Provides a custom set of events specific to Animate Together
 *
 *	Author: Hold on!
 *	Created: 8/14/2019 7:38:56 PM
 */

#ifndef Events_H
#define Events_H
#ifdef _WIN32
#pragma once
#endif

#include <qevent.h>
//#include "Projects/Palette/CPalette.h"
//#include "Projects/Layer/CLayer.h"

class CProject;

class CCustomEvent
{
public:
	enum class e_type
	{
		null = 0,
		PaletteEvent,
		LayerEvent,
		ProjectEvent,
		FrameEvent
	};

protected:
	e_type m_type = e_type::null;

public:
	CCustomEvent(e_type Type) : m_type(Type) { }
	virtual ~CCustomEvent() { }

	inline e_type Type() const { return m_type; }
};

// Sent when a project's palette has updated

class CPalette;
class CPaletteEvent : public CCustomEvent
{
	CPalette* m_palette;

public:
	CPaletteEvent(CPalette* Palette) : CCustomEvent(e_type::PaletteEvent), m_palette(Palette) { }

	inline CPalette* Palette() const { return m_palette; }
	//inline CProject* Project() const { return m_palette->Project(); }
	CProject* Project() const;
};

// Sent when a project's layers have updated

class CLayer;
class CLayerEvent : public CCustomEvent
{
public:
	enum e_action
	{
		null = 0,
		Add,
		Remove,
		Switched,	// - When the active layer changes
		Hidden,		// - Hidden mode toggled in layer(s)
		Edit,		// - Layer properties changed (name, private, locked, ...)
		Moved		// - Order in the list has changed
	};

private:
	CLayer* m_layer;
	e_action m_action;

public:
	CLayerEvent(CLayer* Layer, e_action Action) : CCustomEvent(e_type::LayerEvent), m_layer(Layer), m_action(Action) { }

	inline CLayer* Layer() const { return m_layer; }
	//inline CProject* Project() const { return m_layer->Project(); }
	CProject* Project() const;
	inline e_action Action() const { return m_action; }
};

// Sent when the active project has changed

class CProjectEvent : public CCustomEvent
{
public:
	enum e_action
	{
		null = 0,
		ActiveProject,	// - The active project changed
		ActiveFrame,	// - The active frame index changed
	};

private:
	CProject* m_project;
	e_action m_action;

public:
	CProjectEvent(CProject * Project, e_action Action) : CCustomEvent(e_type::ProjectEvent), m_project(Project), m_action(Action) { }

	inline CProject* Project() const { return m_project; }
	inline e_action Action() const { return m_action; }
};

class CFrame;
class CFrameEvent : public CCustomEvent
{
public:
	enum e_action
	{
		null = 0,
		Add,
		Remove,
		Replace
	};

private:
	CFrame* m_frame, * m_old;
	e_action m_action;

public:
	CFrameEvent(CFrame* Frame, e_action Action, CFrame* Old = 0) : CCustomEvent(e_type::FrameEvent), m_frame(Frame), m_action(Action), m_old(Old) { }

	// - Gets the frame that previously took the place of the new frame
	// - Result is non-null only in certain actions (e.g. 'Replace')
	inline CFrame* OldFrame() const { return m_old; }
	inline CFrame* Frame() const { return m_frame; }
	inline e_action Action() const { return m_action; }
};

#endif // Events_H