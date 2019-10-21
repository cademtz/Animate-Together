/*	Description:
 *		Structure for all messages sent over network
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 2:52:22 PM
 */

#ifndef CNetMsg_H
#define CNetMsg_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>
#include <stdint.h>
#include "Config.h"

struct CNetMsg
{
public:
	inline const char* Data() const { return (const char*)this; }
	inline size_t Length() const { return m_len; }
	inline void Send(QTcpSocket& Socket) {
		Socket.write(Data(), m_len);
	}

	// - Casts 'data' to a CNetMsg pointer
	// - Returns 0 instead if the message is incomplete
	static CNetMsg* FromData(unsigned length, char data[])
	{
		if (length >= sizeof(CNetMsg))
		{
			CNetMsg* msg = (CNetMsg*)data;
			if (msg->m_len < length)
				return msg;
		}
		return 0;
	}

private:

	size_t m_len;
	char m_rawdata[];
};

class CBaseMsg
{
public:
	enum e_type : uint8_t
	{
		Debug = 0,
		Protocol,
		Login,
		Kick,
		Ban,
		Chat,
	};

	inline e_type Type() const { return m_type; }

	void Send(QTcpSocket& Socket)
	{
		CNetMsg* msg = NewMsg();
		msg->Send(Socket);
		delete msg;
	}

protected:
	CBaseMsg(e_type Type) : m_type(Type) { }

	// - Allocates and creates a CNetMsg with serialized data
	virtual CNetMsg* NewMsg() = 0;

private:
	e_type m_type;
};

class CProtocolMsg : public CBaseMsg
{
public:
	CProtocolMsg(CNetMsg* Msg);
	CProtocolMsg()
		: CBaseMsg(CBaseMsg::Protocol), m_prefix(AT_PROTO_PREFIX), m_major(AT_PROTO_MAJOR), m_minor(AT_PROTO_MINOR) { }

	inline const char* Prefix() const { return m_prefix; }
	inline unsigned Major() const { return m_major; }
	inline unsigned Minor() const { return m_minor; }

protected:
	CNetMsg* NewMsg();

private:
	char m_prefix[sizeof(AT_PROTO_PREFIX)];
	unsigned m_major, m_minor;
};

#endif // CNetMsg_H