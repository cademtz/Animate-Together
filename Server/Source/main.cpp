#include <QtCore/QCoreApplication>
#include "Server/CServer.h"
#include "NetObjects/CRasterFrame.h"

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

	CFolderLayer* fla = new CFolderLayer("Folder A"), * flb = new CFolderLayer("Folder B"), * flfl = new CFolderLayer("Folder inafolder");
	flb->Append(flfl);

	proj->Root().Append(fla);
	proj->Root().Append(flb);

	for (int i = 0; i < 10; i++)
		flfl->AppendFrame(new CRasterFrame((i % 5) == 0));

	server->SetProject(proj);
	server->Listen();

	return a.exec();
}

void Close() {
	server->Close();
}