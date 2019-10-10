#include "CLayerLayout.h"
/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 8:03:35 PM
 */

CLayerLayout::CLayerLayout(QGraphicsLayoutItem * Parent)
	: QGraphicsLinearLayout(Qt::Vertical, Parent)
{
	setSpacing(0);
	setContentsMargins(0, 0, 0, 0);
}

int CLayerLayout::IndexOf(CFrameLayout * Layer)
{
	for (int i = 0; i < count(); i++)
		if (itemAt(i) == Layer)
			return i;
	return -1;
}
