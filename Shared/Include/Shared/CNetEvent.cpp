/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/20/2019 10:58:41 PM
 */

#include "CNetEvent.h"
#include "CSharedProject.h"
#include "NetObjects/CRasterFrame.h"
#include "NetObjects/CUser.h"

void CNetEvent::SetUser(CUser * User) {
	m_user = User->Handle();
}

CBaseLayerMsg::CBaseLayerMsg(EEvent EventType, CBaseLayer * Layer)
	: CNetEvent(EventType, Layer->RootProject()), m_layer(Layer) { }

CLayerAddMsg::CLayerAddMsg(CSharedProject* Proj, SerialStream& Stream) : CBaseLayerMsg(Proj, Stream)
{
	Stream >> m_add;
	if (m_add) // Get new layer's info and create it
	{
		// TO DO: Potential size error in serializing / deserializing a 'CSerialize' type.
		// funny. Fix that now.

		uint8_t type;
		CSerialize data;
		Stream >> type >> m_index >> data;

		switch (type)
		{
		case CBaseLayer::Layer_Folder:
			SetLayer(new CFolderLayer(Proj, data.Stream()));
			break;
		}
	}
	else
	{
		unsigned handle;
		Stream >> handle;

		SetLayer(Project()->FindLayer(handle));
		m_index = Layer()->Index();
	}

	m_parent = Layer()->Parent();

	// Set to opposite of 'undone' because the client has not yet performed the event
	SetUndone(!Undone());
}

CSerialize CLayerAddMsg::Serialize() const
{
	CSerialize data = CBaseLayerMsg::Serialize().Add(m_add);//(Type(), EventType(), m_add, Undone());
	if (m_add) // Send new layer's info
		data.Add((uint8_t)Layer()->Type(), Layer()->Index(), Layer()->Serialize());
	else
		data.Add(Layer()->Handle());
	return data;
}

void CLayerAddMsg::_Flip(bool Revert)
{
	if (WasAdded())
		m_parent->Insert(m_index, Layer());
	else
		m_parent->Remove(Layer());
}

CLayerEditMsg::CLayerEditMsg(CBaseLayer * Layer)
	: CBaseLayerMsg(Event_LayerEdit, Layer), m_oldindex(Layer->Index()), m_oldname(Layer->Name()), m_oldowner(Layer->Parent()->Handle()) { }

//CLayerEditMsg::CLayerEditMsg(CSharedProject * Proj, CNetMsg * Msg) : CBaseLayerMsg(Event_LayerEdit, Proj)
CLayerEditMsg::CLayerEditMsg(CSharedProject * Proj, SerialStream& Stream) : CBaseLayerMsg(Proj, Stream)
{
	uint8_t type, eventtype;
	bool undone;
	unsigned layer;

	//SerialStream s = CSerialize::Stream(Msg->Data());
	//s >> eventtype >> undone >> layer >> m_edits;
	Stream >> layer >> m_edits;
	SetLayer(Proj->FindLayer(layer));
	SetUndone(!undone);

	m_oldindex = Layer()->Index(), m_oldname = Layer()->Name(), m_oldowner = Layer()->Parent()->Handle(), m_oldparent = Layer()->Parent();
	if (m_edits & Edit_Name)
		Stream >> m_name;
	if (m_edits & Edit_Owner)
		Stream >> m_owner;
	if (m_edits & Edit_Place)
	{
		unsigned parent;
		Stream >> m_index >> parent;
		m_parent = Proj->FindLayer<CFolderLayer>(parent, CBaseLayer::Layer_Folder);
	}
}

CSerialize CLayerEditMsg::Serialize() const
{
	CSerialize data = CBaseLayerMsg::Serialize().Add(Layer()->Handle(), m_edits);
	if (m_edits & Edit_Name)
		data.Add(m_name);
	if (m_edits & Edit_Owner)
		data.Add(m_owner);
	if (m_edits & Edit_Place)
		data.Add(m_index, m_parent->Handle());
	return data;
}

void CLayerEditMsg::_Flip(bool Revert)
{
	if (m_edits & Edit_Name)
		Layer()->SetName(Revert ? m_oldname : m_name);
	if (m_edits & Edit_Owner)
		Layer()->SetOwner(Revert ? m_oldowner : m_owner);
	if (m_edits & Edit_Place)
	{
		Layer()->Parent()->Remove(Layer());
		(Revert ? m_oldparent : m_parent)->Insert(m_index, Layer());
	}
}

CBaseFrameMsg::CBaseFrameMsg(EEvent EventType, CBaseFrame * Frame)
	: CNetEvent(EventType, Frame->Parent()->RootProject()), m_frame(Frame) { }

CFrameAddMsg::CFrameAddMsg(CSharedProject * Proj, CNetMsg * Msg) : CBaseFrameMsg(Event_FrameAdd, Proj)
{
	SerialStream s = CSerialize::Stream(Msg->Data());
	uint8_t eventtype;
	bool undone;

	s >> eventtype >> m_add >> undone;

	if (m_add) // Get new layer's info and create it
	{
		// TO DO: Potential size error in serializing / deserializing a 'CSerialize' type.
		// funny. Fix that now.

		uint8_t type;
		CSerialize data;
		s >> type >> m_index >> data;

		switch (type)
		{
		case CBaseFrame::Frame_Raster:
			SetFrame(new CRasterFrame(Proj, data.Stream()));
			break;
		}
	}
	else
	{
		unsigned parent, handle;
		s >> parent >> handle;

		SetFrame(Project()->FindLayer(parent)->FindFrame(handle));
		m_index = Frame()->Index();
	}

	m_parent = Frame()->Parent();

	// Set to opposite of 'undone' because the client has not yet performed the event
	SetUndone(!undone);
}

CSerialize CFrameAddMsg::Serialize() const
{
	CSerialize data(Type(), EventType(), m_add, Undone());
	if (m_add) // Send new frame's info
		data.Add((uint8_t)Frame()->Type(), Frame()->Index(), Frame()->Serialize());
	else
		data.Add(Frame()->Parent()->Handle(), Frame()->Handle());
	return data;
}

void CFrameAddMsg::_Flip(bool Revert)
{
	if (WasAdded())
		m_parent->InsertFrame(m_index, Frame());
	else
		m_parent->RemoveFrame(Frame());
}
