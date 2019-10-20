/*	Description:
 *		Provides functions for managing a connection to servers
 *
 *	Author: Hold on!
 *	Created: 10/17/2019 11:16:47 PM
 */

#ifndef ATClient_H
#define ATClient_H
#ifdef _WIN32
#pragma once
#endif

#include <qtcpsocket.h>

class CProject;

namespace ATClient
{
	void Init();

	void Connect(QString Host);
	void Close();

	namespace Private
	{
		static CProject* _proj;
		static QTcpSocket* _sock;

		void Connected();
		void Disconnected();
		void Incoming();
		void Error(QTcpSocket::SocketError Error);
	}
};

#endif // ATClient_H