/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 12/14/2019 11:46:53 PM
 */

#include "CBaseFrame.h"
#include "CBaseLayer.h"

void CBaseFrame::SerializeCustom(CSerialize & Data) const
{
	Data.Add(m_parent ? m_parent->Handle() : 0, IsEmpty());
}
