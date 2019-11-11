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

	CFolderLayer(CFolderLayer* Parent) : CBaseLayer(CBaseLayer::Layer_Folder, Parent) { }
	CFolderLayer(CSharedProject* Parent) : CBaseLayer(CBaseLayer::Layer_Folder, 0), m_root(Parent) { }

	inline const LayerList_t& Layers() const { return m_layers; }

protected:
	friend CBaseLayer;
	friend CSharedProject;

	inline void SetRootProject(CSharedProject* Project)
	{
		if (Parent())
			return Parent()->SetRootProject(Project);
		m_root = Project;
	}

	// - Returns the instance's project if it is a root folder, otherwise return is null
	// - Only used when a layer class must access the property
	inline CSharedProject* _Project() const { return m_root; }
	
private:
	CSharedProject* m_root = 0;
	LayerList_t m_layers;

	void _InsertLayer(int Index, CBaseLayer* Layer);
};

#endif // CFolderLayer_H