/*	Description:
 *		A shared frame class that holds all necessary networked data, as well as convenience functions.
 *
 *		Details:
 *			If set, functions to access frame-specific data (image audio, or other) will return its parent's.
 *
 *	Author: Hold on!
 *	Created: 11/8/2019 9:48:14 PM
 */

#ifndef CBaseFrame_H
#define CBaseFrame_H
#ifdef _WIN32
#pragma once
#endif

#include "NetObject.h"

class CBaseFrame : public CNetObject
{
public:
	virtual bool IsEmpty() = 0;

protected:
	// - Creates a hold frame
	CBaseFrame(CBaseFrame* KeyParent) : m_parent(KeyParent) { }

	template<typename T = CBaseFrame>
	inline T* Parent() const { return m_parent; }

private:
	CBaseFrame* m_parent;
};

#endif // CBaseFrame_H