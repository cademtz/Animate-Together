#include <QtCore/QCoreApplication>
#include <qtcpserver.h>
#include <qtcpsocket.h>
#include <qdebug.h>

QTcpServer* server;

void NewConnection();
void Close();

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	a.connect(&a, &QCoreApplication::aboutToQuit, &Close);

	qInfo() << "Creating server...";

	server = new QTcpServer(&a);
	server->connect(server, &QTcpServer::newConnection, &NewConnection);
	server->listen(QHostAddress::Any, 65535);

	return a.exec();
}

void NewConnection()
{
	QTcpSocket* sock = server->nextPendingConnection();
	qInfo() << "New connection from " << sock->peerAddress();
}

void Close() {
	qInfo() << "Closing server";
	server->close();
}