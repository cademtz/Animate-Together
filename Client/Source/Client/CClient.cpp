/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "CClient.h"
#include <Shared/CNetMsg.h>
#include <qcoreapplication.h>

void CClient::Connect(QString Host)
{
	QStringList info = Host.split(':');
	int port;
	if (info.size() < 2) // Use default if port specified
		port = AT_DEFPORT;
	else
		port = info[1].toInt();

	Client().Socket()->connectToHost(info.front(), port);
}

void CClient::Close() {
	Client().Socket()->close();
}

void CClient::Connected()
{
	m_stage = ATNet::ProtocolStage;
	SendMsg(CProtocolMsg());
}

void CClient::Disconnected() {
	m_stage = ATNet::ClosedStage;
}

void CClient::HandleMsg(CNetMsg * Msg)
{
	if (m_stage == ATNet::ProtocolStage)
	{
		// Check protocol and abort connection if unmatching
		if (Msg->Type() == CBaseMsg::ProtocolMsg)
		{
			//if (ATNet::GoodProtocol(CProtocolMsg(Msg)))
			CProtocolMsg proto(Msg);
			if (!strcmp(proto.Prefix(), AT_PROTO_PREFIX) &&
				proto.Major() == AT_PROTO_MAJOR &&
				proto.Minor() == AT_PROTO_MINOR)
			{
				m_stage = ATNet::JoinStage;
				SendMsg(CChatMsg("Hello from animator!"));
				return;
			}
		}
		Socket()->abort();
		return;
	}

	switch (Msg->Type())
	{

	}
}