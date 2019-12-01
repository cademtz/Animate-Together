/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:36:19 PM
 */

#include "CGraphicsLayer.h"
#include <qgraphicslinearlayout.h>
#include "NetObjects/CBaseLayer.h"

CGraphicsLayer::CGraphicsLayer(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	// Keep everything aligned when certain icons aren't used
	QSizePolicy policy = sizePolicy();
	policy.setRetainSizeWhenHidden(true);
	setSizePolicy(policy);

	m_layout = new QGraphicsLinearLayout(Qt::Horizontal);
	setLayout(m_layout);

	/*
	Normal QGraphics items cannot be used in layouts,
	so basically every single primitive is useless.
	I Guess I'll have to make a whole class to shove a text item inside another layout item.
	Maybe tomorrow.
	*/
	//m_label = new QGraphicsSimpleTextItem(m_layer->Name());
	//m_layout->addItem(m_label);

	m_listener = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
}

CGraphicsLayer::~CGraphicsLayer() {
	CBaseLayer::EndListen(m_listener);
}

void CGraphicsLayer::OnLayerEvent(CBaseLayerMsg * Event)
{
}
