/*	Description:
 *		A graphics widget to hold multiple frame items
 *
 *	Author: Hold on!
 *	Created: 1/29/2020 11:47:11 PM
 */

#ifndef CGraphicsFrameList_H
#define CGraphicsFrameList_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicswidget.h>
#include <qgraphicslinearlayout.h>
#include "Graphics/GraphicsTypes.h"

class CBaseLayer;

class CGraphicsFrameList : public QGraphicsWidget
{
public:
	CGraphicsFrameList(CBaseLayer* Layer, QGraphicsItem* Parent = 0);

	int type() const override { return (int)EGraphicsType::FrameList; }
	QRectF boundingRect() const override { return rect(); }

private:
	QGraphicsLinearLayout* m_layout;
	CBaseLayer* m_layer;
};

#endif // CGraphicsFrameList_H