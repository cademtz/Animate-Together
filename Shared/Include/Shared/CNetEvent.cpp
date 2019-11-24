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
	bool undone;

	s >> obj >> parent >> undone;
	SetUndone(undone);
	m_parent = Proj->FindLayer<CFolderLayer>(parent);

	if (Undone())
		SetLayer(Project()->FindLayer(obj));
	else // Get new layer's info and create it
	{
		QString name;
		CBaseLayer::EType type;
		s >> name >> type;
		switch (type)
		{
		case CBaseLayer::Layer_Folder:
			SetLayer(new CFolderLayer(name)); break;
		}
	}
	
	m_index = m_parent->Index();
}

const CSerialize CLayerAddMsg::Serialize() const
{
	CSerialize data(Layer()->Handle(), Layer()->Parent()->Handle(), Undone());
	if (!Undone()) // Send new layer's info
		data.Add(Layer()->Name().utf16(), Layer()->Type());
	return data;
}

void CLayerAddMsg::_Flip(bool Revert)
{
	if (Revert)
		Project()->Root().Remove(Layer());
	else
		m_parent->Insert(m_index, Layer());
}