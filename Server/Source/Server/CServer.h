/*	Description:
 *		Provides a class to manage a TCP server
 *
 *	Author: Hold on!
 *	Created: 10/18/2019 5:48:53 PM
 */

#ifndef CServer_H
#define CServer_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpserver.h>
#include <qlist.h>
#include "ClientSocket\CClientSocket.h"

class CServer : public QTcpServer
{
public:
	CServer(int argc, char *argv[]);

	bool Listen();
	void Close();
	inline bool IsListening() const { return this->isListening(); }

protected:
	void ClientConnect();
	void ClientDisconnect();

private:
	uint16_t m_port;
	QList<CClientSocket> m_clients;

	CClientSocket* GetClient(QTcpSocket* Socket);
};

#endif // CServer_H