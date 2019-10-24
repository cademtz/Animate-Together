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

#include <stdint.h>
#include <qendian.h>
#include <qtcpsocket.h>
#include "Config.h"

struct CNetMsg
{
	inline const char* Raw() const { return (const char*)this; }
	inline const char* Data() const { return m_rawdata; }
	inline size_t Length() const { return qFromBigEndian(m_len); }
	inline uint8_t Type() const { return qFromBigEndian(m_type); }
	inline void Send(QTcpSocket* Socket) {
		Socket->write(Raw(), Length());
	}

	// - Casts data to a CNetMsg pointer
	// - Returns 0 instead if the message is incomplete
	inline static CNetMsg* FromData(QByteArray& Bytes) {
		return FromData(Bytes.size(), Bytes.data());
	}
	static CNetMsg* FromData(unsigned Length, char* Data);

private:
	unsigned m_len;
	uint8_t m_type;
	char m_rawdata[];
};

class CBaseMsg
{
public:
	enum e_type : uint8_t
	{
		DebugMsg = 0,
		ProtocolMsg,
		LoginMsg,
		KickMsg,
		BanMsg,
		ChatMsg,
	};

	inline uint8_t Type() const { return m_type; }

	void Send(QTcpSocket* Socket)
	{
		CNetMsg* msg = NewMsg();
		msg->Send(Socket);
		delete msg;
	}

	virtual ~CBaseMsg() { }

protected:
	CBaseMsg(e_type Type) : m_type(Type) { }

	// - Allocates and constructs a CNetMsg from serialized data
	virtual CNetMsg* NewMsg() = 0;

private:
	e_type m_type;
};

class CProtocolMsg : public CBaseMsg
{
public:
	CProtocolMsg(CNetMsg* Msg);
	CProtocolMsg()
		: CBaseMsg(ProtocolMsg), m_prefix(AT_PROTO_PREFIX), m_major(AT_PROTO_MAJOR), m_minor(AT_PROTO_MINOR) { }

	inline const char* Prefix() const { return m_prefix; }
	inline unsigned Major() const { return m_major; }
	inline unsigned Minor() const { return m_minor; }

protected:
	CNetMsg* NewMsg();

private:
	char m_prefix[sizeof(AT_PROTO_PREFIX)];
	unsigned m_major, m_minor;
};

class CChatMsg : public CBaseMsg
{
public:
	CChatMsg(CNetMsg* Msg);
	CChatMsg(const char* Text = 0);

	inline const QString& Text() const { return m_text; }
	inline void SetText(const QString& String) { m_text = String; }

protected:
	CNetMsg* NewMsg();

private:
	QString m_text;
};

#endif // CNetMsg_H