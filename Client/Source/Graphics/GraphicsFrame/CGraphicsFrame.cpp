/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 8:28:45 PM
 */

#include "CGraphicsFrame.h"
#include <qpainter.h>
#include "Projects/CProject.h"

const QRectF CGraphicsFrame::m_rect = QRectF(0, 0, 8, 18);

void CGraphicsFrame::SetFrame(CFrame * Frame)
{
	if (Frame == m_frame)
		return;
	m_frame = Frame;
	update();
}

void CGraphicsFrame::SelectFrame(bool Select)
{
	Frame()->Layer()->SelectFrame(Frame(), Select);
	update();
}

void CGraphicsFrame::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setClipRect(m_rect);

	bool filled = Frame()->State() != CFrame::Hold;

	painter->fillRect(m_rect, filled ? QColor(100, 100, 100) : QColor(75, 75, 75));

	if (filled)
	{
		painter->setPen(QPen(QColor(118, 118, 118)));
		painter->drawRect(m_rect.x(), m_rect.y() + 1, m_rect.width() - 2, m_rect.height() - 2);
	}

	painter->setPen(Qt::black);

	switch (Frame()->State())
	{
	case CFrame::Key:
	case CFrame::Empty:
		painter->setBrush(filled ? Qt::black : Qt::transparent);
		painter->drawEllipse(m_rect.x() + 1, m_rect.y() + 10, 4, 4);
		break;
	case CFrame::Hold:
		painter->drawRect(m_rect.x() + 1, m_rect.y() + 8, 4, 7);
	}

	painter->setPen(QPen(QColor(43, 43, 43)));
	painter->setBrush(Frame()->Layer()->IsFrameSelected(Frame()) ? QColor(0, 128, 255, 128) : Qt::transparent);
	painter->drawRect(m_rect.x() - 1, m_rect.y(), m_rect.width(), m_rect.height());
}

QSizeF CGraphicsFrame::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
	switch (which)
	{
	case Qt::MinimumSize:
	case Qt::PreferredSize:
	case Qt::MaximumSize:
		return m_rect.size();
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
