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
public:
	static const QRectF m_rect;

	CFrame* Frame();

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
	bool IsSelected();

	CLayer* Layer();
	int Index();
};

#endif // CGraphicsFrame_H