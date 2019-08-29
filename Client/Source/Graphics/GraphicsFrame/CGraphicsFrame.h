/*	Description:
 *		A frame object to be used and displayed in a Qt graphics scene
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 8:22:29 PM
 */

#ifndef CGraphicsFrame_H
#define CGraphicsFrame_H
#ifdef _WIN32
#pragma once
#endif

#include "Graphics/GraphicsTypes.h"
#include <qgraphicslayoutitem.h>
#include "Projects/Frame/CFrame.h"

class CGraphicsFrame : public  QGraphicsLayoutItem, public QGraphicsItem
{
	CFrame::e_state m_state;

public:
	CGraphicsFrame(CFrame::e_state State, QGraphicsItem* Parent = 0) : QGraphicsItem(Parent), m_state(State) { }

	inline CFrame::e_state State() const { return m_state; }
	inline void SetState(CFrame::e_state State) { m_state = State; }

	int		type() const { return (int)e_graphicstype::Frame; }
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QSizeF	sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
	void	setGeometry(const QRectF &geom);
	QRectF	boundingRect() const { return QRectF(0, 0, 8, 18); }

};

#endif // CGraphicsFrame_H