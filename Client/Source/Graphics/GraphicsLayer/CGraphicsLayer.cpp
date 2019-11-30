/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:36:19 PM
 */

#include "CGraphicsLayer.h"
#include <qgraphicslinearlayout.h>

CGraphicsLayer::CGraphicsLayer(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	// Keep everything aligned when certain icons aren't used
	QSizePolicy policy = sizePolicy();
	policy.setRetainSizeWhenHidden(true);
	setSizePolicy(policy);

	m_layout = new QGraphicsLinearLayout(Qt::Horizontal);
	setLayout(m_layout);
}

void CGraphicsLayer::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
}
