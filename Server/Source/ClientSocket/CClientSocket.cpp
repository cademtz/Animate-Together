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
		switch (CheckLogin(Msg))
		{
		case ELogin::Valid:
			m_stage = ATNet::FinalStage;
			SendMsg(m_parent->Motd());
			break;
		case ELogin::Error:
			Kick(tr("Error confirming login. Retry, update client, or report a bug"));
			break;
		case ELogin::Duplicate:
			Kick(tr("User already joined with same login"));
			break;
		case ELogin::BadInfo:
			Kick(tr("Login info was incorrect"));
			break;
		}
		return;
	}

	switch (Msg->Type())
	{
	case CBaseMsg::ChatMsg:
		qInfo() << User() << ": " << CChatMsg(Msg).Text();
		break;
	}
}

void CClientSocket::Disconnected() {
	qInfo() << Socket()->peerAddress() << " Disconnected";
}

CClientSocket::ELogin CClientSocket::CheckLogin(CNetMsg * Msg)
{
	if (Msg->Type() != CBaseMsg::LoginMsg)
		return ELogin::Error;

	ELogin status = ELogin::Valid;
	CLoginMsg login(Msg);

	for (auto client : m_parent->Clients())
	{
		if (login.User() == client->User())
		{
			status = ELogin::Duplicate;
			break;
		}
	}

	if (status == ELogin::Valid)
	{
		m_user = login.User();
		m_uuid = m_parent->NewUUID();
	}

	return status;
}
