/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/24/2019 12:16:55 AM
 */

#include "Shared/Protocol.h"
#include "Shared/CNetMsg.h"
#include <cstring>

namespace ATNet
{
	bool GoodProtocol(const CProtocolMsg * Msg)
	{
		return !strcmp(Msg->Prefix(), AT_PROTO_PREFIX) &&
			Msg->Major() == AT_PROTO_MAJOR &&
			Msg->Minor() == AT_PROTO_MINOR;
	}
}
