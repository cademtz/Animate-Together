/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/22/2019 9:48:42 PM
 */

#include "CClientSocket.h"
#include <qtcpserver.h>
#include <Shared/CNetMsg.h>
#include "Server/CServer.h"

CClientSocket::CClientSocket(QTcpSocket * Socket, CServer * Parent)
	: CSocketMgr(Socket, Parent), m_parent(Parent) {
	SendMsg(CProtocolMsg());
}

void CClientSocket::HandleMsg(CNetMsg * Msg)
{
	if (m_stage == ATNet::ProtocolStage)
	{
		if (Msg->Type() == CBaseMsg::ProtocolMsg)
		{
			CProtocolMsg proto(Msg);
			if (!strcmp(proto.Prefix(), AT_PROTO_PREFIX) &&
				proto.Major() == AT_PROTO_MAJOR &&
				proto.Minor() == AT_PROTO_MINOR)
			{
				m_stage = ATNet::JoinStage;
				qInfo() << Socket()->peerAddress() << ": Good protocol";
				return;
			}
		}
		Socket()->abort();
		return;
	}

	switch (Msg->Type())
	{
	case CBaseMsg::ChatMsg:
		qInfo() << "Chat: " << CChatMsg(Msg).Text();
		break;
	}
}