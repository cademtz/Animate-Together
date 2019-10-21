/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "ATClient.h"
#include <Shared/Config.h>
#include <Shared/CNetMsg.h>
#include <qcoreapplication.h>

using namespace ATClient::Private;

void ATClient::Init()
{
	_sock = new QTcpSocket(qApp);
	_sock->connect(_sock, &QTcpSocket::connected, &ATClient::Private::Connected);
	_sock->connect(_sock, &QTcpSocket::readyRead, &ATClient::Private::Incoming);
	_sock->connect(_sock, &QTcpSocket::disconnected, &ATClient::Private::Disconnected);
	_sock->connect(_sock, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::error), &ATClient::Private::Error);
}

void ATClient::Connect(QString Host)
{
	QStringList info = Host.split(':');
	int port;
	if (info.size() < 2) // Use default if port specified
		port = AT_DEFPORT;
	else
		port = info[1].toInt();

	_sock->connectToHost(info.front(), port);
}

void ATClient::Private::Connected() {
}

void ATClient::Private::Disconnected() {
}

void ATClient::Close() {
	_sock->close();
}

void ATClient::Private::Incoming()
{
}

void ATClient::Private::Error(QTcpSocket::SocketError Error) {
	qInfo() << Error;
}
