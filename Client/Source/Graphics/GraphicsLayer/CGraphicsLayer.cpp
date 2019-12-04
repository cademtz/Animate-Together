/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:36:19 PM
 */

#include "CGraphicsLayer.h"
#include <qgraphicslinearlayout.h>
#include <qstyle.h>
#include "NetObjects/CBaseLayer.h"

CGraphicsLayer::CGraphicsLayer(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(0x808080));
	setPalette(pal);

	m_layout = new QGraphicsLinearLayout(Qt::Horizontal);
	setLayout(m_layout);

	// Keep everything aligned if certain icons aren't visible
	QSizePolicy policy = m_layout->sizePolicy();
	policy.setRetainSizeWhenHidden(true);
	m_layout->setSizePolicy(policy);

	/*
	Normal QGraphics items cannot be used in layouts,
	so basically every single primitive is useless.
	I Guess I'll have to make a whole class to shove a text item inside another layout item.
	Maybe tomorrow.

	Update: Okay so first attempt. Fingers crossed.
	Update: Okay so it failed. I'll figure it out tomorrow (again)
	Update: Well the only option is to parent them individualy to a whole new widget item just for themselves
	*/

	QGraphicsWidget* test = new QGraphicsWidget(this);
	m_label = new QGraphicsSimpleTextItem(test);
	test->setGeometry(QRectF(test->pos(), m_label->boundingRect().size()));
	setGeometry(QRectF(pos(), m_label->boundingRect().size()));
	m_layout->addItem(test);

	SetLayer(Layer);

	m_listener = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
}

CGraphicsLayer::~CGraphicsLayer() {
	CBaseLayer::EndListen(m_listener);
}

void CGraphicsLayer::SetLayer(CBaseLayer * Layer)
{
	m_layer = Layer;
	m_label->setText(m_layer->Name());
}

void CGraphicsLayer::OnLayerEvent(CBaseLayerMsg * Event)
{
}
