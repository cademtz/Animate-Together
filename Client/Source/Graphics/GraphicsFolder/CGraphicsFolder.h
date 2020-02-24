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
class CBaseMsg;
class CBaseLayerMsg;

class CGraphicsFolder : public QGraphicsWidget
{
public:
	CGraphicsFolder(CFolderLayer* Folder, QGraphicsItem* Parent = nullptr);
	~CGraphicsFolder();

	inline CFolderLayer* Folder() const { return m_folder; }
	void SetFolder(CFolderLayer* Folder);
	int	type() const override { return (int)EGraphicsType::FolderLayer; }

private:
	QGraphicsWidget* FindLayerWidget(const CBaseLayer* Layer);
	void InsertLayer(CBaseLayer* Layer);
	void RemoveLayer(const CBaseLayer* Layer);
	void MoveLayer(int Index, CBaseLayer* Layer);

	void OnClientEvent(CBaseMsg* Msg);
	void OnLayerEvent(CBaseLayerMsg* Event);
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;

	CFolderLayer* m_folder;
	CGraphicsLayer* m_item;
	QGraphicsWidget* m_layerlist;
	QGraphicsLinearLayout* m_layout, * m_listlayout;
	unsigned m_listenlayer, m_listenclient;
};

#endif // CGraphicsFolder_H