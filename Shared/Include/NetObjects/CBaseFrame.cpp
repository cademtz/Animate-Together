/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 12/14/2019 11:46:53 PM
 */

#include "CBaseFrame.h"
#include "CBaseLayer.h"

int CBaseFrame::Index() const {
	return m_parent->IndexOf(this);
}

void CBaseFrame::SerializeCustom(CSerialize & Data) const {
	Data.Add(m_parent->Handle(), m_isKey, IsEmpty());
}

CBaseFrame * CBaseFrame::FindKey()
{
	return m_isKey ? this : m_parent->LastKey(Index());
}
