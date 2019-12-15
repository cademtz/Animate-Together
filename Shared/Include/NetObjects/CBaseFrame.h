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

#include "CNetObject.h"
#include "Shared/CSerialize.h"

class CBaseLayer;

class CBaseFrame : public CNetObject
{
public:
	template <typename T = CBaseFrame>
	inline T* Clone() const { return (T*)NewClone(); }
	template<typename T = CBaseFrame>
	inline T* Parent() const { return (T*)m_parent; }
	inline bool IsKey() const { return m_parent; }

	virtual bool IsEmpty() const = 0;

protected:
	// - Creates a hold frame if 'KeyParent' is set
	CBaseFrame(CBaseFrame* KeyParent = nullptr) : m_parent(KeyParent) { }
	void SerializeCustom(CSerialize& Data) const override;

	virtual CBaseFrame* NewClone() = 0;

private:
	CBaseFrame* m_parent;
	CBaseLayer* m_layer;
};

#endif // CBaseFrame_H