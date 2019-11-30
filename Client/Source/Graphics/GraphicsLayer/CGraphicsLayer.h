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
#include "Graphics/GraphicsTypes.h"

class CBaseLayer;
class QGraphicsLinearLayout;

class CGraphicsLayer : public QGraphicsWidget
{
public:
	CGraphicsLayer(CBaseLayer* Layer, QGraphicsItem* Parent = nullptr);

	inline CBaseLayer* Layer() const { return m_layer; }
	inline void SetLayer(CBaseLayer* Layer) { m_layer = Layer; }

	int type() const override { return (int)e_graphicstype::Layer; }
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private:
	CBaseLayer* m_layer;
	QGraphicsLinearLayout* m_layout;
};

#endif // CGraphicsLayer_H