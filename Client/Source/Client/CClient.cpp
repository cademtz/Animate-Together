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
	if (Client().m_stage != ATNet::JoinStage)
		return;
	Client().SendMsg(CLoginMsg(User, Pass));
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
	if (m_stage > ATNet::ProtocolStage) // Priority messages that can occur at any time
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
	case ATNet::ProtocolStage:
		// Check protocol and abort connection if unmatching
		if (Msg->Type() == CBaseMsg::Msg_Protocol && CProtocolMsg(Msg).Compatible())
			m_stage = ATNet::JoinStage;
		else
			Socket()->abort();
		return;
	case ATNet::JoinStage:
		switch(Msg->Type())
		{
		case CBaseMsg::Msg_Login:
			CreateEvent(CLoginMsg(Msg));
			return;
		case CBaseMsg::Msg_Welcome:
		{
			m_stage = ATNet::FinalStage;
			SendMsg(CChatMsg("Hello from animator!"));
			CMotdView::Open(CWelcomeMsg(Msg));
			return;
		}
		}
		Socket()->disconnectFromHost();
		return;
	}

	switch (Msg->Type())
	{

	}
}

void CClient::Error(QTcpSocket::SocketError Error)
{
	QMessageBox msg(QMessageBox::Warning, "Network error", Socket()->errorString());
	msg.setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	msg.exec();
}
