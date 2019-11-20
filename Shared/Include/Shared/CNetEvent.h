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

class CBaseLayer;

class CNetEvent : public CBaseMsg
{
public:
	enum EEvent : uint8_t
	{
		Event_SharedProj = 0,
		Event_BaseLayer,
	};

	// - Performs the event. Every call toggles between revert and redo
	inline void Perform()
	{
		_Flip(m_revert);
		m_revert = !m_revert;
	}

	// - Returns true if the action was reverted or not performed
	inline bool Undone() const { return !m_revert; }

protected:
	CNetEvent(EEvent EventType) : CBaseMsg(Msg_Event), m_eventtype(EventType) { }

	// - When called, the action must be performed or reverted
	// - 'Revert' determines whether or not to revert the action
	virtual void _Flip(bool Revert) = 0;

private:
	EEvent m_eventtype;
	bool m_revert = false;
};

class CSharedProjectMsg : public CNetEvent
{
public:
	CSharedProjectMsg() : CNetEvent(Event_SharedProj) { }

protected:
	void _Flip(bool Revert) override { }
	const CSerialize Serialize() const override { }
};

class CBaseLayerMsg : public CNetEvent
{
public:
	CBaseLayerMsg() : CNetEvent(Event_BaseLayer) { }

protected:
	template<class T = CBaseLayer>
	inline T* _Layer() const { return (T*)m_layer; }

private:
	CBaseLayer* m_layer;
};

class CNewLayerMsg : public CBaseLayerMsg
{
public:
	
	/*
	
	Receives and sends basic layer data (name, handle, type)
	Structural issue: Cannot perform the action if given a handle with no way to use it
	How I'm planning on changing that? No idea...
	
	*/
	
protected:
	void _Flip(bool Revert) override { }

private:
	unsigned m_handle;
};

#endif // CNetEvent_H