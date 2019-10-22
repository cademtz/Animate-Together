/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 9:04:06 PM
 */

#include "CNetMsg.h"
#include <cstring>
#include <cassert>

// I'm just messing around with random stuff for serializing. Could probably make it cleaner or just use a library

template <class P>
inline void NextData(const char* Src, P& DestPos)
{
	size_t len = strlen(Src) + 1;
	memcpy(DestPos, Src, len);
	DestPos += len;
}

template <class D, class P>
inline void NextData(D Src, P& DestPos)
{
	qToBigEndian(Src, DestPos);
	DestPos += sizeof(D);
}

template <class D, class P>
inline void GetData(D& Dest, P& SrcPos)
{
	Dest = qFromBigEndian(*(D*)SrcPos);
	SrcPos += sizeof(D);
}

struct CalcSize
{
	unsigned len;

	inline operator unsigned() const {
		return len;
	}

	inline CalcSize& operator<<(CalcSize other)
	{
		len += other;
		return *this;
	}

	inline CalcSize& operator<<(const char* Str) {
		len += strlen(Str) + 1;
		return *this;
	}

	template <class T>
	inline CalcSize& operator<<(T)
	{
		len += sizeof(T);
		return *this;
	}

	CalcSize() : len(0) { }
};

CProtocolMsg::CProtocolMsg(CNetMsg * Msg) : CBaseMsg(Protocol)
{
	const char* pos = Msg->Data() + sizeof(m_prefix);
	assert(Msg->Length() > sizeof(m_prefix));

	strcpy_s(m_prefix, Msg->Data());
	GetData(m_major, pos);
	GetData(m_minor, pos);
}

CNetMsg * CProtocolMsg::NewMsg()
{
	unsigned len = CalcSize() << len << Type() << AT_PROTO_PREFIX;
	char* data = new char[len], * pos = data;
	NextData(len, pos);
	NextData((uint8_t)Type(), pos);
	NextData(AT_PROTO_PREFIX, pos);
	return CNetMsg::FromData(len, data);
}