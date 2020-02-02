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

class TestThing : public QGraphicsTextItem, public CEventHandler<TestThing>
{
public:
	TestThing(QGraphicsItem* Parent = 0) : QGraphicsTextItem(Parent) { }
protected:
	void focusOutEvent(QFocusEvent* Event) override
	{
		//Event->accept();
		QGraphicsTextItem::focusOutEvent(Event);
		CreateEvent(*this);
	}

	void keyPressEvent(QKeyEvent* Event) override
	{
		if (Event->key() == Qt::Key_Return)
		{
			CreateEvent(*this);
			Event->accept();
		}
		else
			QGraphicsTextItem::keyPressEvent(Event);
	}
};

TestThing::Listeners_t TestThing::m_listeners;

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

	m_container = new QGraphicsWidget(this);
	m_label = new TestThing(m_container);
	m_label->setPos(0, 0);

	m_container->setVisible(true);
	m_container->setContentsMargins(0, 0, 0, 0);
	m_container->setPreferredSize(m_label->boundingRect().size());

	setPreferredSize(20, 100);
	m_layout->addItem(m_container);
	SetLayer(Layer);

	m_listenlayer = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
	m_listenclient = CClient::Listen([this](CBaseMsg* Msg) { OnClientEvent(Msg); });
}

CGraphicsLayer::~CGraphicsLayer() {
	CBaseLayer::EndListen(m_listenlayer);
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

	switch (Event->EventType())
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

void CGraphicsLayer::OnClientEvent(CBaseMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Msg_Event:
		OnLayerEvent((CBaseLayerMsg*)Msg);
		break;
	}
}

void CGraphicsLayer::NameEdited()
{
	m_label->setTextInteractionFlags(Qt::TextInteractionFlag::NoTextInteraction);
	CLayerEditMsg edit(m_layer);
	edit.SetNewName(m_label->toPlainText());
	CClient::Send(edit);
}

void CGraphicsLayer::contextMenuEvent(QGraphicsSceneContextMenuEvent * event)
{
	QMenu menu;
	QAction* remove = menu.addAction(tr("Remove")),
		* moveup = menu.addAction(tr("Move up")),
		* movedown = menu.addAction(tr("Move down")),
		* rename = menu.addAction(tr("Rename"));

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
	else if (action == rename)
	{
		m_label->setTextInteractionFlags(Qt::TextEditable | Qt::TextSelectableByMouse | Qt::TextSelectableByKeyboard);
		m_label->setFocus();
		((TestThing*)m_label)->Listen([this](TestThing* thing) { NameEdited(); });
	}
}
