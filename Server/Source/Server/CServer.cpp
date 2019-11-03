/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/18/2019 5:50:34 PM
 */

#include "CServer.h"
#include <qtcpsocket.h>
#include <Shared/CNetMsg.h>
#include <Shared/Protocol.h>
#include <Shared/CJsonConfig.h>
#include "ClientSocket/CClientSocket.h"

CServer::CServer(int argc, char * argv[]) : m_port(AT_DEFPORT)
{
	connect(this, &QTcpServer::newConnection, this, &CServer::ClientConnect);
	CServerCfg cfg;
	m_port = cfg.Port();
	m_pass = cfg.Password();

	QFile motdf = cfg.MotdFile();
	if (motdf.exists())
	{
		motdf.open(QIODevice::ReadOnly);
		m_motd.SetMotd(motdf.readAll());
	}

	if (argc < 2)
		return;

	// TODO: Simple console or file configuration
}

bool CServer::Listen()
{
	if (IsListening())
		return false;

	qInfo() << "Listening on port " << m_port;
	return listen(QHostAddress::Any, m_port);
}

void CServer::Close() {
	qInfo() << "Closing server";
	close();
}

void CServer::ClientConnect()
{
	QTcpSocket* sock = nextPendingConnection();
	sock->connect(sock, &QTcpSocket::disconnected, this, &CServer::ClientDisconnect);
	m_clients.push_back(new CClientSocket(sock, this));
	qInfo() << sock->peerAddress() << "Connected";
}

void CServer::ClientDisconnect()
{
	QTcpSocket* sock = (QTcpSocket*)QObject::sender();
	for (int i = 0; i < m_clients.size(); i++)
	{
		if (m_clients[i]->Socket() == sock)
		{
			delete m_clients[i];
			m_clients.removeAt(i);
			break;
		}
	}
	qInfo() << sock->peerAddress() << " Disconnected";
}

CClientSocket * CServer::GetClient(QTcpSocket * Socket)
{
	for (auto client : m_clients)
		if (client->Socket() == Socket)
			return client;
	return 0;
}
