/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 10:37:44 PM
 */

#include "CFrame.h"
#include "Projects/Layer/CLayer.h"

size_t CFrame::Index() const {
	return Layer()->IndexOf(this);
}
