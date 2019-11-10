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
#include <NetObjects/CStroke.h>
#include "Graphics/GraphicsTypes.h"

class CGraphicsStroke : public QGraphicsPixmapItem
{
public:
	CGraphicsStroke(const CStroke& Stroke, QGraphicsItem* Parent = 0) : QGraphicsPixmapItem(Parent), m_stroke(Stroke) { }

	void Add(const CStroke& Other);

protected:
	int	type() const { return (int)e_graphicstype::Stroke; }

private:
	CStroke m_stroke;
};


#endif // CGraphicsStroke_H