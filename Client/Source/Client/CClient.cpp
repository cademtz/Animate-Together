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
	if (m_proj)
	{
		delete m_proj;
		m_proj = nullptr;
	}
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
		m_proj = new CSharedProject();
		m_proj->SetName(info.Name());
		m_proj->SetFramerate(info.Framerate());
		CreateEvent(info);
		break;
	}
	case CBaseMsg::Msg_Event:
	{
		// TO DO: Separate the net event handler of course
		CNetEventInfo type(Msg);
		switch (type.EventType())
		{
		case CNetEvent::Event_LayerAdd:
		{
			CLayerAddMsg add(m_proj, Msg);
			add.Perform();
			auto layers = m_proj->Root().Layers1D();
			for (auto layer : layers)
			{
				QString out = layer->Name();
				CBaseLayer* parent = layer;
				while ((parent = parent->Parent()) && !parent->IsRoot())
					out.prepend(parent->Name() + " > ");
				qInfo() << out;
			}
			break;
		}
		}
		break;
	}
	case CBaseMsg::Msg_Welcome:
	{
		SendMsg(CChatMsg("Hello from animator!"));
		CMotdView::Open(CWelcomeMsg(Msg));
		break;
	}
	case CBaseMsg::Msg_Join:
	{
		CJoinMsg join(Msg);
		m_users.push_back(new CUser(join));
		break;
	}
	case CBaseMsg::Msg_Chat:
	{
		CChatMsg chat(Msg);
		CreateEvent(chat);
		QMessageBox(QMessageBox::NoIcon, FromHandle(chat.User())->Name(), chat.Text()).exec();
		break;
	}
	}
}

void CClient::Error(QTcpSocket::SocketError Error)
{
	QMessageBox msg(QMessageBox::Warning, "Network error", Socket()->errorString());
	msg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	msg.exec();
}
