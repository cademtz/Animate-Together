/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 8:28:45 PM
 */

#include "CGraphicsFrame.h"
#include <qpainter.h>

void CGraphicsFrame::SetFrame(CFrame * Frame)
{
	if (Frame == m_frame)
		return;
	m_frame = Frame;
	update();
}

void CGraphicsFrame::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setClipRect(boundingRect());

	bool filled = Frame()->State() != CFrame::Hold;
	QRectF bounds = boundingRect();

	painter->setPen(QPen(QColor(43, 43, 43)));
	painter->fillRect(bounds, filled ? QColor(100, 100, 100) : QColor(75, 75, 75));
	painter->drawRect(bounds.x() - 1, bounds.y(), bounds.width(), bounds.height());

	if (filled)
	{
		painter->setPen(QPen(QColor(118, 118, 118)));
		painter->drawRect(bounds.x(), bounds.y() + 1, bounds.width() - 2, bounds.height() - 2);
	}

	painter->setPen(Qt::black);

	switch (Frame()->State())
	{
	case CFrame::Key:
	case CFrame::Empty:
		painter->setBrush(filled ? Qt::black : Qt::transparent);
		painter->drawEllipse(bounds.x() + 1, bounds.y() + 10, 4, 4);
		break;
	case CFrame::Hold:
		painter->drawRect(bounds.x() + 1, bounds.y() + 8, 4, 7);
	}
}

QSizeF CGraphicsFrame::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
	case Qt::MaximumSize:
		return boundingRect().size();
	default:
		break;
	}
	return constraint;
}

void CGraphicsFrame::setGeometry(const QRectF & geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
}
