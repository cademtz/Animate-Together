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

class CBaseFrame;
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
		Event_LayerEdit,
		Event_FrameAdd,
	};

	// - Performs the event. Every call toggles between revert and redo
	inline void Perform()
	{
		m_undone = !m_undone;
		_Flip(m_undone);
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
	CSerialize Serialize() const override { return CSerialize(); }

	uint8_t m_eventtype;
};

class CSharedProjectMsg : public CNetEvent
{
public:
	CSharedProjectMsg(CSharedProject* Proj) : CNetEvent(Event_SharedProj, Proj) { }

protected:
	void _Flip(bool Revert) override { }
	CSerialize Serialize() const override { }
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
	// - Creates an add or remove event depending on 'Add'
	CLayerAddMsg(CBaseLayer * Layer, int Index, CFolderLayer* Parent, bool Add)
		: CBaseLayerMsg(Event_LayerAdd, Layer), m_add(Add), m_parent(Parent), m_index(Index) { }
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
	inline int Index() const { return m_index; }
	inline CFolderLayer* Parent() const { return m_parent; }

protected:
	CSerialize Serialize() const override;
	void _Flip(bool Revert) override;

private:
	CFolderLayer* m_parent;
	int m_index = 0;
	bool m_add;
};

class CLayerEditMsg : public CBaseLayerMsg
{
public:
	enum EEditFlags : uint8_t
	{
		Edit_Name = (1 << 0),
		Edit_Owner = (1 << 1),
		Edit_Place = (1 << 2), // - Layer was moved
	};
	CLayerEditMsg(CBaseLayer* Layer);
	CLayerEditMsg(CSharedProject* Proj, CNetMsg* Msg);

	inline EEditFlags Edited() const { return (EEditFlags)m_edits; }
	inline const QString& NewName() const { return m_name; }
	inline const QString& OldName() const { return m_oldname; }
	inline const unsigned NewOwner() const { return m_owner; }
	inline const unsigned OldOwner() const { return m_oldowner; }
	inline int NewIndex() const { return m_index; }
	inline int OldIndex() const { return m_oldindex; }
	inline CFolderLayer* NewParent() const { return m_parent; }
	inline CFolderLayer* OldParent() const { return m_oldparent; }
	void SetNewName(const QString& Name) { m_name = Name, m_edits |= Edit_Name; }
	void SetNewOwner(const unsigned Handle) { m_owner = Handle, m_edits |= Edit_Owner; }
	void SetNewPlace(int Index, CFolderLayer* Parent) { m_index = Index, m_parent = Parent, m_edits |= Edit_Place; }

protected:
	CSerialize Serialize() const override;
	void _Flip(bool Revert) override;

private:
	uint8_t m_edits = 0;
	QString m_name, m_oldname;
	unsigned m_owner, m_oldowner;
	CFolderLayer* m_parent, * m_oldparent;
	int m_index, m_oldindex;
};

/* TO DO: Make CBaseLayerMsg & CLayerAddMsg more abstract to be used for frames and strokes as well.
 * Currently these message classes are almost entirely copy-pasted for the frame events.
 */

class CBaseFrameMsg : public CNetEvent
{
public:
	CBaseFrameMsg(EEvent EventType, CBaseFrame* Frame);
	CBaseFrameMsg(EEvent EventType, CSharedProject* Proj)
		: CNetEvent(EventType, Proj), m_frame(nullptr) { }

	template<class T = CBaseFrame>
	inline T* Frame() const { return (T*)m_frame; }

protected:
	inline void SetFrame(CBaseFrame* Frame) { m_frame = Frame; }

private:
	CBaseFrame* m_frame;
};

class CFrameAddMsg : public CBaseFrameMsg
{
public:
	// - Creates an add or remove event depending on 'Add'
	CFrameAddMsg(CBaseFrame * Frame, bool Add) : CBaseFrameMsg(Event_FrameAdd, Frame), m_add(Add) { }
	CFrameAddMsg(CSharedProject* Proj, CNetMsg* Msg);
	~CFrameAddMsg()
	{
		if (Undone())
			delete Frame();
	}

	// - Returns true if the event describes an added frame, false if it's a removed frame
	inline bool IsAdd() const { return m_add; }

	// - Returns true if the frame was last added or removed
	inline bool WasAdded() const { return m_add != Undone(); }
	inline int Index() const { return m_index; }

protected:
	CSerialize Serialize() const override;
	void _Flip(bool Revert) override;

private:
	CBaseLayer* m_parent;
	int m_index = 0;
	bool m_add;
};

#endif // CNetEvent_H