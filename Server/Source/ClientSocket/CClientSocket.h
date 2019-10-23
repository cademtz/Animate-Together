/*	Description:
 *		A class to pair sockets with client-specific information and handle net messages
 *
 *	Author: Hold on!
 *	Created: 10/22/2019 5:58:15 PM
 */

#ifndef CClientSocket_H
#define CClientSocket_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>

class CNetMsg;

class CClientSocket
{
public:
	CClientSocket(QTcpSocket* Socket);

	inline QTcpSocket* Socket() { return m_sock; }

private:
	QTcpSocket* m_sock;
	QByteArray m_buffer;
	unsigned m_nextmsg = 0;

	void Incoming();
	void HandleMsg(CNetMsg* Msg);
};

#endif // CClientSocket_H