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
#include <Shared/CEventHandler.h>

class CUndoStack : public CEventHandler<CUndoAction>
{
public:
	~CUndoStack();

	bool UndoLast();
	bool RedoLast();

	// - Adds an undoable action
	void Push(CUndoAction* Action);

	// - Enabled:	If true, always creates a new compound action for adding following actions to.
	// - Disguise:	Set a custom name for the action. Useful for naming special user actions
	void Compound(bool Enabled, const char* Disguise = 0);

	// - Returns a list of all the actions
	inline const std::list<CUndoAction*>& GetActions() const { return m_actions; }

	static inline void UndoEvent(CUndoAction* Undo) { CreateEvent(*Undo); }

private:
	std::list<CUndoAction*> m_actions;
	int m_compound = 0;
};

#endif // CUndoStack_H