/*	Description:
 *		A class that describes a handle to an object shared over network.
 *		Valid handles are always non-zero, and are only unique per session.
 *
 *	Author: Hold on!
 *	Created: 11/9/2019 1:04:38 PM
 */

#ifndef CNetObject_H
#define CNetObject_H
#ifdef _WIN32
#pragma once
#endif

#include "Shared/CSerialize.h"

class CNetObject
{
public:
	inline CNetObject(unsigned Handle) : m_handle(Handle) { }
	CNetObject(SerialStream& Data) { Data >> m_handle; }
	inline CNetObject(const CNetObject& Other) : m_handle(Other.m_handle) { }

	inline unsigned Handle() const { return m_handle; }
	inline const CSerialize Serialize() const
	{
		CSerialize data(m_handle);
		SerializeCustom(data);
		return data;
	}

	// - Directly appends the serialized object to 'Data'
	inline void Serialize(CSerialize& Data) const
	{
		Data.Add(m_handle);
	}

	static inline void Reset() { m_nexthandle = 1; }

protected:
	inline CNetObject() : m_handle(_NewHandle()) { }
	void Deserialize(const CSerialize& Data)
	{
		SerialStream stream = Data.Stream();
		stream >> m_handle;
		DeserializeCustom(stream);
	}

	// - If overriden, adding to `Data` will extend CNetObject's returned data in 'Serialize'
	virtual void SerializeCustom(CSerialize& Data) const { }

	// - If overriden, calling 'Deserialize' on the object will further run this
	virtual void DeserializeCustom(SerialStream& Data) { }

private:
	unsigned m_handle;
	static unsigned m_nexthandle;

	static inline unsigned _NewHandle() { return m_nexthandle++; }
};

#endif // CNetObject_H