/*	Description:
 *		This is basically just CGraphicsFrameV2. I really should just be rewriting these in a separate branch
 *
 *	Author: Hold on!
 *	Created: 1/28/2020 10:55:12 PM
 */

#ifndef CFrameItem_H
#define CFrameItem_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsitem.h>
#include <qgraphicslayoutitem.h>
#include "Graphics/GraphicsTypes.h"

class CBaseFrame;

class CFrameItem : public QGraphicsLayoutItem, public QGraphicsItem
{
public:

	CBaseFrame* Frame();

	int		type() const override { return (int)e_graphicstype::Frame; }
	void	paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
	QSizeF	sizeHint(Qt::SizeHint which, const QSizeF &constraint) const override { return m_size; }
	void	setGeometry(const QRectF &geom) override;
	QRectF	boundingRect() const override { return QRectF(pos(), m_size); }
	void	contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

	static void SetSize(QSizeF Size) { m_size = Size; }
	inline bool IsSelected() const { return m_selected; }
	inline void SetSelected(bool Selected) { m_selected = Selected; }
	int Index();

private:
	static QSizeF m_size;
	bool m_selected = false;
};
#endif // CFrameItem_H