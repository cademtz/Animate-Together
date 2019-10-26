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
	switch (m_stage)
	{
	case ATNet::ProtocolStage:
		if (Msg->Type() == CBaseMsg::ProtocolMsg)
		{
			if (CProtocolMsg(Msg).Compatible())
			{
				m_stage = ATNet::JoinStage;
				qInfo() << Socket()->peerAddress() << ": Good protocol";
				return;
			}
		}
		Socket()->abort();
		return;
	case ATNet::JoinStage:
		if (Msg->Type() == CBaseMsg::LoginMsg)
		{

		}
	}

	switch (Msg->Type())
	{
	case CBaseMsg::ChatMsg:
		qInfo() << "Chat: " << CChatMsg(Msg).Text();
		break;
	}
}

void CClientSocket::Disconnected() {
	qInfo() << Socket()->peerAddress() << " Disconnected";
}
