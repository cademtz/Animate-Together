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

bool CFolderLayer::Remove(CBaseLayer * Layer)
{
	if (m_layers.removeOne(Layer))
		return true;
	for (CBaseLayer* layer : m_layers)
	{
		if (layer->Type() != CBaseLayer::Layer_Folder)
			continue;
		CFolderLayer* folder = (CFolderLayer*)layer;
		if (folder->Remove(Layer))
			return true;
	}
	return false;
}

CBaseLayer * CFolderLayer::_FindLayer(const CNetObject & Obj)
{
	if (Handle() == Obj.Handle())
		return this;

	for (CBaseLayer* layer : m_layers)
	{
		if (layer->Handle() == Obj.Handle())
			return layer;
		if (layer->Type() == CBaseLayer::Layer_Folder)
		{
			CFolderLayer* folder = (CFolderLayer*)layer;
			if (CBaseLayer* found = folder->_FindLayer(Obj))
				return found;
		}
	}
	return nullptr;
}
