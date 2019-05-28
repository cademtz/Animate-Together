/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/3/2019 12:17:56 AM
 */

#include "CUndoStack.h"

std::list<UndoEvent_t> CUndoStack::m_listeners;

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
		if (!action->WasUndone())
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
	if (m_actions.back()->WasUndone())
		epic--;
	else
	{
		for (auto it = ++m_actions.begin(); it != m_actions.end(); it++)
		{
			if (!(*it)->WasUndone())
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
		if ((*it)->WasUndone()) // All redos become invalid, so we must remove them
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
	m_actions.push_front(Action);
	CUndoStack::UndoEvent(Action);
}

void CUndoStack::UndoEvent(const CUndoAction* Undo)
{
	for (auto fn : m_listeners)
		fn(Undo);
}