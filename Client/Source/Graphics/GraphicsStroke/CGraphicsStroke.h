/*	Description:
 *		A QGraphics representation of the CStroke class
 *
 *	Author: Hold on!
 *	Created: 11/4/2019 10:23:56 AM
 */

#ifndef CGraphicsStroke_H
#define CGraphicsStroke_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsitem.h>
#include "Graphics/GraphicsTypes.h"
#include "Projects/Objects/CStroke.h"

class CGraphicsStroke : public QGraphicsItem
{
public:
	CGraphicsStroke(const CStroke& Stroke, QGraphicsItem* Parent = 0) : QGraphicsItem(Parent), m_stroke(Stroke) { }

protected:
	int		type() const { return (int)e_graphicstype::Stroke; }
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
private:
	const CStroke& m_stroke;
};


#endif // CGraphicsStroke_H