/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/22/2019 9:48:42 PM
 */

#include "CClientSocket.h"
#include <Shared/CNetMsg.h>
#include <qtcpserver.h>
#include <qdebug.h>

CClientSocket::CClientSocket(QTcpSocket * Socket): m_sock(Socket) {
	m_sock->connect(m_sock, &QTcpSocket::readyRead, [this] { Incoming(); });
}

void CClientSocket::Incoming()
{
	qInfo() << m_sock->peerAddress() << "Sent data (" << m_sock->bytesAvailable() << " bytes)";
	qint64 unread = m_sock->bytesAvailable();

	// Enough bytes to complete the next msg or read new msg length
	while ((!m_nextmsg && unread >= sizeof(m_nextmsg)) || (m_nextmsg && unread >= m_nextmsg))
	{
		if (!m_nextmsg)
		{
			m_buffer = m_sock->read(sizeof(m_nextmsg));
			CNetMsg* msg = CNetMsg::FromData(m_buffer);
			if (msg)
				m_nextmsg = msg->Length() - m_buffer.size();
		}
		if (m_sock->bytesAvailable() >= m_nextmsg)
		{
			m_buffer.append(m_sock->read(m_nextmsg));
			if (CNetMsg* msg = CNetMsg::FromData(m_buffer))
				HandleMsg(msg);
			m_nextmsg = 0;
		}
	}
}

void CClientSocket::HandleMsg(CNetMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Protocol:
	{
		CProtocolMsg proto(Msg);
		qInfo() << m_sock->peerAddress() << "Sent protocol: ("
			<< proto.Prefix() << ", " <<  proto.Major() << ", " << proto.Minor() << ')';
		break;
	}
	}
}
