/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/26/2020 10:54:56 PM
 */

#include "CLayerModel.h"
#include <Shared/CSharedProject.h>

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
	if (!hasIndex(row, column, parent))
		return QModelIndex();

	CBaseLayer *parentLayer;

	if (!parent.isValid())
		parentLayer = &m_proj->Root();
	else
		parentLayer = (CBaseLayer*)parent.internalPointer();

	CBaseLayer *child = nullptr;
	if (parentLayer->Type() == CBaseLayer::Layer_Folder)
		((CFolderLayer*)parentLayer)->Layers()[row];

	if (child)
		return createIndex(row, column, child);
	return QModelIndex();
}

QModelIndex CLayerModel::parent(const QModelIndex & index) const
{
	if (!index.isValid())
		return QModelIndex();

	CBaseLayer* parent = ((CBaseLayer*)index.internalPointer())->Parent();
	if (parent == &m_proj->Root())
		return QModelIndex();

	return createIndex(parent->Index(), 0, parent);
}

int CLayerModel::rowCount(const QModelIndex & parent) const
{
	CBaseLayer *parentLayer;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentLayer = &m_proj->Root();
	else
		parentLayer = (CBaseLayer*)parent.internalPointer();

	if (parentLayer->Type() == CBaseLayer::Layer_Folder)
		return ((CFolderLayer*)parentLayer)->Layers().size();
	return 0;
}
