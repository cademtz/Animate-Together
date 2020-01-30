/*	Description:
 *		Provides a graphics object representation of a layer
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:32:02 PM
 */

#ifndef CGraphicsLayer_H
#define CGraphicsLayer_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicswidget.h>
#include <qgraphicslayoutitem.h>
#include "Graphics/GraphicsTypes.h"
#include "Graphics/LayoutWrap/CLayoutWrap.h"

class CBaseLayer;
class CBaseLayerMsg;
class QGraphicsLinearLayout;

class CGraphicsLayer : public QGraphicsWidget
{
public:
	CGraphicsLayer(CBaseLayer* Layer, QGraphicsItem* Parent = nullptr);
	~CGraphicsLayer();

	inline CBaseLayer* Layer() const { return m_layer; }
	void SetLayer(CBaseLayer* Layer);

	int type() const override { return (int)EGraphicsType::Layer; }

private:
	void OnLayerEvent(CBaseLayerMsg* Event);
	void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;

	unsigned m_listener;
	CBaseLayer* m_layer;
	QGraphicsTextItem* m_label;
	QGraphicsWidget* m_container;
	QGraphicsLinearLayout* m_layout;
};

#endif // CGraphicsLayer_H