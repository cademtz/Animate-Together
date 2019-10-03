/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 10:37:44 PM
 */

#include "CFrame.h"
#include "Projects/Layer/CLayer.h"

CFrame::Listeners_t CFrame::m_listeners;

CFrame::CFrame(CLayer * Layer, e_type Type, bool Hold)
	: m_layer(Layer), m_type(Type), m_key(!Hold) {
}

CFrame * CFrame::_Parent() {
	return Layer()->LastKey(Index());
}

CProject * CFrame::Project() {
	return Layer()->Project();
}
void CFrame::Select(bool Selected) {
	Layer()->SelectFrame(this);
}
bool CFrame::IsSelected() {
	return Layer()->IsFrameSelected(this);
}

size_t CFrame::Index() {
	if (Layer())
		return Layer()->IndexOf(this);
	return UINT_MAX;
}
