/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "CClient.h"
#include <qcoreapplication.h>
#include <qmessagebox.h>
#include <Shared/CNetMsg.h>
#include <Shared/CSharedProject.h>
#include <NetObjects/CUser.h>
#include "Widgets/MotdView/CMotdView.h"

CClient::Listeners_t CClient::m_listeners;

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

void CClient::Login(const QString & User, const QString & Pass)
{
	if (Client().m_stage != ATNet::Stage_Join)
		return;
	Client().SendMsg(CLoginMsg(User, Pass));
}

void CClient::Close() {
	Client().Socket()->close();
}

const CUser * CClient::FromHandle(const CNetObject& Object)
{
	for (auto user : Client().m_users)
		if (user->Handle() == Object.Handle())
			return user;
	return 0;
}

CClient::~CClient()
{
	Socket()->disconnectFromHost();
}

void CClient::Connected()
{
	m_stage = ATNet::Stage_Protocol;
	SendMsg(CProtocolMsg());
}

void CClient::Disconnected() 
{
	m_stage = ATNet::Stage_Closed;

	for (auto user : m_users)
		delete user;
	m_users.clear();

	if (m_self)
		m_self = nullptr;
}

void CClient::HandleMsg(CNetMsg * Msg)
{
	if (m_stage > ATNet::Stage_Protocol) // Priority messages that can occur at any time
	{
		switch (Msg->Type())
		{
		case CBaseMsg::Msg_Server:
			QMessageBox(QMessageBox::NoIcon, tr("Server message"), CServerMsg(Msg).Text()).exec();
			break;
		}
	}

	switch (m_stage)
	{
	case ATNet::Stage_Protocol:
		// Check protocol and abort connection if unmatching
		if (Msg->Type() == CBaseMsg::Msg_Protocol && CProtocolMsg(Msg).Compatible())
			m_stage = ATNet::Stage_Join;
		else
			Socket()->abort();
		return;
	case ATNet::Stage_Join:
		switch(Msg->Type())
		{
		case CBaseMsg::Msg_Login:
			CreateEvent(CLoginMsg(Msg));
			return;
		case CBaseMsg::Msg_Join:
		{
			m_stage = ATNet::Stage_Final;
			CJoinMsg join(Msg);
			m_self = new CUser(join);
			m_users.push_back(m_self);
			return;
		}
		}
		Socket()->disconnectFromHost();
		return;
	}

	switch (Msg->Type())
	{
	case CBaseMsg::Msg_ProjSetup:
	{
		CProjSetupMsg info(Msg);
		CSharedProject* old = m_proj;
		m_proj = new CSharedProject();
		m_proj->SetName(info.Name());
		m_proj->SetFramerate(info.Framerate());
		CreateEvent(info);
		delete old;
		break;
	}
	case CBaseMsg::Msg_Event:
	{
		HandleEvent(Msg);
		break;
	}
	case CBaseMsg::Msg_Join:
	{
		CJoinMsg join(Msg);
		m_users.push_back(new CUser(join));
		CreateEvent(join);
		break;
	}
	case CBaseMsg::Msg_Leave:
	{
		CLeaveMsg left(Msg);
		CreateEvent(left);

		for (auto it = m_users.begin(); it != m_users.end(); it++)
		{
			if (left.UserHandle() == (*it)->Handle())
			{
				m_users.erase(it);
				break;
			}
		}
		break;
	}
	case CBaseMsg::Msg_Welcome:
		CMotdView::Open(CWelcomeMsg(Msg));
		break;
	case CBaseMsg::Msg_Chat:
		CreateEvent(CChatMsg(Msg));
		break;
	}
}

void CClient::HandleEvent(CNetMsg * Msg)
{
	CNetEventInfo info(Msg);
	CNetEvent* e;
	switch (info.EventType())
	{
	case CNetEvent::Event_LayerAdd:
		e = new CLayerAddMsg(m_proj, Msg);
		break;
	case CNetEvent::Event_LayerEdit:
		e = new CLayerEditMsg(m_proj, Msg);
		break;
	case CNetEvent::Event_FrameAdd:
		e = new CFrameAddMsg(m_proj, Msg);
		break;
	}
	e->Perform();
	CreateEvent(*e);
	// TODO: Net messages must include user who originally created them
}

void CClient::Error(QTcpSocket::SocketError Error)
{
	QMessageBox msg(QMessageBox::Warning, "Network error", Socket()->errorString());
	msg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	msg.exec();
}
