/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/7/2019 10:39:53 AM
 */

#include "CFolderLayer.h"

void CFolderLayer::Append(CBaseLayer * Layer)
{
	Layer->SetParent(this);
	m_layers.append(Layer);
}

void CFolderLayer::Insert(int Index, CBaseLayer * Layer)
{
	Layer->SetParent(this);
	m_layers.insert(Index, Layer);
}