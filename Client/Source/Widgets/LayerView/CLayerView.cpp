/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/1/2020 6:34:06 PM
 */

#include "CLayerView.h"
#include <qmenu.h>
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

	m_menu = new QMenu(this);
	m_menu->addActions({ m_del, m_moveup, m_movedown, m_edit });

	setContextMenuPolicy(Qt::CustomContextMenu);
	connect(this, &CLayerView::customContextMenuRequested, this, &CLayerView::CustomMenu);
}

void CLayerView::CustomMenu(const QPoint & Pos)
{
	QModelIndex index = indexAt(Pos);
	if (!index.isValid())
		return;

	CBaseLayer* layer = m_model->LayerAt(index);
	m_moveup->setDisabled(layer->Parent()->IsRoot() && index.row() <= 0);
	m_movedown->setDisabled(layer->Parent()->IsRoot() && index.row() >= layer->Parent()->Layers().size() - 1);

	QAction* action = m_menu->exec(mapToGlobal(Pos));

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
