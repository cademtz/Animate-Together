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
#include "CSerialize.h"

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
		Msg_Server = 0,	// - A message box from the server
		Msg_Protocol,	// - Sender's protocol info
		Msg_Login,		// - Sender requesting or sending login info
		Msg_Join,		// - Server accepts login and sends public user info
		Msg_Kick,		// - User request to kick
		Msg_Ban,		// - User request to ban
		Msg_Chat,		// - Chat message from server or user
		Msg_Welcome,	// - Server is sending the MOTD
		Msg_Stroke,		// - A client has made or continued a new stroke
		Msg_Event,		// - Server or client has performed a specific action
	};

	inline uint8_t Type() const { return m_type; }

	inline void Send(QTcpSocket* Socket) const {
		Socket->write(Serialize().Bytes());
	}

	virtual ~CBaseMsg() { }

protected:
	CBaseMsg(EType Type) : m_type(Type) { }

	// - Allocates and constructs a CNetMsg from serialized data
	virtual const CSerialize Serialize() const = 0;

private:
	EType m_type;
};

class CServerMsg : public CBaseMsg
{
public:
	CServerMsg(CNetMsg* Msg);
	CServerMsg(QString Text) : CBaseMsg(Msg_Server), m_text(Text) { }

	inline QString Text() const { return m_text; }

protected:
	const CSerialize Serialize() const override;
		
private:
	QString m_text;
};

class CProtocolMsg : public CBaseMsg
{
public:
	CProtocolMsg(CNetMsg * Msg);
	CProtocolMsg()
		: CBaseMsg(Msg_Protocol), m_prefix(AT_PROTO_PREFIX), m_major(AT_PROTO_MAJOR), m_minor(AT_PROTO_MINOR) { }

	// - Returns true if the protocol will maintain compatibility with current version
	bool Compatible();

	inline QString Prefix() const { return m_prefix; }
	inline unsigned Major() const { return m_major; }
	inline unsigned Minor() const { return m_minor; }

protected:
	const CSerialize Serialize() const override;

private:
	QString m_prefix = AT_PROTO_PREFIX;
	unsigned m_major, m_minor;
};

class CLoginMsg : public CBaseMsg
{
public:
	enum EFlags
	{
		Flag_Required = 0,		// - Servers requires the client to send a login with the specified flags
		Flag_Token = (1 << 0),	// - Expects a token in the 'User' string
		Flag_Pass = (1 << 1)	// - Expects a password in the 'Pass' string
	};

	CLoginMsg(CNetMsg* Msg);
	CLoginMsg(uint8_t ReqFlags = 0) : CBaseMsg(Msg_Login), m_flags(ReqFlags) { }
	CLoginMsg::CLoginMsg(QString User, QString Pass)
		: CBaseMsg(Msg_Login), m_user(User), m_pass(Pass), m_flags(Pass.isEmpty() ? 0 : Flag_Pass) { }

	inline uint8_t Flags() const { return m_flags; }
	inline QString User() const { return m_user; }
	inline QString Pass() const { return m_pass; }

protected:
	const CSerialize Serialize() const override;

private:
	uint8_t m_flags = 0;
	QString m_user, m_pass;
};

class CChatMsg : public CBaseMsg
{
public:
	CChatMsg(CNetMsg* Msg);
	CChatMsg(QString Text = QString()) : CBaseMsg(Msg_Chat), m_text(Text) { }

	inline const QString& Text() const { return m_text; }
	inline void SetText(const QString& String) { m_text = String; }

protected:
	const CSerialize Serialize() const override;

private:
	QString m_text;
};

class CWelcomeMsg : public CBaseMsg
{
public:
	CWelcomeMsg(CNetMsg* Msg);
	CWelcomeMsg(QString Motd = QString(), bool IsUrl = false)
		: CBaseMsg(Msg_Welcome), m_motd(Motd), m_url(IsUrl) { }

	inline bool IsUrl() const { return m_url; }
	inline QString Motd() const { return m_motd; }

	inline void SetMotd(QString Motd, bool IsUrl = false) { m_motd = Motd, m_url = IsUrl; }

protected:
	const CSerialize Serialize() const override;

private:
	bool m_url;
	QString m_motd;
};


class CUser;

class CJoinMsg : public CBaseMsg
{
public:
	CJoinMsg(CNetMsg* Msg);
	CJoinMsg(const CUser* User) : CBaseMsg(Msg_Join), m_user(User) { }

	inline const CUser* User() const { return m_user; }

protected:
	const CSerialize Serialize() const override;

private:
	const CUser* m_user;
};

#endif // CNetMsg_H
