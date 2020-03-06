/*	Description:
 *		A model to represent the layers in a client's current project
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
class CBaseFrame;
class CBaseLayerMsg;
class CSharedProject;

struct FrameItem;

struct LayerModelItem
{
	LayerModelItem(CBaseLayer* Layer, LayerModelItem* Parent = 0);
	CBaseLayer* m_layer;
	LayerModelItem* m_parent;
	QList<LayerModelItem> m_children;
	QList<FrameItem> m_frames;

	inline int rowCount() const { return m_children.size(); }
	int columnCount() const;
	int row() const;
};

struct FrameItem
{
	FrameItem(CBaseFrame* Frame, LayerModelItem* Parent) : m_frame(Frame), m_parent(Parent) { }
	CBaseFrame* m_frame;
	LayerModelItem* m_parent;
};

class CLayerModel : public QAbstractItemModel
{
public:
	CLayerModel(QObject* Parent = 0);
	~CLayerModel();

	CBaseLayer* LayerAt(const QModelIndex& Index) const;

	QVariant data(const QModelIndex& index, int role) const override;
	Qt::ItemFlags flags(const QModelIndex& index) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
	QModelIndex parent(const QModelIndex& index) const override;
	int rowCount(const QModelIndex& parent = QModelIndex()) const override;
	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

private:
	void SetProj(CSharedProject* Proj);
	LayerModelItem* FindItem(CBaseLayer* Layer, LayerModelItem* Parent = 0) const;
	QModelIndex index(CBaseLayer* Layer) const;
	void OnClientEvent(CBaseMsg* Msg);

	CSharedProject* m_proj;
	LayerModelItem* m_root;
	unsigned m_clientlistener;
	unsigned m_layerlistener;
};

#endif // CLayerModel_H