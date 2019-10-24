/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 9:04:06 PM
 */

#include "Shared/CNetMsg.h"
#include <cstring>
#include <cassert>
#include "Serialize.h"

CNetMsg * CNetMsg::FromData(unsigned length, char* Data)
{
	if (length >= sizeof(m_len))
	{
		CNetMsg* msg = (CNetMsg*)Data;
		if (msg->Length() <= length)
			return msg;
	}
	return 0;
}

CProtocolMsg::CProtocolMsg(CNetMsg * Msg) : CBaseMsg(ProtocolMsg)
{
	const char* pos = Msg->Data();
	assert(Msg->Length() > sizeof(m_prefix));

	GetData(m_prefix, pos);
	GetData(m_major, pos);
	GetData(m_minor, pos);
}

CNetMsg * CProtocolMsg::NewMsg()
{
	unsigned len = CalcSize() + len + Type() + AT_PROTO_PREFIX + m_major + m_minor;
	char* data = new char[len], * pos = data;
	NextData(len, pos);
	NextData(Type(), pos);
	NextData(AT_PROTO_PREFIX, pos);
	NextData(m_major, pos);
	NextData(m_minor, pos);
	return CNetMsg::FromData(len, data);
}

CChatMsg::CChatMsg(CNetMsg * Msg) : CBaseMsg(ChatMsg)
{
	const char* pos = Msg->Data();
	int textlen;
	GetData(textlen, pos);
	GetData(m_text, textlen, pos);
}

CChatMsg::CChatMsg(const char* Text) : CBaseMsg(ChatMsg)
{
	if (Text)
		m_text = Text;
}

CNetMsg * CChatMsg::NewMsg()
{
	unsigned len = CalcSize() + len + Type() + m_text.size() + m_text;
	char* data = new char[len], *pos = data;
	NextData(len, pos);
	NextData(Type(), pos);
	NextData(m_text.size(), pos);
	NextData(m_text, pos);
	return CNetMsg::FromData(len, data);
}
