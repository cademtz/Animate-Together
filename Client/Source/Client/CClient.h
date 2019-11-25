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
class CUser;
class CSharedProject;

class CClient : public CSocketMgr, public CEventHandler<CBaseMsg>
{
public:
	static void Connect(QString Host);
	static void Login(const QString& User, const QString& Pass = QString());
	static void Close();

	// - Returns the client's own user info
	// - Value is always null while not joined to a server
	inline static const CUser* LocalUser() { return Client().m_self; }
	static const CUser* FromHandle(CNetObject Object);

private:
	static inline CClient& Client()
	{
		static CClient client;
		return client;
	}

	CClient() : CSocketMgr(), m_stage(ATNet::Stage_Closed) { }
	~CClient();

	ATNet::EProtoStage m_stage;
	CSharedProject* m_proj = 0;
	QList<CUser*> m_users;
	CUser* m_self = nullptr;

	void Connected() override;
	void Disconnected() override;
	void HandleMsg(CNetMsg* Msg) override;
	void Error(QTcpSocket::SocketError Error);
};

#endif // CClient_H