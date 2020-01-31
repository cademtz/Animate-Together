/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 1/28/2020 11:00:39 PM
 */

#include "CFrameItem.h"
#include <qgraphicssceneevent.h>
#include <qgraphicslayout.h>
#include <qobjectdefs.h>
#include <qpainter.h>
#include <qmenu.h>
#include "Graphics/GraphicsFrameList/CGraphicsFrameList.h"
#include "NetObjects/CBaseLayer.h"

QSizeF CFrameItem::m_size;

int CFrameItem::Index()
{
	QGraphicsLayout* layout = parentWidget()->layout();
	for (int i = 0; i < layout->count(); i++)
		if (layout->itemAt(i) == this)
			return i;
	return -1;
}

CBaseFrame * CFrameItem::Frame() {
	return Parent()->Layer()->Frames()[Index()];
}

void CFrameItem::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget) {
	painter->fillRect(boundingRect(), Qt::blue);
}

void CFrameItem::setGeometry(const QRectF & geom)
{
	prepareGeometryChange();
	QGraphicsLayoutItem::setGeometry(geom);
	setPos(geom.topLeft());
	m_size = geom.size();
}

void CFrameItem::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	QAction* del = menu.addAction(QObject::tr("Delete")),
		* copy = menu.addAction(QObject::tr("Copy")),
		* paste = menu.addAction(QObject::tr("Paste")),
		* color = menu.addAction(QObject::tr("Color...")),
		* lock = menu.addAction(QObject::tr("Lock"));

	del->setEnabled(false);
	copy->setEnabled(false);
	paste->setEnabled(false);
	color->setEnabled(false);
	lock->setEnabled(false);

	menu.exec(event->screenPos());
}