/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/1/2020 6:34:06 PM
 */

#include "CLayerView.h"
#include <qmenu.h>
#include <qheaderview.h>
#include <Shared/CNetEvent.h>
#include <Shared/CSharedProject.h>
#include "Client/CClient.h"
#include "Models/CLayerModel.h"

CLayerView::CLayerView(QWidget * Parent) : QTreeView(Parent)
{
	m_model = new CLayerModel(this);
	setModel(m_model);

	m_del =			new QAction("Remove");
	m_moveup =		new QAction("Move up");
	m_movedown =	new QAction("Move down");
	m_edit =		new QAction("Edit...");

	m_layermenu = new QMenu(this), m_framemenu = new QMenu(this);
	m_layermenu->addActions({ m_del, m_moveup, m_movedown, m_edit });
	m_framemenu->addAction(m_del);

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &CLayerView::customContextMenuRequested, this, &CLayerView::CustomMenu);
	connect(m_model, &CLayerModel::columnsInserted, this, [this] { ColumsInserted(); });
}

void CLayerView::CustomMenu(const QPoint & Pos)
{
	QModelIndex index = indexAt(Pos);
	if (!index.isValid())
		return;

	if (index.column())
		FrameMenu(Pos, index);
	else
		LayerMenu(Pos, index);
}

void CLayerView::LayerMenu(const QPoint& Pos, const QModelIndex& Index)
{
	CBaseLayer* layer = m_model->LayerAt(Index);
	m_moveup->setDisabled(layer->Parent()->IsRoot() && Index.row() <= 0);
	m_movedown->setDisabled(layer->Parent()->IsRoot() && Index.row() >= layer->Parent()->Layers().size() - 1);

	QAction* action = m_layermenu->exec(mapToGlobal(Pos));

	if (action == m_del)
		CClient::Send(CLayerAddMsg(layer, layer->Index(), layer->Parent(), false));
	else if (action == m_moveup || action == m_movedown)
	{
		bool cool = false;
		int index = layer->Index();
		CFolderLayer* parent = layer->Parent();
		if (parent)
		{
			int newindex = index + (action == m_moveup ? -1 : 1);
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
			CLayerEditMsg move(layer);
			move.SetNewPlace(index, parent);
			CClient::Send(move);
		}
	}
}

void CLayerView::FrameMenu(const QPoint & Pos, const QModelIndex & Index)
{
	CBaseLayer* layer = m_model->LayerAt(Index);
	QAction* action = m_framemenu->exec(mapToGlobal(Pos));
	if (action == m_del)
		CClient::Send(CFrameAddMsg(layer->Frames().at(Index.column() - 1), false));
}

void CLayerView::ColumsInserted()
{
	for (int i = 0; i < m_model->columnCount(); i++)
	{
		QSize size = m_model->headerData(i, Qt::Horizontal, Qt::SizeHintRole).toSize();
		setColumnWidth(i, size.width());
	}
}
