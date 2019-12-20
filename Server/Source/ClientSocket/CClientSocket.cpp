/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/22/2019 9:48:42 PM
 */

#include "CClientSocket.h"
#include <qtcpserver.h>
#include <Shared/CNetMsg.h>
#include <NetObjects/CUser.h>
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
	case ATNet::Stage_Protocol:
		if (Msg->Type() == CBaseMsg::Msg_Protocol && CProtocolMsg(Msg).Compatible())
		{
			m_stage = ATNet::Stage_Join;
			SendMsg(m_parent->Auth());
			qInfo() << Socket()->peerAddress() << ": Good protocol";
		}
		else
			Socket()->abort();
		return;
	case ATNet::Stage_Join:
		switch (Msg->Type())
		{
		case CBaseMsg::Msg_Login:
		{
			CLoginMsg login(Msg);
			switch (CheckLogin(login))
			{
			case ELogin::Valid:
			{
				m_stage = ATNet::Stage_Final;
				m_user = new CUser(login.Name(), CUser::Perm_Guest);
				Joined();
				break;
			}
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
		}
		}
		return;
	}

	switch (Msg->Type())
	{
	case CBaseMsg::Msg_Chat:
	{
		CChatMsg chat(Msg);
		qInfo() << m_user->Name() << ": " << chat.Text();
		m_parent->SendAll(CChatMsg(chat.Text(), m_user));
		break;
	}
	case CBaseMsg::Msg_Event:
	{
		CNetEventInfo eventinfo(Msg);
		switch (eventinfo.EventType())
		{
		case CNetEvent::Event_LayerAdd:
		{
			if (!(m_user->Perms() & CUser::Perm_Guest))
				break;

			CLayerAddMsg add(m_parent->Project(), Msg);
			add.Perform();
			m_parent->SendAll(add);

			auto layers = add.Project()->Root().Layers1D();
			for (auto layer : layers)
			{
				QString out = layer->Name();
				CBaseLayer* parent = layer;
				while ((parent = parent->Parent()) && !parent->IsRoot())
					out.prepend(parent->Name() + " > ");
				qInfo() << out;
			}
		}
		}
	}
	}
}

void CClientSocket::Disconnected()
{
	qInfo() << (m_user ? m_user->Name() : "") << " Disconnected" << "(IP: " << Socket()->peerAddress() << ')';
	if (m_user)
		m_parent->SendAll(CLeaveMsg(m_user));
}

CClientSocket::ELogin CClientSocket::CheckLogin(const CLoginMsg& Login)
{
	if (m_parent->Auth().Flags() &CLoginMsg::Flag_Pass)
		if (Login.Pass() != m_parent->Pass())
			return ELogin::BadInfo;

	for (auto client : m_parent->Clients())
		if (client->User() && Login.Name() == client->User()->Name())
			return ELogin::Duplicate;

	return ELogin::Valid;
}

void CClientSocket::Joined()
{
	// The first message a new joining user recieves will be their own information
	CJoinMsg join(m_user);
	m_parent->SendAll(join);
	SendMsg(m_parent->Motd());

	for (auto client : m_parent->Clients()) // Send all users info to client
		if (client != this)
			SendMsg(CJoinMsg(client->User()));

	CSharedProject* proj = m_parent->Project();
	SendMsg(CProjSetupMsg(proj));

	auto layers = proj->Root().Layers1D();
	for (auto layer : layers) // Send all layers in order
		SendMsg(CLayerAddMsg(layer, true));
}
