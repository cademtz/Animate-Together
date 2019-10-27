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

void CClientSocket::Kick(QString Reason)
{
	if (Reason.isEmpty())
		Reason = tr("Kicked from server");
	SendMsg(CServerMsg(Reason));
	Socket()->close();
}

void CClientSocket::HandleMsg(CNetMsg * Msg)
{
	switch (m_stage)
	{
	case ATNet::ProtocolStage:
		if (Msg->Type() == CBaseMsg::ProtocolMsg && CProtocolMsg(Msg).Compatible())
		{
			m_stage = ATNet::JoinStage;
			qInfo() << Socket()->peerAddress() << ": Good protocol";
		}
		else
			Socket()->abort();
		return;
	case ATNet::JoinStage:
		if (CheckLogin(Msg))
		{
			m_stage = ATNet::FinalStage;
			SendMsg(m_parent->Motd());
		}
		else
			Kick(tr("Invalid login"));
		return;
	}

	switch (Msg->Type())
	{
	case CBaseMsg::ChatMsg:
		qInfo() << Name() << ": " << CChatMsg(Msg).Text();
		break;
	}
}

void CClientSocket::Disconnected() {
	qInfo() << Socket()->peerAddress() << " Disconnected";
}

bool CClientSocket::CheckLogin(CNetMsg * Msg)
{
	if (Msg->Type() != CBaseMsg::LoginMsg)
		return false;

	bool valid = false;
	CLoginMsg login(Msg);
	if (login.User() == "Hold") // Test code
		valid = login.Pass() == "on!";
	else
		valid = true;

	if (valid)
	{
		m_user = login.User();
		m_uuid = m_parent->NewUUID();
	}

	return valid;
}
