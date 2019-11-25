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
	CNetObject obj = 0, parent = 0;
	uint8_t eventtype;
	bool undone;

	s >> eventtype >> obj >> parent >> undone;
	m_parent = Proj->FindLayer<CFolderLayer>(parent);

	if (undone)
	{
		SetLayer(Project()->FindLayer(obj));
		m_index = Layer()->Index();
	}
	else // Get new layer's info and create it
	{
		QString name;
		uint8_t type;
		s >> m_index >> name >> type;
		switch (type)
		{
		case CBaseLayer::Layer_Folder:
			SetLayer(new CFolderLayer(name)); break;
		}
	}

	// Set to opposite of 'undone' because the client has not yet performed the event
	SetUndone(!undone);
}

const CSerialize CLayerAddMsg::Serialize() const
{
	CSerialize data(Type(), Layer()->Handle(), Layer()->Parent()->Handle(), Undone());
	if (!Undone()) // Send new layer's info
		data.Add(Layer()->Index(), Layer()->Name().utf16(), (uint8_t)Layer()->Type());
	return data;
}

void CLayerAddMsg::_Flip(bool Revert)
{
	if (Revert)
		Project()->Root().Remove(Layer());
	else
		m_parent->Insert(m_index, Layer());
}
