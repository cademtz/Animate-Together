/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/20/2019 10:58:41 PM
 */

#include "CNetEvent.h"
#include "NetObjects/CBaseLayer.h"

CBaseLayerMsg::CBaseLayerMsg(EEvent EventType, CBaseLayer * Layer)
	: CNetEvent(EventType, Layer->RootProject()), m_layer(Layer) { }

CLayerAddMsg::CLayerAddMsg(CSharedProject* Proj, CNetMsg * Msg) : CBaseLayerMsg(Event_LayerAdd, Proj)
{
	CNetObject obj = 0;
	bool undone;
	SerialStream s = CSerialize::Stream(Msg->Data());
	s >> obj >> undone;
	if (!undone) // Get new layer's info and add
	{
		QString name;
		CBaseLayer::EType type;
		s >> name >> type;
	}
}

const CSerialize CLayerAddMsg::Serialize() const
{
	CSerialize data(*Layer(), Undone());
	if (!Undone()) // Send new layer's info
		data.Add(Layer()->Name().utf16(), Layer()->Type());
	return data;
}

void CLayerAddMsg::_Flip(bool Revert)
{

}