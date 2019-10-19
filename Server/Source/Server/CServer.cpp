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
	connect(this, &QTcpServer::newConnection, this, &CServer::OnConnection);

	if (argc < 2)
		return;

	// TODO: Simple console or file configuration
}

bool CServer::Listen()
{
	if (IsListening())
		return false;

	qInfo() << "Begin listening";
	return listen(QHostAddress::Any, m_port);
}

void CServer::Close() {
	qInfo() << "Closing server";
	close();
}

void CServer::OnConnection()
{
	QTcpSocket* sock = nextPendingConnection();
	qInfo() << "New connection from " << sock->peerAddress();
}
