/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/18/2019 5:50:34 PM
 */

#include "CServer.h"
#include <qtcpsocket.h>
#include <Shared/NetDefaults.h>

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
	connect(sock, &QTcpSocket::disconnected, this, &CServer::ClientDisconnect);
	connect(sock, &QTcpSocket::readyRead, this, &CServer::Incoming);
	m_clients.push_back(sock);
	qInfo() << sock->peerAddress() << "Connected";
}

void CServer::ClientDisconnect()
{
	QTcpSocket* sock = (QTcpSocket*)QObject::sender();
	m_clients.removeOne(sock);
	qInfo() << sock << " Disconnected";
}

void CServer::Incoming()
{
	QTcpSocket* sock = (QTcpSocket*)QObject::sender();
}
