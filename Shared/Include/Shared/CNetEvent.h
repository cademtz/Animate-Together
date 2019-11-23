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
class CSharedProject;

class CNetEvent : public CBaseMsg
{
public:
	enum EEvent : uint8_t
	{
		Event_SharedProj = 0,
		Event_LayerAdd,
	};

	// - Performs the event. Every call toggles between revert and redo
	inline void Perform()
	{
		_Flip(m_revert);
		m_revert = !m_revert;
	}

	// - Returns true if the action was reverted or not performed
	inline bool Undone() const { return !m_revert; }
	inline CSharedProject* Project() const { return m_proj; }

protected:
	CNetEvent(EEvent EventType, CSharedProject* Proj)
		: CBaseMsg(Msg_Event), m_eventtype(EventType), m_proj(Proj) { }

	// - When called, the action must be performed or reverted
	// - 'Revert' determines whether or not to revert the action
	virtual void _Flip(bool Revert) = 0;
	void SetUndone(bool Undone) { m_revert = !Undone; }

private:
	EEvent m_eventtype;
	bool m_revert = false;
	CSharedProject* m_proj;
};

class CSharedProjectMsg : public CNetEvent
{
public:
	CSharedProjectMsg(CSharedProject* Proj) : CNetEvent(Event_SharedProj, Proj) { }

protected:
	void _Flip(bool Revert) override { }
	const CSerialize Serialize() const override { }
};

class CBaseLayerMsg : public CNetEvent
{
public:
	CBaseLayerMsg(EEvent EventType, CBaseLayer* Layer);
	CBaseLayerMsg(EEvent EventType, CSharedProject* Proj)
		: CNetEvent(EventType, Proj), m_layer(nullptr) { }

	template<class T = CBaseLayer>
	inline T* Layer() const { return (T*)m_layer; }

protected:
	inline void SetLayer(CBaseLayer* Layer) { m_layer = Layer; }

private:
	CBaseLayer* m_layer;
};

class CLayerAddMsg : public CBaseLayerMsg
{
public:
	CLayerAddMsg(CBaseLayer* Layer) : CBaseLayerMsg(Event_LayerAdd, Layer) { }
	CLayerAddMsg(CSharedProject* Proj, CNetMsg* Msg);
	~CLayerAddMsg()
	{
		if (Undone())
			delete Layer();
	}

protected:
	const CSerialize Serialize() const override;
	void _Flip(bool Revert) override;

private:
	CFolderLayer* m_parent;
	int m_index = 0;
};

#endif // CNetEvent_H