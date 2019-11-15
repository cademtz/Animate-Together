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
#include <Shared/CNetMsg.h>

class CClientSocket;

class CServer : public QTcpServer
{
public:
	CServer(int argc, char *argv[]);

	bool Listen();
	void Close();

	inline bool IsListening() const { return this->isListening(); }
	inline const QString& Pass() const { return m_pass; }
	inline CLoginMsg Auth() const { return CLoginMsg(m_pass.isEmpty() ? 0 : CLoginMsg::Flag_Pass); }
	inline const CWelcomeMsg& Motd() const { return m_motd; }
	inline const QList<CClientSocket*>& Clients() const { return m_clients; }

	// - Sends a message to all joined users, unless specified otherwise
	void SendAll(const CBaseMsg& Msg, bool JoinedOnly = true);

protected:
	friend CClientSocket;
	void ClientConnect();
	void ClientDisconnect();

	// - Returns a unique-per-session User ID very time. Always non-zero
	inline unsigned NewUUID() { return ++m_lastuuid; }

private:
	uint16_t m_port;
	QString m_pass;
	CWelcomeMsg m_motd;
	unsigned m_lastuuid = 0;

	QList<CClientSocket*> m_clients;

	CClientSocket* GetClient(QTcpSocket* Socket);
};

#endif // CServer_H