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

// - CNetMsg wraps around raw serialized data for quickly handling networkable message data

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
	inline static CNetMsg* FromData(const QByteArray& Bytes) {
		return FromData(Bytes.size(), Bytes.data());
	}
	static CNetMsg* FromData(unsigned Length, const char* Data);

private:
	unsigned m_len;
	uint8_t m_type;
	char m_rawdata[];
};

class CBaseMsg
{
public:
	enum EType : uint8_t
	{
		ServerMsg = 0,	// - A message box from the server
		ProtocolMsg,	// - Sender's protocol info
		LoginMsg,		// - Sender requesting or sending login info
		KickMsg,		// - User request to kick
		BanMsg,			// - User request to ban
		ChatMsg,		// - Chat message from server or user
		WelcomeMsg,		// - Server has accepted user's login and sends the MOTD
	};

	inline uint8_t Type() const { return m_type; }

	void Send(QTcpSocket* Socket) const
	{
		CNetMsg* msg = NewMsg();
		msg->Send(Socket);
		//delete[](char*)msg;
	}

	virtual ~CBaseMsg() { }

protected:
	CBaseMsg(EType Type) : m_type(Type) { }

	// - Allocates and constructs a CNetMsg from serialized data
	virtual CNetMsg* NewMsg() const = 0;

private:
	EType m_type;
};

class CServerMsg : public CBaseMsg
{
public:
	CServerMsg(CNetMsg* Msg);
	CServerMsg(QString Text) : CBaseMsg(ServerMsg), m_text(Text) { }

	inline QString Text() const { return m_text; }

protected:
	CNetMsg* NewMsg() const override;
		
private:
	QString m_text;
};

class CProtocolMsg : public CBaseMsg
{
public:
	CProtocolMsg(CNetMsg* Msg);
	CProtocolMsg()
		: CBaseMsg(ProtocolMsg), m_prefix(AT_PROTO_PREFIX), m_major(AT_PROTO_MAJOR), m_minor(AT_PROTO_MINOR) { }

	// - Returns true if the protocol will maintain compatibility with current version
	bool Compatible();

	inline QString Prefix() const { return m_prefix; }
	inline unsigned Major() const { return m_major; }
	inline unsigned Minor() const { return m_minor; }

protected:
	CNetMsg* NewMsg() const override;

private:
	QString m_prefix = AT_PROTO_PREFIX;
	unsigned m_major, m_minor;
};

class CLoginMsg : public CBaseMsg
{
public:
	enum EFlags
	{
		TokenFlag = 0, // - Requires or sends a token in 'User' string
		PassFlag = (1 << 0) // - Requires or sends a password in the 'Pass' string
	};

	CLoginMsg(CNetMsg* Msg);
	CLoginMsg(uint8_t ReqFlags = 0) : CBaseMsg(LoginMsg), m_flags(ReqFlags) { }
	CLoginMsg::CLoginMsg(QString User, QString Pass)
		: CBaseMsg(LoginMsg), m_user(User), m_pass(Pass), m_flags(Pass.isEmpty() ? 0 : PassFlag) { }


	inline uint8_t Flags() const { return m_flags; }
	inline QString User() const { return m_user; }
	inline QString Pass() const { return m_pass; }

protected:
	CNetMsg* NewMsg() const override;

private:
	uint8_t m_flags = 0;
	QString m_user, m_pass;
};

class CChatMsg : public CBaseMsg
{
public:
	CChatMsg(CNetMsg* Msg);
	CChatMsg(QString Text = QString()) : CBaseMsg(ChatMsg), m_text(Text) { }

	inline const QString& Text() const { return m_text; }
	inline void SetText(const QString& String) { m_text = String; }

protected:
	CNetMsg* NewMsg() const override;

private:
	QString m_text;
};

class CWelcomeMsg : public CBaseMsg
{
public:
	CWelcomeMsg(CNetMsg* Msg);
	CWelcomeMsg(QString Motd = QString(), bool IsUrl = false)
		: CBaseMsg(WelcomeMsg), m_motd(Motd), m_url(IsUrl) { }

	inline bool IsUrl() const { return m_url; }
	inline QString Motd() const { return m_motd; }

	inline void SetMotd(QString Motd, bool IsUrl = false) { m_motd = Motd, m_url = IsUrl; }

protected:
	CNetMsg* NewMsg() const override;

private:
	bool m_url;
	QString m_motd;
};

#endif // CNetMsg_H