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
	typedef QList<CBaseLayer*> LayerList_t;

	CFolderLayer(CSharedProject* Proj) : CBaseLayer(Layer_Folder), m_proj(Proj) { }
	CFolderLayer(QString Name = "New Folder") : CBaseLayer(Layer_Folder) {
		SetName(Name);
	}

	// - Finds a child or the current layer by handle
	// - Return is null if the layer is not listed
	template<typename T = CBaseLayer>
	inline T* FindLayer(const CNetObject& Obj) { return (T*)_FindLayer(Obj); }

	// - Finds the index of an immediate descendant layer
	// - Return is negative if the layer is not listed
	int IndexOf(const CNetObject& Obj);
	inline int IndexOf(const CBaseLayer* Layer) { return IndexOf(Layer->Handle()); }
	inline const LayerList_t& Layers() const { return m_layers; }

	// TO DO: Events n stuff for these functions (kinda why the list isn't simply public)

	void Append(CBaseLayer* Layer);
	void Insert(int Index, CBaseLayer* Layer);

	// - Finds and removes a child layer by pointer
	// - Returns true if Layer is listed
	bool Remove(CBaseLayer* Layer);


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
	CBaseLayer* _FindLayer(const CNetObject& Obj);

	CSharedProject* m_proj = 0;
	LayerList_t m_layers;
};

#endif // CFolderLayer_H