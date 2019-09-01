/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/28/2019 9:57:16 PM
 */

#include "CGraphicsScrubBar.h"
#include <qgraphicsscene.h>
#include <qpainter.h>

CGraphicsScrubBar::CGraphicsScrubBar(QGraphicsItem * Parent) : QGraphicsItem(Parent), m_size(QSizeF(9, 18)) {
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void CGraphicsScrubBar::SetWidth(qreal Width)
{
	m_size.setWidth(Width);
	QRectF r = geometry();
	r.setSize(m_size);
	setGeometry(r);
}

void CGraphicsScrubBar::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
	painter->fillRect(boundingRect(), QColor(50, 50, 50));
}

QSizeF CGraphicsScrubBar::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
	case Qt::MaximumSize:
		return m_size;
	default:
		break;
	}
	return constraint;
}

void CGraphicsScrubBar::setGeometry(const QRectF & geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

QRectF CGraphicsScrubBar::boundingRect() const {
	return QRectF(QPointF(0, 0), m_size);
}