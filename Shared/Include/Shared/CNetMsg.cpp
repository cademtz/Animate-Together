/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 9:04:06 PM
 */

#include "CNetMsg.h"
#include "NetObjects/CUser.h"

CNetMsg * CNetMsg::FromData(unsigned length, const char* Data)
{
	if (length >= sizeof(m_len))
	{
		CNetMsg* msg = (CNetMsg*)Data;
		if (msg->Length() <= length)
			return msg;
	}
	return 0;
}

CServerMsg::CServerMsg(CNetMsg * Msg) : CBaseMsg(Msg_Server) {
	CSerialize::Deserialize(Msg->Data(), m_text);
}

const CSerialize CServerMsg::Serialize() const {
	return CSerialize(Type(), m_text.utf16());
}

CProtocolMsg::CProtocolMsg(CNetMsg * Msg) : CBaseMsg(Msg_Protocol) {
	CSerialize::Deserialize(Msg->Data(), m_prefix, m_major, m_minor);
}

bool CProtocolMsg::Compatible()
{
	return Prefix() == AT_PROTO_PREFIX &&
		Major() == AT_PROTO_MAJOR &&
		Minor() == AT_PROTO_MINOR;
}

const CSerialize CProtocolMsg::Serialize() const {
	return CSerialize(Type(), m_prefix, m_major, m_minor);
}

CLoginMsg::CLoginMsg(CNetMsg * Msg) : CBaseMsg(Msg_Login) {
	CSerialize::Deserialize(Msg->Data(), m_flags, m_user, m_pass);
}

const CSerialize CLoginMsg::Serialize() const {
	return CSerialize(Type(), m_flags, m_user.utf16(), m_pass);
}

CChatMsg::CChatMsg(CNetMsg * Msg) : CBaseMsg(Msg_Chat) {
	CSerialize::Deserialize(Msg->Data(), m_text);
}

const CSerialize CChatMsg::Serialize() const {
	return CSerialize(Type(), m_text.utf16());
}

CWelcomeMsg::CWelcomeMsg(CNetMsg * Msg) : CBaseMsg(Msg_Welcome) {
	CSerialize::Deserialize(Msg->Data(), m_url, m_motd);
}

const CSerialize CWelcomeMsg::Serialize() const {
	return CSerialize(Type(), m_url, m_motd.utf16());
}

CJoinMsg::CJoinMsg(CNetMsg * Msg) : CBaseMsg(Msg_Join)
{
	QString name;
	uint8_t perms;
	CSerialize::Deserialize(Msg->Data(), m_handle, name, perms);
}

const CSerialize CJoinMsg::Serialize() const
{
	return CSerialize();
	// return CSerialize(Type(), m_handle, m_user->Name().utf16(), m_user->Perms());
}
