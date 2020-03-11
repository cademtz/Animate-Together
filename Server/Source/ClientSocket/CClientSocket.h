/*	Description:
 *		A class to pair sockets with client-specific information and handle messages use Animate Together's protocol
 *
 *	Author: Hold on!
 *	Created: 10/22/2019 5:58:15 PM
 */

#ifndef CClientSocket_H
#define CClientSocket_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>
#include <Shared/CSocketMgr.h>
#include <Shared/Protocol.h>

class CNetMsg;
class CServer;
class CUser;

class CClientSocket : public CSocketMgr
{
public:
	CClientSocket(QTcpSocket* Socket, CServer* Parent);
	~CClientSocket() {
		if (m_user)
			delete m_user;
	}

	// - Returns the user belonging to the socket
	// - Value is null if the client hasn't reached final stage
	inline CUser* User() const { return m_user; }
	inline ATNet::EProtoStage Stage() const { return m_stage; }
	void Kick(QString Reason);

protected:
	void HandleMsg(CNetMsg* Msg) override;
	void Disconnected() override;

private:
	enum class ELogin {
		Valid = 0, Error, BadInfo, Duplicate
	};

	void HandleEvent(CNetMsg* Msg);
	ELogin CheckLogin(const CLoginMsg& Login);

	// - Called when the client has successfully joined a session
	// - Sends current project data needed for the client side project
	void Joined();

	ATNet::EProtoStage m_stage = ATNet::Stage_Protocol;
	CServer* m_parent;
	CUser* m_user = 0;
};

#endif // CClientSocket_H