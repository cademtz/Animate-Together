/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:06:49 PM
 */

#include "CSharedProject.h"

CSharedProject::Listeners_t CSharedProject::m_listeners;

CSharedProject::CSharedProject() : m_root(this) {
	CBaseLayer::Listen([this](CNetEvent* Event) { OnLayerEvent(Event); });
}

void CSharedProject::SelectLayer(CBaseLayer * Layer, bool Select)
{
	bool selected = IsSelected(Layer);
	if (Select == selected)
		return;

	if (!Select)
		m_selected.removeOne(Layer);
	else if (Contains(Layer)) // Only select if we own the layer
		m_selected.push_back(Layer);
}

void CSharedProject::AddEvent(CNetEvent * Event)
{
	Event->Perform();
	m_events.push_back(Event);
}

void CSharedProject::OnLayerEvent(CNetEvent* Event)
{
	m_events.push_back(Event);
}
