#include <QtCore/QCoreApplication>
#include "Server/CServer.h"

CServer* server;

void NewConnection();
void Close();

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	a.connect(&a, &QCoreApplication::aboutToQuit, &Close);

	server = new CServer(argc, argv);
	server->Listen();

	return a.exec();
}

void Close() {
	server->Close();
}