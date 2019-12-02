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
class CFolderLayer;
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
		_Flip(!m_undone);
		m_undone = !m_undone;
	}

	// - Returns true if the action was reverted or not performed
	inline bool Undone() const { return m_undone; }
	inline CSharedProject* Project() const { return m_proj; }
	inline uint8_t EventType() const { return m_eventtype; }

protected:
	CNetEvent(EEvent EventType, CSharedProject* Proj)
		: CBaseMsg(Msg_Event), m_eventtype(EventType), m_proj(Proj) { }

	// - When called, the action must be performed or reverted
	// - 'Revert' determines whether or not to revert the action
	virtual void _Flip(bool Revert) = 0;
	void SetUndone(bool Undone) { m_undone = Undone; }

private:
	uint8_t m_eventtype;
	bool m_undone = false;
	CSharedProject* m_proj;
};

class CNetEventInfo : public CBaseMsg
{
public:
	CNetEventInfo(const CNetMsg * Msg) : CBaseMsg(Msg_Event) {
		CSerialize::Deserialize(Msg->Data(), m_eventtype);
	}

	inline uint8_t EventType() const { return m_eventtype; }

private:
	const CSerialize Serialize() const override { return CSerialize(); }

	uint8_t m_eventtype;
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
	// - Creates an add or remove event dependind on 'Add'
	CLayerAddMsg(CBaseLayer * Layer, bool Add) : CBaseLayerMsg(Event_LayerAdd, Layer), m_add(Add) { }
	CLayerAddMsg(CSharedProject* Proj, CNetMsg* Msg);
	~CLayerAddMsg()
	{
		if (Undone())
			delete Layer();
	}

	// - Returns true if the event describes an added layer, false if it's a removed layer
	inline bool IsAdd() const { return m_add; }

	// - Returns true if the layer was last added or removed
	inline bool WasAdded() const { return m_add != Undone(); }

protected:
	const CSerialize Serialize() const override;
	void _Flip(bool Revert) override;

private:
	CFolderLayer* m_parent;
	int m_index = 0;
	bool m_add;
};

#endif // CNetEvent_H