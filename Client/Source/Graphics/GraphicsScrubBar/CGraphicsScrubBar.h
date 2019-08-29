/*	Description:
 *		Graphics item that displays the frame interval with a playhead
 *
 *	Author: Hold on!
 *	Created: 8/28/2019 9:42:13 PM
 */

#ifndef CGraphicsScrubBar_H
#define CGraphicsScrubBar_H
#ifdef _WIN32
#pragma once
#endif

#include "Graphics/GraphicsTypes.h"
#include <qgraphicslayoutitem.h>

class CGraphicsScrubBar : public QGraphicsLayoutItem, public QGraphicsRectItem
{
public:
	CGraphicsScrubBar(const QRectF& Rect, QGraphicsItem* Parent = 0);

	int		type() const { return (int)e_graphicstype::ScrubBar; }
	//void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QSizeF	sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
	//void	setGeometry(const QRectF &geom);
	//QRectF	boundingRect() const;
};

#endif // CGraphicsScrubBar_H