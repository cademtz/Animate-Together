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
	Socket()->disconnectFromHost();
}

void CClientSocket::HandleMsg(CNetMsg * Msg)
{
	switch (m_stage)
	{
	case ATNet::ProtocolStage:
		if (Msg->Type() == CBaseMsg::Msg_Protocol && CProtocolMsg(Msg).Compatible())
		{
			m_stage = ATNet::JoinStage;
			SendMsg(m_parent->Auth());
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
	case CBaseMsg::Msg_Chat:
		qInfo() << User() << ": " << CChatMsg(Msg).Text();
		break;
	}
}

void CClientSocket::Disconnected() {
	qInfo() << Socket()->peerAddress() << " Disconnected";
}

CClientSocket::ELogin CClientSocket::CheckLogin(CNetMsg * Msg)
{
	if (Msg->Type() != CBaseMsg::Msg_Login)
		return ELogin::Error;

	CLoginMsg login(Msg);
	if (m_parent->Auth().Flags() &CLoginMsg::PassFlag)
		if (login.Pass() != m_parent->Pass())
			return ELogin::BadInfo;

	for (auto client : m_parent->Clients())
		if (login.User() == client->User())
			return ELogin::Duplicate;

	m_user = login.User();
	m_uuid = m_parent->NewUUID();

	return ELogin::Valid;
}
