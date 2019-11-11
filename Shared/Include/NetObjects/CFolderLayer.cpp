/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/7/2019 10:39:53 AM
 */

#include "CFolderLayer.h"

void CFolderLayer::_InsertLayer(int Index, CBaseLayer* Layer)
{
	m_layers.insert(Index, Layer);
	
}
