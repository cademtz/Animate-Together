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

	inline CPalette* Palette() { return m_palette; }
	CProject* Project();
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
	size_t m_oldindex;

public:
	CLayerEvent(CLayer* Layer, e_action Action, size_t Index = UINT_MAX);

	inline CLayer* Layer() { return m_layer; }
	CProject* Project();
	inline e_action Action() const { return m_action; }

	// - In the 'Moved' and 'Remove' events, it returns the index before the event
	// - Otherwise, it is the layer's index after the event or UINT_MAX if no layer specified
	// - Current index can be found using the layer or project.
	inline size_t OldIndex() const { return m_oldindex; }
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

	inline CProject* Project() { return m_project; }
	inline e_action Action() const { return m_action; }
};

// Sent when a layer's frame or frames are updated

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
	size_t m_oldindex;

public:
	CFrameEvent(CFrame* Frame, e_action Action, CFrame* Old = 0, size_t OldIndex = UINT_MAX);

	// - Gets the frame that previously took the place of the new frame
	// - Result is non-null only in certain actions (e.g. 'Replace')
	inline CFrame* OldFrame() { return m_old; }
	inline CFrame* Frame() { return m_frame; }
	inline e_action Action() const { return m_action; }

	// - In the 'Remove' event it returns the index before the event
	// - Otherwise, it is the frame's index after the event or UINT_MAX if no frame specified
	// - Current index can be found using the layer or project.
	inline size_t OldIndex() const { return m_oldindex; }
};

#endif // Events_H