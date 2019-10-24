/*	Description:
 *		Provides functions for managing a connection to servers
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
#include <Shared/Protocol.h>
#include <Shared/CSocketMgr.h>

class CProject;
class CNetMsg;

class CClient : public CSocketMgr
{
public:
	static void Connect(QString Host);
	static void Close();

private:
	static inline CClient& Client()
	{
		static CClient client;
		return client;
	}

	CClient() : CSocketMgr(), m_stage(ATNet::ClosedStage) { }
	~CClient() { }

	ATNet::EProtoStage m_stage;
	CProject* m_proj;

	void Connected();
	void Disconnected();
	void HandleMsg(CNetMsg* Msg);
};

#endif // CClient_H