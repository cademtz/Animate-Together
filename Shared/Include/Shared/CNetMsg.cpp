/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 9:04:06 PM
 */

#include "CNetMsg.h"
#include "NetObjects/CUser.h"
#include "CSharedProject.h"

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

CSerialize CServerMsg::Serialize() const {
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

CSerialize CProtocolMsg::Serialize() const {
	return CSerialize(Type(), m_prefix, m_major, m_minor);
}

CLoginMsg::CLoginMsg(CNetMsg * Msg) : CBaseMsg(Msg_Login) {
	CSerialize::Deserialize(Msg->Data(), m_flags, m_name, m_pass);
}

CSerialize CLoginMsg::Serialize() const {
	return CSerialize(Type(), m_flags, m_name.utf16(), m_pass);
}

CChatMsg::CChatMsg(CNetMsg * Msg) : CBaseMsg(Msg_Chat) {
	CSerialize::Deserialize(Msg->Data(), m_user, m_text);
}

CChatMsg::CChatMsg(QString Text, const CUser* Sender)
	: CBaseMsg(Msg_Chat), m_text(Text)
{
	if (Sender)
		m_user = Sender->Handle();
}

CSerialize CChatMsg::Serialize() const {
	return CSerialize(Type(), m_user, m_text.utf16());
}

CWelcomeMsg::CWelcomeMsg(CNetMsg * Msg) : CBaseMsg(Msg_Welcome) {
	CSerialize::Deserialize(Msg->Data(), m_url, m_motd);
}

CSerialize CWelcomeMsg::Serialize() const {
	return CSerialize(Type(), m_url, m_motd.utf16());
}

CJoinMsg::CJoinMsg(CNetMsg * Msg) : CBaseMsg(Msg_Join) {
	CSerialize::Deserialize(Msg->Data(), m_name, m_perms, m_handle);
}

CJoinMsg::CJoinMsg(const CUser* User)
	: CBaseMsg(Msg_Join), m_name(User->Name()), m_perms(User->Perms()), m_handle(User->Handle()) {
}

CSerialize CJoinMsg::Serialize() const {
	return CSerialize(Type(), m_name.utf16(), m_perms, m_handle);
}

CProjSetupMsg::CProjSetupMsg(CNetMsg * Msg) : CBaseMsg(Msg_ProjSetup)
{
	CSerialize::Deserialize(Msg->Data(), m_name, m_framerate);
}

CProjSetupMsg::CProjSetupMsg(const CSharedProject * Project)
	: CBaseMsg(Msg_ProjSetup), m_name(Project->Name()), m_framerate(Project->Framerate())
{
}

CSerialize CProjSetupMsg::Serialize() const {
	return CSerialize(Type(), m_name.utf16(), m_framerate);
}
