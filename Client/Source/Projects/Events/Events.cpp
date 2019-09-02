/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/15/2019 8:31:45 PM
 */

#include "Events.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Palette/CPalette.h"

CLayerEvent::CLayerEvent(CLayer * Layer, e_action Action, size_t Index) :
	CCustomEvent(e_type::LayerEvent), m_layer(Layer), m_action(Action), m_oldindex(Index)
{
	if (m_oldindex == UINT_MAX && Layer)
		m_oldindex = Layer->Index();
}

 CProject * CLayerEvent::Project() const {
	return m_layer->Project();
}

CProject * CPaletteEvent::Project() const {
	return m_palette->Project();
}
