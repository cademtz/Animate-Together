/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/28/2019 9:57:16 PM
 */

#include "CGraphicsScrubBar.h"
#include <qgraphicsscene.h>
#include <qpainter.h>

CGraphicsScrubBar::CGraphicsScrubBar(const QRectF& Rect, QGraphicsItem * Parent) : QGraphicsRectItem(Rect, Parent)
{
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
	setBrush(QColor(50, 50, 50));
	//setMinimumHeight(18);
	//setGeometry(QRectF(0, 0,))
}

/*void CGraphicsScrubBar::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
	painter->fillRect(boundingRect(), QColor(50, 50, 50));
}*/

QSizeF CGraphicsScrubBar::sizeHint(Qt::SizeHint which, const QSizeF & constraint) const
{
	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
	case Qt::MaximumSize:
		return rect().size();
	default:
		break;
	}
	return constraint;
}

/*void CGraphicsScrubBar::setGeometry(const QRectF & geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}

QRectF CGraphicsScrubBar::boundingRect() const {
	return QRectF(0, 0, scene() ? scene()->width() : 18, 18);
}*/
