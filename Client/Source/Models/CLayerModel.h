/*	Description:
 *		A model to represent the layers in a project
 *
 *	Author: Hold on!
 *	Created: 2/26/2020 10:49:37 PM
 */

#ifndef CLayerModel_H
#define CLayerModel_H
#ifdef _WIN32
#pragma once
#endif

#include <qabstractitemmodel.h>

class CBaseMsg;
class CBaseLayer;
class CBaseLayerMsg;
class CSharedProject;

struct LayerModelItem
{
	LayerModelItem(CBaseLayer* Layer, LayerModelItem* Parent = 0);
	CBaseLayer* m_layer;
	LayerModelItem* m_parent;
	QList<LayerModelItem> m_children;

	inline int rowCount() const { return m_children.size(); }
	int row() const;
};

class CLayerModel : public QAbstractItemModel
{
public:
	CLayerModel(CSharedProject* Proj, QObject* Parent = 0);
	~CLayerModel();

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override { return 1; }

private:
	LayerModelItem* FindItem(CBaseLayer* Layer, LayerModelItem* Parent = 0) const;
	QModelIndex index(CBaseLayer* Layer) const;
	void OnClientEvent(CBaseMsg* Msg);
	void OnLayerEvent(CBaseLayerMsg* Msg);

	CSharedProject* m_proj;
	LayerModelItem* m_root;
	unsigned m_clientlistener;
	unsigned m_layerlistener;
};

#endif // CLayerModel_H