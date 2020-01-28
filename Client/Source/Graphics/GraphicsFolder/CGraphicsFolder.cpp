/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 7:58:52 PM
 */

#include "CGraphicsFolder.h"
#include "Client/CClient.h"
#include <qgraphicslinearlayout.h>
#include <NetObjects/CFolderLayer.h>

CGraphicsFolder::CGraphicsFolder(CFolderLayer * Folder, QGraphicsItem* Parent) : QGraphicsWidget(Parent), m_folder(Folder)
{
	setAutoFillBackground(true);
	setContentsMargins(0, 0, 0, 0);

	m_layout = new QGraphicsLinearLayout(Qt::Vertical, this);
	m_layout->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Fixed);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);

	m_item = new CGraphicsLayer(m_folder, this);
	m_layout->addItem(m_item);
	m_item->setSizePolicy(m_layout->sizePolicy());

	m_layerlist = new QGraphicsWidget(this);
	m_layerlist->setMinimumHeight(0);
	//m_layerlist->setSizePolicy(policy);
	QPalette pal = m_layerlist->palette();
	pal.setBrush(QPalette::Window, QColor(0x404040));
	setPalette(pal);

	m_listlayout = new QGraphicsLinearLayout(Qt::Vertical, m_layerlist);
	m_listlayout->setContentsMargins(15, 0, 0, 0);
	m_listlayout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	m_listlayout->setSpacing(0);
	//m_listlayout->setPreferredHeight(0);


	SetFolder(Folder);

	//adjustSize();
	//setMinimumSize(size() + QSizeF(15, 0));

	m_listenlayer = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
	m_listenclient = CClient::Listen([this](CBaseMsg* Event) { OnClientEvent(Event); });
}

CGraphicsFolder::~CGraphicsFolder()
{
	CBaseLayer::EndListen(m_listenlayer);
	CClient::EndListen(m_listenclient);
}

void CGraphicsFolder::SetFolder(CFolderLayer * Folder)
{
	m_folder = Folder;
	m_open = true;

	while (m_layerlist->children().size())
		delete m_layerlist->children().back();

	for (auto layer : m_folder->Layers())
		InsertLayer(layer);

	m_layerlist->setVisible(m_open);
	m_layout->addItem(m_layerlist);
}

QGraphicsWidget * CGraphicsFolder::FindLayerWidget(const CBaseLayer * Layer)
{
	for (auto item : m_layerlist->childItems())
	{
		QGraphicsWidget* found = nullptr;
		switch (item->type())
		{
		case (int)e_graphicstype::FolderLayer:
		{
			CGraphicsFolder* folder = (CGraphicsFolder*)item;
			if (folder->Folder() == Layer)
				found = folder;
			break;
		}
		case (int)e_graphicstype::Layer:
		{
			CGraphicsLayer* layer = (CGraphicsLayer*)item;
			if (layer->Layer() == Layer)
				found = layer;
			break;
		}
		}
		if (found)
			return found;
	}
	return nullptr;
}

void CGraphicsFolder::InsertLayer(CBaseLayer* Layer)
{
	if (!Layer || !m_folder->IsDirectChild(Layer->Handle()))
		return; // This folder does not own specified layer

	QGraphicsWidget* item;
	if (Layer->Type() == CBaseLayer::Layer_Folder)
		item = new CGraphicsFolder((CFolderLayer*)Layer);
	else
		item = new CGraphicsLayer(Layer);
	item->setFlag(QGraphicsItem::ItemIsMovable);
	m_listlayout->insertItem(Layer->Index(), item);
}

void CGraphicsFolder::RemoveLayer(const CBaseLayer* Layer)
{
	if (QGraphicsWidget* item = FindLayerWidget(Layer))
		delete item;
}

void CGraphicsFolder::MoveLayer(int Index, CBaseLayer * Layer)
{
	if (QGraphicsWidget* item = FindLayerWidget(Layer))
	{
		m_listlayout->removeItem(item);
		m_listlayout->insertItem(Index, item);
	}
	else
		InsertLayer(Layer);
}

void CGraphicsFolder::OnClientEvent(CBaseMsg * Msg)
{
	if (Msg->Type() == CBaseMsg::Msg_Event)
	{
		CNetEvent* netevent = (CNetEvent*)Msg;
		if (netevent->EventType() == CNetEvent::Event_LayerEdit)
			OnLayerEvent((CLayerEditMsg*)netevent);
	}
}

void CGraphicsFolder::OnLayerEvent(CBaseLayerMsg * Event)
{
	switch (Event->EventType())
	{
	case CNetEvent::Event_LayerAdd:
	{
		CLayerAddMsg* add = (CLayerAddMsg*)Event;
		if (add->WasAdded() && m_folder->IsDirectChild(add->Layer()))
			InsertLayer(add->Layer());
		else if (!add->WasAdded())
			RemoveLayer(add->Layer());
		break;
	}
	case CNetEvent::Event_LayerEdit:
	{
		CLayerEditMsg* edit = (CLayerEditMsg*)Event;
		if (edit->Edited() & CLayerEditMsg::Edit_Place)
		{
			if (m_folder->IsDirectChild(edit->Layer()))
				MoveLayer(edit->NewIndex(), edit->Layer());
			else
				RemoveLayer(edit->Layer());
		}
		break;
	}
	}
}

void CGraphicsFolder::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event)
{
	m_layerlist->setVisible(m_open = !m_open);
	for (QGraphicsWidget* widg = m_layerlist; widg; widg = widg->parentWidget())
	{
		widg->adjustSize();
		//if (widg->layout())
			//widg->layout()->updateGeometry();
		//widg->update();
	}
}
