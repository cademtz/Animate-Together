/*	Description:
 *		Provides an abstract class to send and receive data over a TCP socket using Animate Together's protocol
 *
 *	Author: Hold on!
 *	Created: 10/23/2019 11:12:29 PM
 */

#ifndef CSocketMgr_H
#define CSocketMgr_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>
#include "CNetMsg.h"

class CSocketMgr : public QObject
{
	Q_OBJECT

public:
	CSocketMgr(QTcpSocket* Socket = Q_NULLPTR, QObject* Parent = Q_NULLPTR);
	virtual ~CSocketMgr() { }

	inline QTcpSocket* Socket() { return m_sock; }
	inline void SendMsg(const CBaseMsg& Msg) { Msg.Send(m_sock); }
	inline void SendData(const CSerialize& Data) {
		CNetMsg::FromData(Data.Bytes())->Send(m_sock);
	}

protected:
	virtual void HandleMsg(CNetMsg* Msg) = 0;
	virtual void Connected() { }
	virtual void Disconnected() { }
	virtual void Error(QAbstractSocket::SocketError Error) { qInfo() << Error; }

private:
	QTcpSocket* m_sock;
	QByteArray m_buffer;
	unsigned m_nextmsg = 0;

	void Incoming();
};

#endif // CSocketMgr_H