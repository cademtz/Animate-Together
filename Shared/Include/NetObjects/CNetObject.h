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

class CNetObject
{
public:
	inline CNetObject(const CNetObject& Other) : m_handle(Other.m_handle) { }

	inline unsigned Handle() const { return m_handle; }
	static inline void Reset() { m_nexthandle = 1; }

protected:
	inline CNetObject(unsigned Handle = _NewHandle()) : m_handle(Handle) { }


private:
	unsigned m_handle;
	static unsigned m_nexthandle;

	static inline unsigned _NewHandle() { return m_nexthandle++; }
};

#endif // CNetObject_H