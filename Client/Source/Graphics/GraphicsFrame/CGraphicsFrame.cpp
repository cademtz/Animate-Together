/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/25/2019 8:28:45 PM
 */

#include "CGraphicsFrame.h"
#include <qmenu.h>
#include <qpainter.h>
#include <qgraphicssceneevent.h>
#include <qgraphicslinearlayout.h>
#include "Projects/CProject.h"
#include "Graphics/LayerLayout/CLayerLayout.h"

const QRectF CGraphicsFrame::m_rect = QRectF(0, 0, 8, 18);

CFrame * CGraphicsFrame::Frame()
{
	if (CLayer* layer = Layer())
	{
		int index = Index();
		if (index >= 0 && index < layer->Frames().size())
			return layer->Frames()[index];
	}
	return 0;
}

void CGraphicsFrame::SelectFrame(bool Select)
{
	if (CLayer* layer = Layer())
		layer->SelectFrame(Index(), Select);
	update();
}

void CGraphicsFrame::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->setClipRect(m_rect);
	CFrame* frame = Frame();
	if (!frame)
	{
		if (IsSelected())
			painter->fillRect(m_rect, QColor(64, 128, 255));

		painter->setPen(QPen(QColor(64, 64, 64), 2));
		painter->drawRect(QRectF(m_rect.topLeft(), m_rect.size() + QSize(0, -1)));
		return;
	}

	bool filled = !frame->IsEmpty(), left = Connected(Qt::LeftEdge), right = Connected(Qt::RightEdge);
	painter->fillRect(m_rect, filled ? QColor(100, 100, 100) : QColor(75, 75, 75));
	
	if (filled)
	{
		QRectF area(m_rect.topLeft(), m_rect.size() - QSize(2, 2));

		painter->setPen(QPen(QColor(117, 117, 117)));
		painter->drawLine(area.topLeft(), area.topRight());
		if (!left)
			painter->drawLine(area.topLeft(), area.bottomLeft());

		painter->setPen(QPen(QColor(108, 108, 108)));
		if (!right)
			painter->drawLine(area.topRight(), area.bottomRight());
		painter->drawLine(area.bottomLeft(), area.bottomRight());

	}

	painter->setPen(Qt::black);

	if (frame->IsKey())
	{
		painter->setBrush(filled ? Qt::black : Qt::transparent);
		painter->drawEllipse(m_rect.x() + 1, m_rect.y() + 9, 4, 4);
	}
	else if (!right)
			painter->drawRect(m_rect.x() + 1, m_rect.y() + 7, 4, 7);

	QRectF area(m_rect.topLeft(), m_rect.size() - QSize(1, 1));
	painter->setPen(QPen(QColor(43, 43, 43)));
	if (right)
	{
		painter->setPen(QPen(QColor(43, 43, 43, 64)));
		QRegion other(m_rect.x(), m_rect.y() + 2, m_rect.width(), m_rect.height() - 5);
		painter->setClipRegion(painter->clipRegion().subtracted(other));
	}

	painter->drawLine(area.topRight(), area.bottomRight());
	painter->setPen(QPen(QColor(43, 43, 43)));
	painter->setClipRect(m_rect);
	painter->drawLine(area.bottomLeft(), area.bottomRight());

	if (IsSelected())
		painter->fillRect(m_rect, QColor(64, 128, 255, 128));
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

void CGraphicsFrame::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	if (!parentLayoutItem())
		return;

	QMenu menu;
	QAction* del = menu.addAction("Delete");//, * copy = menu.addAction("Copy"), * paste = menu.addAction("Paste");

	CFrame* frame = Frame();
	if (frame)
	{
		if (!frame->IsSelected())
		{
			frame->Project()->DeselectFrames();
			frame->Select();
		}
	}
	else
	{
		del->setDisabled(true);
		//copy->setDisabled(true);
		//paste->setDisabled(true);
	}

	QAction *action = menu.exec(event->screenPos());
	if (action == del)
		frame->Layer()->Project()->RemoveSelectedFrames();
}

bool CGraphicsFrame::Connected(Qt::Edge Edge)
{
	CFrame* frame = Frame();
	if (!frame)
		return false;
	else if (Edge == Qt::LeftEdge)
		return !frame->IsKey();

	auto& frames = frame->Layer()->Frames();
	int index = frame->Index();
	if (index + 1 < frames.size() && !frames[index + 1]->IsKey())
		return true;

	return false;
}

bool CGraphicsFrame::IsSelected()
{
	if (CLayer* layer = Layer())
		return layer->IsFrameSelected(Index());
	return false;
}

CLayer * CGraphicsFrame::Layer()
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return 0;

	auto l_layer = Parent();
	if (!l_layer)
		return 0;
	return proj->Layers()[l_layer->Index()];

	return 0;
}

int CGraphicsFrame::Index()
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return 0;

	/*auto l_layer = (QGraphicsLinearLayout*)parentLayoutItem();
	if (!l_layer)
		return 0;*/

	/*for (int i = 0; i < l_layer->count(); i++)
	{
		if (l_layer->itemAt(i) == this)
			return i;
	}
	return -1;*/
	return Parent()->IndexOf(this);
}
