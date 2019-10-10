/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 8:09:14 PM
 */

#include "CFrameLayout.h"
#include "Graphics/LayerLayout/CLayerLayout.h"

CFrameLayout::CFrameLayout(CLayerLayout * Parent)
	: QGraphicsLinearLayout(Qt::Horizontal, Parent)
{
	setSpacing(0);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

int CFrameLayout::IndexOf(CGraphicsFrame * Frame)
{
	for (int i = 0; i < count(); i++)
		if (itemAt(i) == Frame)
			return i;
	return -1;
}

int CFrameLayout::Index() {
	return Parent()->IndexOf(this);
}

