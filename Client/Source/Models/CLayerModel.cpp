/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/26/2020 10:54:56 PM
 */

#include "CLayerModel.h"
#include <Shared/CSharedProject.h>
#include "Client/CClient.h"

CLayerModel::CLayerModel(QObject * Parent) : QAbstractItemModel(Parent)
{
	m_proj = CClient::Project();
	SetProj(m_proj);

	m_clientlistener = CClient::Listen([this](CBaseMsg* Msg) { OnClientEvent(Msg); });
	m_layerlistener = CBaseLayer::Listen([this](CBaseLayerMsg* Msg) { OnLayerEvent(Msg); });
}

CLayerModel::~CLayerModel()
{
	CClient::EndListen(m_clientlistener);
	CBaseLayer::EndListen(m_layerlistener);
	delete m_root;
}

CBaseLayer * CLayerModel::LayerAt(const QModelIndex & Index) const {
	return ((LayerModelItem*)Index.internalPointer())->m_layer;
}

QVariant CLayerModel::data(const QModelIndex & index, int role) const
{
	switch (role)
	{
	case Qt::DisplayRole:
	{
		LayerModelItem* item = (LayerModelItem*)index.internalPointer();
		if (index.column())
			return item->m_frames.at(index.column() - 1).m_frame->IsKey() ? "K" : "-";
		else
			return item->m_layer->Name();
	}
	}
	return QVariant();
}

Qt::ItemFlags CLayerModel::flags(const QModelIndex & index) const
{
	if (!index.isValid())
		return Qt::NoItemFlags;
	return QAbstractItemModel::flags(index);
}

QVariant CLayerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	return QVariant();
}

QModelIndex CLayerModel::index(int row, int column, const QModelIndex & parent) const
{
	if (!m_proj || !hasIndex(row, column, parent))
		return QModelIndex();

	LayerModelItem *item;

	if (!parent.isValid())
		item = m_root;
	else
		item = (LayerModelItem*)parent.internalPointer();

	LayerModelItem* child = &item->m_children[row];

	if (child)
		return createIndex(row, column, child);
	return QModelIndex();
}

QModelIndex CLayerModel::parent(const QModelIndex & index) const
{
	if (!m_proj || !index.isValid())
		return QModelIndex();

	LayerModelItem* parent = ((LayerModelItem*)index.internalPointer())->m_parent;
	if (parent == m_root)
		return QModelIndex();

	return createIndex(parent->row(), 0, parent);
}

int CLayerModel::rowCount(const QModelIndex & parent) const
{
	if (!m_proj || parent.column() > 0)
		return 0;

	LayerModelItem *item;
	if (!parent.isValid())
		item = m_root;
	else
		item = (LayerModelItem*)parent.internalPointer();

	return item->rowCount();
}

int CLayerModel::columnCount(const QModelIndex & parent) const
{
	if (!m_proj)
		return 0;
	else if (parent.column() > 0)
		return 1;

	LayerModelItem *item;
	if (!parent.isValid())
		item = m_root;
	else
		item = (LayerModelItem*)parent.internalPointer();
	return item->columnCount();
}

void CLayerModel::SetProj(CSharedProject * Proj)
{
	if (m_proj)
	{
		beginRemoveRows(QModelIndex(), 0, m_root->rowCount() - 1);
		delete m_root;
		m_root = nullptr, m_proj = nullptr;
		endRemoveRows();
	}

	if (Proj)
	{
		m_proj = Proj;
		bool newLayers = !Proj->Root().Layers().isEmpty();
		if (newLayers)
			beginInsertRows(QModelIndex(), 0, Proj->Root().Layers().size());
		m_root = new LayerModelItem(&Proj->Root());
		if (newLayers)
			endInsertRows();
	}
}

LayerModelItem* CLayerModel::FindItem(CBaseLayer * Layer, LayerModelItem* Parent) const
{
	if (!Parent)
		Parent = m_root;

	for (auto& item : Parent->m_children)
	{
		if (item.m_layer == Layer)
			return &item;
		if (LayerModelItem* found = FindItem(Layer, &item))
			return found;
	}
	return nullptr;
}

