/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/3/2019 12:17:56 AM
 */

#include "CUndoStack.h"

CUndoStack::Listeners_t CUndoStack::m_listeners;

CUndoStack::~CUndoStack()
{
	for (auto action : m_actions)
		delete action;
	m_actions.clear();
}

bool CUndoStack::UndoLast()
{
	for (CUndoAction* action : m_actions)
	{
		if (!action->Undone())
		{
			action->Undo();
			return true;
		}
	}
	return false;
}

bool CUndoStack::RedoLast()
{
	if (!m_actions.size())
		return false;

	std::list<CUndoAction*>::iterator& epic = m_actions.end();
	if (m_actions.back()->Undone())
		epic--;
	else
	{
		for (auto it = ++m_actions.begin(); it != m_actions.end(); it++)
		{
			if (!(*it)->Undone())
			{
				epic = --it;
				break;
			}
		}
	}
	if (epic != m_actions.end())
	{
		(*epic)->Undo();
		return true;
	}
	return false;
}

void CUndoStack::Push(CUndoAction * Action)
{
	for (auto it = m_actions.begin(); it != m_actions.end();)
	{
		if ((*it)->Undone()) // All redos become invalid, so we must remove them
		{
			delete *it;
			if ((it = m_actions.erase(it)) != m_actions.begin())
				it--;
		}
		else it++;
	}
	if (m_actions.size() >= MAX_ACTIONS)
	{
		delete m_actions.back();
		m_actions.pop_back();
	}

	if (m_compound)
	{
		CCompoundAction* comp = (CCompoundAction*)m_actions.front();
		comp->Push(Action);
	}
	else
		m_actions.push_front(Action);

	CUndoStack::UndoEvent(Action);
}

void CUndoStack::Compound(bool Enabled, const char * Disguise)
{
	if (Enabled)
	{
		if (!m_compound)
			m_actions.push_front(new CCompoundAction(Disguise));
	}

	m_compound += Enabled ? 1 : -1;
	if (!m_compound) // All compounding turned off
	{
		CCompoundAction* comp = (CCompoundAction*)m_actions.front();
		if (!comp->Actions().size())
		{
			m_actions.pop_front();
			delete comp; // Don't let empty actions float around
		}
	}
}