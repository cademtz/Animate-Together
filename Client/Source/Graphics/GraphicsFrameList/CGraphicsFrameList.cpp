/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 1/29/2020 11:52:06 PM
 */

#include "CGraphicsFrameList.h"

CGraphicsFrameList::CGraphicsFrameList(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, Qt::red);
	setPalette(pal);

	m_layout = new QGraphicsLinearLayout(this);
	m_layout->setOrientation(Qt::Horizontal);
	m_layout->setSpacing(0);
	m_layout->setContentsMargins(0, 0, 0, 0);
}
