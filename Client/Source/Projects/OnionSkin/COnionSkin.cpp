/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 9/27/2019 7:19:18 PM
 */

#include "COnionSkin.h"
#include "Projects/CProject.h"

bool COnionSkin::m_enabled = false;
int COnionSkin::m_fwd = 2, COnionSkin::m_back = 2;

void COnionSkin::CreateEvent() {
	CProject::CreateEvent(CProjectEvent(0, CProjectEvent::OnionSkin));
}

void COnionSkin::Enable(bool Yes)
{
	m_enabled = Yes;
	CreateEvent();
}

void COnionSkin::SetBack(int Back)
{
	m_back = Back;
	CreateEvent();
}

void COnionSkin::SetForward(int Fwd)
{
	m_fwd = Fwd;
	CreateEvent();
}
