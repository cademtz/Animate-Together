#include <QtCore/QCoreApplication>
#include "Server/CServer.h"

CServer* server;

void Close();

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	a.connect(&a, &QCoreApplication::aboutToQuit, &Close);

	server = new CServer(argc, argv);
	CSharedProject* proj = new CSharedProject();
	proj->SetName("My epic project");
	proj->SetFramerate(33);
	proj->Root().Append(new CFolderLayer("Folder A"));
	proj->Root().Append(new CFolderLayer("Folder B"));
	((CFolderLayer*)proj->Root().Layers().front())->Append(new CFolderLayer("Folder inafolder"));
	server->Listen();

	return a.exec();
}

void Close() {
	server->Close();
}