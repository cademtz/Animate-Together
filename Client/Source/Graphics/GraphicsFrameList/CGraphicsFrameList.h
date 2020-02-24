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
#include "Graphics/GraphicsTypes.h"

class CBaseMsg;
class CBaseLayer;
class QGraphicsLinearLayout;

class CGraphicsFrameList : public QGraphicsWidget
{
public:
	CGraphicsFrameList(CBaseLayer* Layer, QGraphicsItem* Parent = 0);
	~CGraphicsFrameList();

	CBaseLayer* Layer() { return m_layer; }
	void SetLayer(CBaseLayer* Layer);

	int type() const override { return (int)EGraphicsType::FrameList; }
	QRectF boundingRect() const override { return rect(); }

private:
	void OnClientEvent(CBaseMsg* Msg);

	QGraphicsLinearLayout* m_layout;
	CBaseLayer* m_layer;

	unsigned m_listenlayer, m_listenclient;
};

#endif // CGraphicsFrameList_H