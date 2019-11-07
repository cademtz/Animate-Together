/*	Description:
 *		Provides a custom set of networkable events specific to Animate Together
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 4:54:12 PM
 */

#ifndef Events_H
#define Events_H
#ifdef _WIN32
#pragma once
#endif

#include "CNetMsg.h"

class CProjectEventMsg : public CBaseMsg
{
protected:
	const CSerialize Serialize() const override { }
};

class CLayerEventMsg : public CBaseMsg
{
protected:
	const CSerialize Serialize() const override { }
};

#endif // Events_H