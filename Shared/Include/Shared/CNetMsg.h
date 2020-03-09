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
#include "NetObjects/CNetObject.h"

class CUser;
class CSharedProject;

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
		Msg_Event,		// - Server or client has performed a specific action
		Msg_Login,		// - Sender requesting or sending login info
		Msg_Join,		// - Server accepts login and sends public user info
		Msg_Leave,		// - Server notifies clients upon a user leaving
		Msg_Kick,		// - User request to kick
		Msg_Ban,		// - User request to ban
		Msg_Chat,		// - Chat message from server or user
		Msg_Welcome,	// - Server is sending the MOTD
		Msg_ProjSetup,	// - Carries necessary data to initialize shared projects
		Msg_Object,		// - Carrries data to initialize a net object
		Msg_Undo,		// - User redid/undid their action action
	};

	inline uint8_t Type() const { return m_type; }

	inline void Send(QTcpSocket* Socket) const {
		Socket->write(Serialize().Bytes());
	}

	virtual CSerialize Serialize() const = 0;
	virtual ~CBaseMsg() { }

protected:
	CBaseMsg(EType Type) : m_type(Type) { }

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
	CSerialize Serialize() const override;
		
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
	CSerialize Serialize() const override;

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
		: CBaseMsg(Msg_Login), m_name(User), m_pass(Pass), m_flags(Pass.isEmpty() ? 0 : Flag_Pass) { }

	inline uint8_t Flags() const { return m_flags; }
	inline QString Name() const { return m_name; }
	inline QString Pass() const { return m_pass; }

protected:
	CSerialize Serialize() const override;

private:
	uint8_t m_flags = 0;
	QString m_name, m_pass;
};

class CChatMsg : public CBaseMsg
{
public:
	CChatMsg(CNetMsg* Msg);
	CChatMsg(QString Text, const CUser* User = 0);

	inline const CNetObject User() const { return m_user; }
	inline const QString& Text() const { return m_text; }
	inline void SetText(const QString& String) { m_text = String; }

protected:
	CSerialize Serialize() const override;

private:
	unsigned m_user;
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
	CSerialize Serialize() const override;

private:
	bool m_url;
	QString m_motd;
};

class CJoinMsg : public CBaseMsg
{
public:
	CJoinMsg(CNetMsg* Msg);
	CJoinMsg(const CUser* User);

	inline QString Name() const { return m_name; }
	inline uint8_t Perms() const { return m_perms; }
	inline unsigned Handle() const { return m_handle; }

protected:
	CSerialize Serialize() const override;

private:
	QString m_name;
	uint8_t m_perms;
	unsigned m_handle;
};

class CLeaveMsg : public CBaseMsg
{
public:
	CLeaveMsg(CNetMsg* Msg) : CBaseMsg(Msg_Leave) {
		CSerialize::Deserialize(Msg->Data(), m_handle);
	}
	CLeaveMsg(const CUser* User);

	inline unsigned UserHandle() const { return m_handle; }

protected:
	CSerialize Serialize() const override { return CSerialize(Type(), m_handle); }

private:
	unsigned m_handle;
};

class CProjSetupMsg : public CBaseMsg
{
public:
	CProjSetupMsg(CNetMsg* Msg);
	CProjSetupMsg(const CSharedProject* Project);

	inline QString Name() const { return m_name; }
	inline unsigned Framerate() const { return m_framerate; }

protected:
	CSerialize Serialize() const override;

private:
	QString m_name;
	unsigned m_framerate;
};

class CUndoMsg : public CBaseMsg
{
public:
	CUndoMsg(CNetMsg* Msg);
	CUndoMsg(const CUser* User, bool Redo = false);

	inline unsigned UserHandle() const { return m_handle; }
	inline bool IsRedo() const { return m_redo; }

protected:
	CSerialize Serialize() const override;

private:
	unsigned m_handle;
	bool m_redo;
};

#endif // CNetMsg_H
