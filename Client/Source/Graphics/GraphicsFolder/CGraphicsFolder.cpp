/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 7:58:52 PM
 */

#include "CGraphicsFolder.h"
#include <qgraphicslinearlayout.h>
#include <NetObjects/CFolderLayer.h>

CGraphicsFolder::CGraphicsFolder(CFolderLayer * Folder)
{
	m_layout = new QGraphicsLinearLayout(Qt::Vertical);
	setLayout(m_layout);

	m_layer = new CGraphicsLayer(Folder, this);
	m_layout->addItem(m_layer);
}

void CGraphicsFolder::OnLayerEvent(CBaseLayerMsg * Event)
{

}