QModelIndex CLayerModel::index(CBaseLayer * Layer) const
{
	if (!Layer->IsRoot() && Layer->RootProject() == m_proj)
	{
		LayerModelItem* item = FindItem(Layer);
		return createIndex(item->row(), 0, item);
	}
	return QModelIndex();
}

void CLayerModel::OnClientEvent(CBaseMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Msg_ProjSetup:
		SetProj(CClient::Project());
		break;
	case CBaseMsg::Msg_Event:
	{
		CNetEvent* e = (CNetEvent*)Msg;
		switch (e->EventType())
		{
		case CNetEvent::Event_FrameAdd:
		{
			CFrameAddMsg* add = (CFrameAddMsg*)e;
			if (add->WasAdded())
			{
				QModelIndex layerIndex = index(add->Frame()->Parent());
				LayerModelItem* item = (LayerModelItem*)layerIndex.internalPointer();
				beginInsertColumns(layerIndex, add->Index() + 1, add->Index() + 1);
				item->m_frames.insert(add->Index(), FrameItem(add->Frame(), item));
				endInsertColumns();
			}
			break;
		}
		case CNetEvent::Event_LayerEdit:
		{
			CLayerEditMsg* edit = (CLayerEditMsg*)Msg;
			QModelIndex layerIndex = index(edit->Layer());
			QModelIndex parent = layerIndex.parent();
			if (!(edit->Edited() & CLayerEditMsg::Edit_Place))
			{
				dataChanged(layerIndex, layerIndex);
			}
			else
			{
				LayerModelItem item = *(LayerModelItem*)layerIndex.internalPointer();
				beginRemoveRows(layerIndex.parent(), layerIndex.row(), layerIndex.row());
				item.m_parent->m_children.removeAt(item.row());
				endRemoveRows();

				parent = index(edit->NewParent());
				LayerModelItem* parentItem = (LayerModelItem*)parent.internalPointer();
				beginInsertRows(parent, edit->NewIndex(), edit->NewIndex());
				parentItem->m_children.insert(edit->NewIndex(), item);
				endInsertRows();
			}
			break;
		}
		}
		break;
	}
	}
}

void CLayerModel::OnLayerEvent(CBaseLayerMsg * Msg)
{
	switch (Msg->EventType())
	{
	case CNetEvent::Event_LayerAdd: // TODO: Just make client emit these already, man
	{
		CLayerAddMsg* add = (CLayerAddMsg*)Msg;
		int row = add->Index();
		QModelIndex parent = index(add->Parent());

		LayerModelItem* item;
		if (!parent.isValid())
			item = m_root;
		else
			item = ((LayerModelItem*)parent.internalPointer());

		if (add->WasAdded())
		{
			beginInsertRows(parent, row, row);
			item->m_children.insert(row, LayerModelItem(add->Layer(), item));
			endInsertRows();
		}
		else
		{
			beginRemoveRows(parent, row, row);
			item->m_children.removeAt(row);
			endRemoveRows();
		}
		break;
	}
	}
}

LayerModelItem::LayerModelItem(CBaseLayer * Layer, LayerModelItem* Parent)
	: m_layer(Layer), m_parent(Parent)
{
	if (Layer->Type() == CBaseLayer::Layer_Folder)
	{
		CFolderLayer* folder = (CFolderLayer*)Layer;
		for (CBaseLayer* layer : folder->Layers())
			m_children.append(LayerModelItem(Layer, this));
	}

	for (auto frame : Layer->Frames())
		m_frames.append(FrameItem(frame, this));
}

int LayerModelItem::row() const
{
	if (m_parent)
	{
		int i = 0;
		for (auto& item : m_parent->m_children)
		{
			if (&item == this)
				return i;
			i++;
		}
	}
	return -1;
}