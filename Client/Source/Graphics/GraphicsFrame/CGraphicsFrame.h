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
#include <qgraphicswidget.h>
#include "Projects/Frame/CFrame.h"

class CGraphicsFrame : public QGraphicsLayoutItem, public QGraphicsItem
{
	CFrame* m_frame;

public:
	static const QRectF m_rect;

	CGraphicsFrame(CFrame* Frame, QGraphicsLayoutItem* Parent = 0) : QGraphicsLayoutItem(Parent), m_frame(Frame) { }

	inline CFrame* Frame() const { return m_frame; }
	void SetFrame(CFrame* Frame);

	void SelectFrame(bool Select = true);

	int		type() const { return (int)e_graphicstype::Frame; }
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	QSizeF	sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
	void	setGeometry(const QRectF &geom);
	QRectF	boundingRect() const { return m_rect; }
	void	contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

private:
	// - Returns when to display a connection to the right and / or left frame
	bool Connected(Qt::Edge Edge);
};

#endif // CGraphicsFrame_H