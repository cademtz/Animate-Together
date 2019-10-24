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
#include "ClientSocket/CClientSocket.h"

CServer::CServer(int argc, char * argv[]) : m_port(AT_DEFPORT)
{
	connect(this, &QTcpServer::newConnection, this, &CServer::ClientConnect);

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
