/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/15/2019 8:31:45 PM
 */

#include "Events.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Palette/CPalette.h"

CProject * CLayerEvent::Project() const {
	return m_layer->Project();
}

CProject * CPaletteEvent::Project() const {
	return m_palette->Project();
}
