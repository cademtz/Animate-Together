/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/15/2019 8:31:45 PM
 */

#include "Events.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Palette/CPalette.h"
#include <Shared/CEventHandler.h>

template <typename TEventClass, typename TCallback>
std::list<std::function<TCallback>> CEventHandler<TEventClass, TCallback>::m_listeners;

CLayerEvent::CLayerEvent(CLayer * Layer, e_action Action, size_t Index) :
	CCustomEvent(e_type::LayerEvent), m_layer(Layer), m_action(Action), m_oldindex(Index)
{
	if (m_oldindex == UINT_MAX && Layer)
		m_oldindex = Layer->Index();
}

CProject * CLayerEvent::Project() {
	return m_layer->Project();
}

CProject * CPaletteEvent::Project() {
	return m_palette->Project();
}

CFrameEvent::CFrameEvent(CFrame * Frame, e_action Action, CFrame * Old, size_t OldIndex)
	: CCustomEvent(e_type::FrameEvent), m_frame(Frame), m_action(Action), m_old(Old), m_oldindex(OldIndex)
{
	if (m_oldindex == UINT_MAX && Frame)
		m_oldindex = Frame->Index();
}