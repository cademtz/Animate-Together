/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/19/2019 9:04:06 PM
 */

#include "CNetMsg.h"
#include <cstring>
#include <cassert>

CNetMsg * CNetMsg::FromData(unsigned length, const char* Data)
{
	if (length >= sizeof(m_len))
	{
		CNetMsg* msg = (CNetMsg*)Data;
		if (msg->Length() <= length)
			return msg;
	}
	return 0;
}

bool CProtocolMsg::Compatible()
{
	return Prefix() == AT_PROTO_PREFIX &&
		Major() == AT_PROTO_MAJOR &&
		Minor() == AT_PROTO_MINOR;
}

