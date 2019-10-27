/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "CClient.h"
#include <Shared/CNetMsg.h>
#include <qcoreapplication.h>
#include <qmessagebox.h>

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
	if (m_stage > ATNet::ProtocolStage) // Priority messages that can occur at any time
	{
		switch (Msg->Type())
		{
		case CBaseMsg::ServerMsg:
			QMessageBox(QMessageBox::NoIcon, tr("Server message"), CServerMsg(Msg).Text()).exec();
			break;
		}
	}

	switch (m_stage)
	{
	case ATNet::ProtocolStage:
		// Check protocol and abort connection if unmatching
		if (Msg->Type() == CBaseMsg::ProtocolMsg && CProtocolMsg(Msg).Compatible())
		{
			m_stage = ATNet::JoinStage;
			SendMsg(CLoginMsg("Hold", "on!"));
		}
		else
			Socket()->abort();
		return;
	case ATNet::JoinStage:
		if (Msg->Type() == CBaseMsg::WelcomeMsg)
		{
			m_stage = ATNet::FinalStage;
			SendMsg(CChatMsg("Hello from animator!"));
			CWelcomeMsg welcome(Msg);
			QMessageBox(QMessageBox::NoIcon, "Message of the Day", welcome.Motd()).exec();
			return;
		}
		else
			Socket()->close();
		return;
	}

	switch (Msg->Type())
	{

	}
}