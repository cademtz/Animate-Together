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
	CFolderLayer(CSharedProject* Proj, SerialStream& Data) : CBaseLayer(Layer_Folder, Proj, Data) { }
	CFolderLayer(QString Name = "New Folder") : CBaseLayer(Layer_Folder) {
		SetName(Name);
	}
	~CFolderLayer()
	{
		for (auto layer : m_layers)
			delete layer;
	}

	// - Creates a flattened list of all children
	// - In order of parent before children including current layer
	LayerList_t Layers1D();
	ConstLayerList_t Layers1D() const;

	// - Finds a child or the current layer by handle
	// - Return is null if the layer is not listed
	template<typename T = CBaseLayer>
	inline T* FindLayer(const CNetObject& Obj, EType Type = Layer_Null) { return (T*)_FindLayer(Obj, Type); }
	template<typename T = CBaseLayer>
	inline const T* FindLayer(const CNetObject& Obj, EType Type = Layer_Null) const { return (const T*)_FindLayer(Obj, Type); }
	inline bool Contains(const CNetObject& Obj) const { return _Contains(Obj); }
	inline bool Contains(const CBaseLayer* Layer) const { return _Contains(Layer->Handle()); }

	template<typename T = CBaseLayer>
	inline T* FindDirectChild(const CNetObject& Obj, EType Type = Layer_Null) { return _FindDirectChild(Obj, Type); }
	template<typename T = CBaseLayer>
	inline const T* FindDirectChild(const CNetObject& Obj, EType Type = Layer_Null) const { return _FindDirectChild(Obj, Type); }
	inline bool IsDirectChild(const CNetObject& Obj) const { return FindDirectChild(Obj); }
	inline bool IsDirectChild(const CBaseLayer* Layer) const { return Layer->Parent() == this; }

	// - Finds the index of an immediate descendant layer
	// - Return is negative if the layer is not listed
	int IndexOf(const CNetObject& Obj) const;
	inline int IndexOf(const CBaseLayer* Layer) const { return IndexOf(Layer->Handle()); }
	inline const LayerList_t& Layers() { return m_layers; }
	inline const ConstLayerList_t& Layers() const { return (ConstLayerList_t&)m_layers; }

	// TO DO: Events n stuff for these functions (kinda why the list isn't simply public)

	// - Returns true if 'Layer' is listed
	bool Remove(CBaseLayer* Layer);

	// - Returns true if 'Layer' is listed and operation succeeds
	bool Move(int Index, CBaseLayer* Layer);
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
	inline CSharedProject* Project() { return m_proj; }
	inline const CSharedProject* Project() const { return m_proj; }
	
private:
	bool _Contains(const CNetObject& Obj) const;
	CBaseLayer* _FindLayer(const CNetObject& Obj, EType Type);
	inline const CBaseLayer* _FindLayer(const CNetObject& Obj, EType Type) const { return _FindLayer(Obj, Type); }
	inline CBaseLayer* _FindDirectChild(const CNetObject& Obj, EType Type) const
	{
		for (auto layer : m_layers)
			if (layer->Handle() == Obj.Handle())
				return layer;
		return nullptr;
	}
	static void AppendLayers(CBaseLayer* Layer, LayerList_t& List);
	static void AppendConstLayers(const CBaseLayer* Layer, ConstLayerList_t& List);

	CSharedProject* m_proj = 0;
	LayerList_t m_layers;
};

#endif // CFolderLayer_H