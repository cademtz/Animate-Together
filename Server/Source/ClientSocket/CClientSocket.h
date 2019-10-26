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

	inline QString Name() const { return m_user; }

protected:
	void HandleMsg(CNetMsg* Msg) override;
	void Disconnected() override;

private:
	ATNet::EProtoStage m_stage = ATNet::ProtocolStage;
	CServer* m_parent;

	QString m_user;
};

#endif // CClientSocket_H