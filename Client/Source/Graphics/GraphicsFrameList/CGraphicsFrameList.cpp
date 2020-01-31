/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 1/29/2020 11:52:06 PM
 */

#include "CGraphicsFrameList.h"
#include <qgraphicslinearlayout.h>
#include "NetObjects/CBaseLayer.h"
#include "Graphics/FrameItem/CFrameItem.h"

CGraphicsFrameList::CGraphicsFrameList(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	setContentsMargins(0, 0, 0, 0);
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(0x808080));
	setPalette(pal);

	m_layout = new QGraphicsLinearLayout(Qt::Horizontal);
	m_layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_layout);

	setPreferredSize(20, 100);
	SetLayer(Layer);
}

void CGraphicsFrameList::SetLayer(CBaseLayer * Layer)
{
	while (m_layout->count())
		delete m_layout->itemAt(0);

	for (int i = 0; i < Layer->Frames().size(); i++)
		m_layout->addItem(new CFrameItem());

	m_layer = Layer;
}
