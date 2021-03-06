/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/23/2019 11:15:54 PM
 */
#include "CSocketMgr.h"

CSocketMgr::CSocketMgr(QTcpSocket* Socket, QObject* Parent) : QObject(Parent), m_sock(Socket)
{
	if (!m_sock)
		m_sock = new QTcpSocket(this);

	m_sock->connect(m_sock, &QTcpSocket::connected, this, &CSocketMgr::Connected);
	m_sock->connect(m_sock, &QTcpSocket::disconnected, this, &CSocketMgr::Disconnected);
	m_sock->connect(m_sock, &QTcpSocket::readyRead, this, &CSocketMgr::Incoming);
	m_sock->connect(m_sock, QOverload<QTcpSocket::SocketError>::of(&QTcpSocket::error), this, &CSocketMgr::Error);
}

void CSocketMgr::Incoming()
{
	qint64 unread = m_sock->bytesAvailable();

	// Enough bytes to complete the next msg or read new msg length
	while (m_sock->state() == QTcpSocket::ConnectedState &&
		((!m_nextmsg && unread >= sizeof(m_nextmsg)) || (m_nextmsg && unread >= m_nextmsg)))
	{
		if (!m_nextmsg)
		{
			m_buffer = m_sock->read(sizeof(m_nextmsg));
			m_nextmsg = ((CNetMsg*)m_buffer.data())->Length() - m_buffer.size();
		}
		if (m_sock->bytesAvailable() >= m_nextmsg)
		{
			m_buffer.append(m_sock->read(m_nextmsg));
			if (CNetMsg* msg = CNetMsg::FromData(m_buffer))
				HandleMsg(msg);
			m_nextmsg = 0;
		}
		unread = m_sock->bytesAvailable();
	}
}