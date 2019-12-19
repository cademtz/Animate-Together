/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/20/2019 10:58:41 PM
 */

#include "CNetEvent.h"
#include "CSharedProject.h"

CBaseLayerMsg::CBaseLayerMsg(EEvent EventType, CBaseLayer * Layer)
	: CNetEvent(EventType, Layer->RootProject()), m_layer(Layer) { }

CLayerAddMsg::CLayerAddMsg(CSharedProject* Proj, CNetMsg * Msg) : CBaseLayerMsg(Event_LayerAdd, Proj)
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
		case CBaseLayer::Layer_Folder:
			SetLayer(new CFolderLayer(Proj, data.Stream()));
			break;
		}
	}
	else
	{
		unsigned handle;
		s >> handle;

		SetLayer(Project()->FindLayer(handle));
		m_index = Layer()->Index();
	}

	m_parent = Layer()->Parent();

	// Set to opposite of 'undone' because the client has not yet performed the event
	SetUndone(!undone);
}

CSerialize CLayerAddMsg::Serialize() const
{
	CSerialize data(Type(), EventType(), m_add, Undone());
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
		Project()->Root().Remove(Layer());
}

CLayerEditMsg::CLayerEditMsg(CSharedProject * Proj, CNetMsg * Msg) : CBaseLayerMsg(Event_LayerEdit, Proj)
{
	uint8_t type, eventtype;
	bool undone;
	unsigned layer;

	SerialStream s = CSerialize::Stream(Msg->Data());
	s >> type >> eventtype >> undone >> layer >> m_edits;

	SetLayer(Proj->FindLayer(layer));
	SetUndone(!undone);

	if (m_edits & Edit_Name)
		s >> m_name;
	if (m_edits & Edit_Owner)
	{
		unsigned owner;
		s >> owner;
		m_owner = owner;
	}
	if (m_edits & Edit_Index)
		s >> m_index;
}

CSerialize CLayerEditMsg::Serialize() const
{
	CSerialize data(Type(), EventType(), Undone(), Layer()->Handle(), m_edits);
	if (m_edits & Edit_Name)
		data.Add(m_name);
	if (m_edits & Edit_Owner)
		data.Add(m_owner.Handle());
	if (m_edits & Edit_Index)
		data.Add(m_index);
	return data;
}

void CLayerEditMsg::_Flip(bool Revert)
{
	// No need to check 'Revert' since data is simply swapped
	if (m_edits & Edit_Name)
	{
		QString name = Layer()->Name();
		Layer()->SetName(m_name);
		m_name = name;
	}
	if (m_edits & Edit_Owner)
	{
		CNetObject owner = Layer()->Owner();
		Layer()->SetOwner(m_owner);
		m_owner = owner;
	}
	if (m_edits & Edit_Index)
	{
		int index = Layer()->Index();
		Layer()->Parent()->Move(m_index, Layer());
		m_index = index;
	}
}
