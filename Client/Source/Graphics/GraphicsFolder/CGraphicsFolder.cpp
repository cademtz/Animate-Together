/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 7:58:52 PM
 */

#include "CGraphicsFolder.h"
#include <qgraphicslinearlayout.h>
#include <NetObjects/CFolderLayer.h>

CGraphicsFolder::CGraphicsFolder(CFolderLayer * Folder, QGraphicsItem* Parent) : QGraphicsWidget(Parent), m_folder(Folder)
{
	setAutoFillBackground(true);
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(0xC0C0C0));
	setPalette(pal);

	m_layout = new QGraphicsLinearLayout(Qt::Vertical);
	setLayout(m_layout);

	m_item = new CGraphicsLayer(m_folder, this);
	m_layout->addItem(m_item);

	m_layerlist = new QGraphicsWidget(this);
	m_listlayout = new QGraphicsLinearLayout(Qt::Vertical);
	m_listlayout->setContentsMargins(15, 0, 0, 0);
	m_layerlist->setLayout(m_listlayout);
	m_listlayout->setSpacing(0);

	SetFolder(Folder);

	adjustSize();

	m_listener = CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
}

CGraphicsFolder::~CGraphicsFolder()
{
	CBaseLayer::EndListen(m_listener);
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

	QGraphicsLayoutItem* item;
	if (Layer->Type() == CBaseLayer::Layer_Folder)
		item = new CGraphicsFolder((CFolderLayer*)Layer);
	else
		item = new CGraphicsLayer(Layer);
	m_listlayout->insertItem(Layer->Index(), item);
}

void CGraphicsFolder::RemoveLayer(const CBaseLayer* Layer)
{
	QGraphicsWidget* item = FindLayerWidget(Layer);
	if (item)
		delete item;
}

void CGraphicsFolder::MoveLayer(int Index, const CBaseLayer * Layer)
{
	QGraphicsWidget* item = FindLayerWidget(Layer);
	if (!item)
		return;

	m_listlayout->removeItem(item);
	m_listlayout->insertItem(Index, item);
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
		if (edit->Edited() & CLayerEditMsg::Edit_Index && m_folder->IsDirectChild(edit->Layer()))
			MoveLayer(edit->NewIndex(), edit->Layer());
		break;
	}
	}
}

void CGraphicsFolder::mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event) {
	m_layerlist->setVisible(m_open = !m_open);
}
