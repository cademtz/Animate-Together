/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 1/29/2020 11:52:06 PM
 */

#include "CGraphicsFrameList.h"
#include <qgraphicsscene.h>
#include <qgraphicslinearlayout.h>
#include "Client/CClient.h"
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
	m_layout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(5);
	setLayout(m_layout);

	setPreferredSize(100, 20);
	SetLayer(Layer);

	m_listenclient = CClient::Listen([this](CBaseMsg* Msg) { OnClientEvent(Msg); });
}

CGraphicsFrameList::~CGraphicsFrameList()
{
	CClient::EndListen(m_listenclient);
}

void CGraphicsFrameList::SetLayer(CBaseLayer * Layer)
{
	while (m_layout->count())
		delete m_layout->itemAt(0);

	for (auto frame : Layer->Frames())
		m_layout->addItem(new CFrameItem(frame));

	m_layer = Layer;
}

void CGraphicsFrameList::OnClientEvent(CBaseMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Msg_Event:
	{
		CNetEvent* e = (CNetEvent*)Msg;
		switch (e->EventType())
		{
		case CNetEvent::Event_FrameAdd:
		{
			CFrameAddMsg* add = (CFrameAddMsg*)e;
			if (add->Frame()->Parent() == m_layer)
			{
				if (add->IsAdd())
				{
					CFrameItem* frame = new CFrameItem(add->Frame());
					m_layout->insertItem(add->Index(), frame);
				}
				else
					m_layout->removeAt(add->Index());
			}
		}
		}
	}
	}
}
