/*	Description:
 *		Provides a custom set of events specific to Animate Together.
 *		These events can be networked, listened for via certain classes, and undone if the sending instance allows it.
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 4:54:12 PM
 */

#ifndef CNetEvent_H
#define CNetEvent_H
#ifdef _WIN32
#pragma once
#endif

#include "CNetMsg.h"

class CNetEvent : public CBaseMsg
{
public:
	enum EEvent : uint8_t
	{
		Event_SharedProj = 0,
		Event_BaseLayer,
	};

protected:
	CNetEvent(EEvent EventType) : CBaseMsg(Msg_Event), m_eventtype(EventType) { }

private:
	EEvent m_eventtype;
};

class CSharedProjectMsg : public CNetEvent
{
public:
	CSharedProjectMsg() : CNetEvent(Event_SharedProj) { }

protected:
	const CSerialize Serialize() const override { }
};

class CBaseLayerMsg : public CNetEvent
{
public:
	CBaseLayerMsg() : CNetEvent(Event_BaseLayer) { }

protected:
	const CSerialize Serialize() const override { }
};

#endif // CNetEvent_H