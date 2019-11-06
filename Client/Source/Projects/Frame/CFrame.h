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
#include <Shared/CEventHandler.h>

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

public:
	virtual ~CFrame() { }
	virtual bool IsEmpty() = 0;

	inline bool IsKey() const { return m_key; }
	inline e_type Type() const { return m_type; }

	inline CLayer* Layer() { return m_layer; }
	CProject* Project();
	void Select(bool Selected = true);
	bool IsSelected();

	// - Returns the index in the parent layer's frame list
	size_t Index();

	// - Returns the parent frame, which must have the same class
	// - Result may be null if the current state does not require a parent (e.g. Key, Empty)
	template<typename T = CFrame>
	inline T* Parent() { return (T*)_Parent(); }
	template<typename T = CFrame>
	inline T* Clone() { return (T*)_Clone(); }

protected:
	e_type m_type;
	bool m_key;
	CLayer* m_layer = 0;

	CFrame(CLayer* Layer, e_type Type, bool Hold);

	CFrame* _Parent();
	virtual CFrame* _Clone() = 0;

	friend class CLayer;
	void SetLayer(CLayer* Layer) { m_layer = Layer; }
};

#endif // CFrame_H