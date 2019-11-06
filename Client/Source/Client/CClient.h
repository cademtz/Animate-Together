/*	Description:
 *		Provides functions for managing a connection to servers
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:16:47 PM
 */

#ifndef CClient_H
#define CClient_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>
#include <Shared/Protocol.h>
#include <Shared/CSocketMgr.h>
#include <Shared/CEventHandler.h>

class CProject;
class CNetMsg;
class CBaseMsg;

class CClient : public CSocketMgr, public CEventHandler<CBaseMsg>
{
public:
	static void Connect(QString Host);
	static void Login(const QString& User, const QString& Pass = QString());
	static void Close();

private:
	static inline CClient& Client()
	{
		static CClient client;
		return client;
	}

	CClient() : CSocketMgr(), m_stage(ATNet::ClosedStage) { }
	~CClient() { Socket()->disconnectFromHost(); }

	ATNet::EProtoStage m_stage;
	CProject* m_proj;

	void Connected() override;
	void Disconnected() override;
	void HandleMsg(CNetMsg* Msg) override;
	void Error(QTcpSocket::SocketError Error);
};

#endif // CClient_H