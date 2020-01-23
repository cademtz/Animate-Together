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
class CSharedProject;

class CBaseFrame : public CNetObject
{
public:
	enum EType : uint8_t
	{
		Frame_Raster,
		Frame_Vector,
		Frame_Stick,
		Frame_Audio
	};

	inline EType Type() const { return m_type; }

	// - Returns the key that a hold frame is holding
	// - Value will be the frame itself if it is a key
	template<typename T = CBaseFrame>
	inline T* Key() { return (T*)FindKey(); }
	template<typename T = CBaseFrame>
	inline const T* Key() const { return Key<T>(); }

	template<typename T = CBaseLayer>
	inline T* Parent() { return m_parent; }
	template<typename T = CBaseLayer>
	inline const T* Parent() const { return m_parent; }

	inline bool IsKey() const { return m_isKey; }
	virtual bool IsEmpty() const = 0;
	int Index() const;

	template <typename T = CBaseFrame>
	inline T* Clone() const { return (T*)NewClone(); }

protected:
	// - If 'IsKey' is false, a hold frame is created
	CBaseFrame(EType Type, bool IsKey) : m_type(Type), m_isKey(IsKey) { }
	CBaseFrame(EType Type, CSharedProject* Proj, SerialStream& Data);
	void SerializeCustom(CSerialize& Data) const override;

	virtual CBaseFrame* NewClone() const = 0;
	CBaseFrame* FindKey();

	friend CBaseLayer;
	inline void SetParent(CBaseLayer* Layer) { m_parent = Layer; }

private:
	EType m_type;
	bool m_isKey;
	CBaseLayer* m_parent;
};

#endif // CBaseFrame_H