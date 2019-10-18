/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "CClient.h"

CProject* CClient::m_proj;
QTcpSocket CClient::m_sock;

void CClient::Connect(QString Host)
{
	QStringList info = Host.split(':');
	if (info.size() < 2) // Use default if port specified
		info.push_back("65535"); // TODO: Shared source files for connections and defaults
	m_sock.connectToHost(info[0], info[1].toInt());
}

void CClient::Close()
{
	m_sock.close();
}
