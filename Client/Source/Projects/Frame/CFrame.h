/*	Description:
 *		Base class for a frame which holds the type and state.
*		Different types and states require their own classes for slightly different functionality.
 *
 *	Author: Hold on!
 *	Created: 8/24/2019 5:19:43 PM
 */

#ifndef CFrame_H
#define CFrame_H
#ifdef _WIN32
#pragma once
#endif

#include "Projects/Events/Events.h"
#include "Projects/Events/CEventHandler.h"

class CLayer;
class CFrame : public CEventHandler<CFrameEvent>
{
public:
	enum e_type
	{
		null = 0,
		Raster,
		Vector,
		Stk
	};
	enum e_state
	{
		Empty,	// - A keyframe with no contents
		Key,	// - A keyframe with contents
		Hold	// - Simply holds an existing keyframe
	};

protected:
	e_type m_type;
	e_state m_state;

	CLayer* m_layer = 0;
	CFrame* m_parent = 0;

	CFrame(CLayer* Layer, e_type Type, e_state State) : m_layer(Layer), m_type(Type), m_state(State) { }

public:
	inline e_state State() const { return m_state; }
	inline e_type Type() const { return m_type; }

	// - Returns the layer that holds the frame
	inline CLayer* Layer() const { return m_layer; }

	// - Returns the index in the parent layer's frame list
	size_t Index() const;

	// - Returns the parent frame, which must have the same class
	// - Result may be null if the current state does not require a parent (e.g. Key, Empty)
	template<typename T = CFrame>
	inline T* Parent() const { return (T*)m_parent; }

	// - Updates the state to Empty or Key, indicating whether it should be displayed
	void SetIsEmpty(bool IsEmpty) { m_state = IsEmpty ? Empty : Key; }
};

#endif // CFrame_H