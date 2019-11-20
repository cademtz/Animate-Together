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

	inline const LayerList_t& Layers() const { return m_layers; }

	// TO DO: Events n stuff for these functions (kinda why the list isn't simply public)

	void Append(CBaseLayer* Layer);
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
	CSharedProject* m_proj = 0;
	LayerList_t m_layers;
};

#endif // CFolderLayer_H