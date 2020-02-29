/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/7/2019 10:39:53 AM
 */

#include "CFolderLayer.h"
#include "Shared/CSharedProject.h"

 LayerList_t CFolderLayer::Layers1D()
{
	LayerList_t layers;
	AppendLayers(this, layers);
	return layers;
}

ConstLayerList_t CFolderLayer::Layers1D() const
{
	ConstLayerList_t layers;
	AppendConstLayers(this, layers);
	return layers;
}

int CFolderLayer::IndexOf(const CNetObject & Obj) const
{
	if (Obj.Handle() == Handle())
		return -1;

	for (int i = 0; i < m_layers.size(); i++)
		if (m_layers[i]->Handle() == Obj.Handle())
			return i;
	return -1;
}

bool CFolderLayer::Move(int Index, CBaseLayer * Layer)
{
	if (m_layers.removeOne(Layer) && Index <= m_layers.size())
	{
		m_layers.insert(Index, Layer);
		return true;
	}
	return false;
}

void CFolderLayer::Insert(int Index, CBaseLayer * Layer)
{
	Layer->SetParent(this);
	m_layers.insert(Index, Layer);
	CreateEvent(CLayerAddMsg(Layer, IndexOf(Layer), this, true));
}

bool CFolderLayer::Remove(CBaseLayer * Layer)
{
	int index = IndexOf(Layer);
	bool removed = false;
	if (m_layers.removeOne(Layer))
		removed = true;
	if (!removed)
	{
		for (CBaseLayer* layer : m_layers)
		{
			if (layer->Type() != CBaseLayer::Layer_Folder)
				continue;
			CFolderLayer* folder = (CFolderLayer*)layer;
			if (folder->Remove(Layer))
			{
				removed = true;
				break;
			}
		}
	}

	if (removed)
		CreateEvent(CLayerAddMsg(Layer, index, this, false));
	return removed;
}

bool CFolderLayer::_Contains(const CNetObject & Obj) const
{
	if (Obj.Handle() == this->Handle())
		return true;
	for (CBaseLayer* layer : m_layers)
	{
		if (layer->Handle() == Obj.Handle())
			return layer;
		if (layer->Type() == CBaseLayer::Layer_Folder)
		{
			CFolderLayer* folder = (CFolderLayer*)layer;
			if (folder->_Contains(Obj))
				return true;
		}
	}
	return false;
}

CBaseLayer * CFolderLayer::_FindLayer(const CNetObject & Obj, EType Type)
{
	if (Handle() == Obj.Handle())
		return this;

	for (CBaseLayer* layer : m_layers)
	{
		if (Type != Layer_Null && Type != layer->Type())
			continue;
		if (layer->Handle() == Obj.Handle())
			return layer;
		if (layer->Type() == Layer_Folder)
		{
			CFolderLayer* folder = (CFolderLayer*)layer;
			if (CBaseLayer* found = folder->_FindLayer(Obj, Type))
				return found;
		}
	}
	return nullptr;
}

void CFolderLayer::AppendLayers(CBaseLayer * Layer, LayerList_t & List)
{
	if (!Layer)
		return;

	if (!Layer->IsRoot()) // Root layers should never be listed
		List.append(Layer);
	if (Layer->Type() == CBaseLayer::Layer_Folder)
		for (auto layer : ((CFolderLayer*)Layer)->Layers())
			AppendLayers(layer, List);
}

void CFolderLayer::AppendConstLayers(const CBaseLayer * Layer, ConstLayerList_t & List)
{
	if (!Layer)
		return;

	if (!Layer->IsRoot()) // Root layers should never be listed
		List.append(Layer);
	if (Layer->Type() == CBaseLayer::Layer_Folder)
		for (auto layer : ((CFolderLayer*)Layer)->Layers())
			AppendConstLayers(layer, List);
}
