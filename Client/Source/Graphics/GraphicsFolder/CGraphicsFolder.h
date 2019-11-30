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

#include "Graphics/GraphicsLayer/CGraphicsLayer.h"

class CFolderLayer;
class CBaseLayerMsg;

class CGraphicsFolder : public QGraphicsWidget
{
public:
	CGraphicsFolder(CFolderLayer* Folder);
	int	type() const override { return (int)e_graphicstype::FolderLayer; }

private:
	void OnLayerEvent(CBaseLayerMsg* Event);

	CGraphicsLayer* m_layer;
	QGraphicsWidget* m_layerlist;
	QGraphicsLinearLayout* m_layout;
};

#endif // CGraphicsFolder_H