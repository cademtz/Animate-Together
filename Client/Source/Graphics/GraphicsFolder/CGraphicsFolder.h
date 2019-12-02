/*	Description:
 *		Provides a graphics object representation of a folder layer
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:42:55 PM
 */

#ifndef CGraphicsFolder_H
#define CGraphicsFolder_H
#ifdef _WIN32
#pragma once
#endif

#include <NetObjects/CNetObject.h>
#include "Graphics/GraphicsLayer/CGraphicsLayer.h"

class CFolderLayer;
class CBaseLayerMsg;

class CGraphicsFolder : public QGraphicsWidget
{
public:
	CGraphicsFolder(CFolderLayer* Folder);
	~CGraphicsFolder();

	inline CFolderLayer* Folder() const { return m_folder; }
	int	type() const override { return (int)e_graphicstype::FolderLayer; }

private:
	void InsertLayer(const CNetObject& Layer);
	void RemoveLayer(const CBaseLayer* Layer);

	void OnLayerEvent(CBaseLayerMsg* Event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	CFolderLayer* m_folder;
	CGraphicsLayer* m_item;
	QGraphicsWidget* m_layerlist;
	QGraphicsLinearLayout* m_layout, * m_listlayout;
	bool m_open = true;
	unsigned m_listener;
};

#endif // CGraphicsFolder_H