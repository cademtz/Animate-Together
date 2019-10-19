/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:38:42 PM
 */

#include "CClient.h"
#include <Shared/NetDefaults.h>

CProject* CClient::m_proj;
QTcpSocket CClient::m_sock;

void CClient::Connect(QString Host)
{
	QStringList info = Host.split(':');
	int port;
	if (info.size() < 2) // Use default if port specified
		port = AT_DEFPORT;
	else
		port = info[1].toInt();

	m_sock.connectToHost(info.front(), port);
}

void CClient::Close()
{
	m_sock.close();
}
