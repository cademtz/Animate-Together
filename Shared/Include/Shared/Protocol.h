/*	Description:
 *		Defines shared properties for the Animate Together protocol
 *
 *	Author: Hold on!
 *	Created: 10/24/2019 12:03:23 AM
 */

#ifndef Protocol_H
#define Protocol_H
#ifdef _WIN32
#pragma once
#endif

#include "Config.h"

class CProtocolMsg;

namespace ATNet
{
	enum EProtoStage
	{
		ClosedStage = 0,
		ProtocolStage,
		JoinStage,
		FinalStage
	};
}

#endif // Protocol_H