/*	Description:
 *		Holds a stack of all the user's completed actions and handles them properly
 *
 *	Author: Hold on!
 *	Created: 3/2/2019 5:14:31 PM
 */

#ifndef CUndoStack_H
#define CUndoStack_H
#ifdef _WIN32
#pragma once
#endif

#define MAX_ACTIONS 30

#include <list>
#include <qpixmap.h>
#include <functional>
#include "CUndoAction.h"

typedef std::function<void(const CUndoAction*)> UndoEvent_t;

class CUndoStack
{
	// - List contains all reversible user actions
	// - New actions appear in front
	std::list<CUndoAction*> m_actions;
	static std::list<UndoEvent_t> m_listeners;

public:
	~CUndoStack();

	// - Returns true if an undo was available
	bool UndoLast();
	// - Returns true if a redo was available
	bool RedoLast();
	// - Adds an undoable action
	void Push(CUndoAction* Action);
	// - Returns a list of all the actions
	inline const std::list<CUndoAction*>& GetActions() const { return m_actions; }

	// - Adds a pointer to your function to the listener list
	// - Listeners are called on undo, redo, and action events
	static inline void Listen(UndoEvent_t Func) { m_listeners.push_back(Func); }
	// - Calls all event listeners
	static void UndoEvent(const CUndoAction* Undo);
};

#endif // CUndoStack_H