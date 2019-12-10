/*	Description:
 *		A layer for holding and managing other layers
 *
 * 		Details:
 *			All layers stored in a folder layer are assumed to be owned by it.
 *			Destruction of a folder layer will destroy all its layers.
 *
 *			Root folders cannot exist in multiple projects at once.
 *			When adding or moving root folders to a project, the root project must be set as well.
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 6:08:39 PM
 */

#ifndef CFolderLayer_H
#define CFolderLayer_H
#ifdef _WIN32
#pragma once
#endif

#include <qlist.h>
#include "CBaseLayer.h"

class CSharedProject;

class CFolderLayer : public CBaseLayer
{
public:
	CFolderLayer(CSharedProject* Proj) : CBaseLayer(Layer_Folder), m_proj(Proj) { }
	CFolderLayer(QString Name = "New Folder") : CBaseLayer(Layer_Folder) {
		SetName(Name);
	}
	CFolderLayer(CSharedProject* Proj, const CSerialize& Data);
	~CFolderLayer()
	{
		for (auto layer : m_layers)
			delete layer;
	}

	// - Creates a flattened list of all children
	// - In order of parent before children including current layer
	LayerList_t Layers1D();

	// - Finds a child or the current layer by handle
	// - Return is null if the layer is not listed
	template<typename T = CBaseLayer>
	inline T* FindLayer(const CNetObject& Obj) { return (T*)_FindLayer(Obj); }
	inline bool Contains(const CNetObject& Obj) const { return _Contains(Obj); }
	inline bool Contains(const CBaseLayer* Layer) const { return _Contains(Layer->Handle()); }

	// - Returns true if the layer exists strictly in the folder's list
	inline bool IsDirectChild(const CNetObject& Obj) const
	{
		for (auto layer : m_layers)
			if (layer->Handle() == Obj.Handle())
				return true;
		return false;
	}
	inline bool IsDirectChild(const CBaseLayer* Layer) const { return IsDirectChild(Layer->Handle()); }

	// - Finds the index of an immediate descendant layer
	// - Return is negative if the layer is not listed
	int IndexOf(const CNetObject& Obj);
	inline int IndexOf(const CBaseLayer* Layer) { return IndexOf(Layer->Handle()); }
	inline const LayerList_t& Layers() const { return m_layers; }

	// TO DO: Events n stuff for these functions (kinda why the list isn't simply public)

	// - Finds and removes a child layer by pointer
	// - Returns true if Layer is listed
	bool Remove(CBaseLayer* Layer);
	inline void Append(CBaseLayer* Layer) { Insert(m_layers.size(), Layer); }
	void Insert(int Index, CBaseLayer* Layer);

protected:
	friend CBaseLayer;
	friend CSharedProject;

	inline void SetRootProject(CSharedProject* Project)
	{
		if (Parent())
			return Parent()->SetRootProject(Project);
		m_proj = Project;
	}

	// - Returns the instance's project if it is a root folder, otherwise return is null
	// - Only used when a layer class must access the property
	inline CSharedProject* _Project() const { return m_proj; }
	
private:
	bool _Contains(const CNetObject& Obj) const;
	CBaseLayer* _FindLayer(const CNetObject& Obj);
	static void AppendLayers(CBaseLayer* Layer, LayerList_t& List);

	CSharedProject* m_proj = 0;
	LayerList_t m_layers;
};

#endif // CFolderLayer_H