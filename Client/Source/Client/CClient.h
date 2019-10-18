/*	Description:
 *		A class that manages the connection to a server's project
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:16:47 PM
 */

#ifndef CClient_H
#define CClient_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>

class CProject;

class CClient
{
public:
	static void Connect(QString Host);
	static void Close();

private:
	CClient() { }
	~CClient() { }

	static CProject* m_proj;
	static QTcpSocket m_sock;
};

#endif // CClient_H