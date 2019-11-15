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

class CClientSocket : public CSocketMgr
{
public:
	CClientSocket(QTcpSocket* Socket, CServer* Parent);

	inline QString User() const { return m_user; }
	inline ATNet::EProtoStage Stage() const { return m_stage; }
	void Kick(QString Reason);

protected:
	void HandleMsg(CNetMsg* Msg) override;
	void Disconnected() override;

private:
	enum class ELogin {
		Valid = 0, Error, BadInfo, Duplicate
	};
	// - Returns login status and sets user information if valid
	// - Method of checking depends on server configuration
	ELogin CheckLogin(CNetMsg* Msg);

	ATNet::EProtoStage m_stage = ATNet::ProtocolStage;
	CServer* m_parent;

	// - User ID, unique within session
	unsigned m_uuid = 0;
	QString m_user;
};

#endif // CClientSocket_H