/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 3:36:19 PM
 */

#include "CGraphicsLayer.h"
#include <qgraphicssceneevent.h>
#include <qgraphicslinearlayout.h>
#include <qmenu.h>
#include "NetObjects/CFolderLayer.h"
#include "Client/CClient.h"

CGraphicsLayer::CGraphicsLayer(CBaseLayer * Layer, QGraphicsItem * Parent)
	: QGraphicsWidget(Parent), m_layer(Layer)
{
	setContentsMargins(0, 0, 0, 0);
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(0x808080));
	setPalette(pal);

	// Keep everything aligned if certain icons aren't visible
	//QSizePolicy policy = m_layout->sizePolicy();
	//policy.setRetainSizeWhenHidden(true);
	//policy.setVerticalPolicy(QSizePolicy::Minimum);
	//policy.setHorizontalPolicy(QSizePolicy::Minimum);

	m_layout = new QGraphicsLinearLayout(Qt::Horizontal);
	m_layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
	m_layout->setContentsMargins(0, 0, 0, 0);
	setLayout(m_layout);

	/*
	Normal QGraphics items cannot be used in layouts,
	so basically every single primitive is useless.
	I Guess I'll have to make a whole class to shove a text item inside another layout item.
	Maybe tomorrow.

	Update: Okay so first attempt. Fingers crossed.
	Update: Okay so it failed. I'll figure it out tomorrow (again)
	Update: Well the only option is to parent them individualy to a whole new widget item just for themselves
	*/

	m_container = new QGraphicsWidget(this);

	m_label = new QGraphicsTextItem(m_container);
	m_label->setPos(0, 0);

	m_container->setVisible(true);
	m_container->setContentsMargins(0, 0, 0, 0);
	m_container->setPreferredSize(m_label->boundingRect().size());

	setPreferredSize(20, 100);
	m_layout->addItem(m_container);
	SetLayer(Layer);

	m_listener = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
}

CGraphicsLayer::~CGraphicsLayer() {
	CBaseLayer::EndListen(m_listener);
}

void CGraphicsLayer::SetLayer(CBaseLayer * Layer)
{
	m_layer = Layer;
	m_label->setPlainText(m_layer->Name());
	m_container->setPreferredSize(m_label->boundingRect().size());
	setPreferredSize(100, 20);
}

void CGraphicsLayer::OnLayerEvent(CBaseLayerMsg * Event)
{
	if (Event->Layer() != m_layer)
		return;

	switch (Event->Type())
	{
	case CNetEvent::Event_LayerEdit:
	{
		CLayerEditMsg* edit = (CLayerEditMsg*)Event;
		if (edit->Edited() & CLayerEditMsg::Edit_Name)
			m_label->setPlainText(edit->NewName());
		break;
	}
	}
}

void CGraphicsLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	QAction* remove = menu.addAction(tr("Remove")),
		* moveup = menu.addAction(tr("Move up")),
		* movedown = menu.addAction(tr("Move down"));

	if (!m_layer->Parent())
	{
		moveup->setDisabled(true);
		movedown->setDisabled(true);
	}

	QAction* action = menu.exec(event->screenPos());
	if (action == remove)
		CClient::Send(CLayerAddMsg(m_layer, false));
	else if (action == moveup || action == movedown)
	{
		bool cool = false;
		int index = m_layer->Index();
		CFolderLayer* parent = m_layer->Parent();
		if (parent)
		{
			int newindex = index + (action == moveup ? -1 : 1);
			if (newindex >= 0 && newindex <= parent->Layers().size() - 1)
			{
				index = newindex;
				cool = true;
			}
			else if (parent->Parent())
			{
				index = parent->Index() + (newindex > 0);
				parent = parent->Parent();
				cool = true;
			}
		}
		if (cool)
		{
			CLayerEditMsg move(m_layer);
			move.SetNewPlace(index, parent);
			CClient::Send(move);
		}
	}
}
