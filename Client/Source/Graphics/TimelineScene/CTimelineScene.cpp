/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 5:05:48 PM
 */

#include "CTimelineScene.h"
#include <Shared/CSharedProject.h>
#include <qgraphicslinearlayout.h>
#include "Client/CClient.h"
#include "Graphics/GraphicsFolder/CGraphicsFolder.h"

CTimelineScene::CTimelineScene()
{
	m_widget = new QGraphicsWidget();
	m_drag = new QGraphicsWidget();
	m_layout = new QGraphicsLinearLayout(Qt::Horizontal, m_widget);

	QPalette pal = m_widget->palette();
	pal.setBrush(QPalette::Window, Qt::black);
	m_widget->setPalette(pal);
	m_widget->setAutoFillBackground(true);
	m_widget->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	m_drag->setPreferredWidth(5);
	m_drag->setFlag(QGraphicsItem::ItemIsMovable);

	addItem(m_widget);

	m_layout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
	m_layout->setContentsMargins(0, 0, 0, 0);
	m_layout->setSpacing(0);
	m_layout->addItem(m_drag);

	CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
	CClient::Listen([this](CBaseMsg* Msg) { OnClientEvent(Msg); });
	connect(m_drag, &QGraphicsWidget::geometryChanged, this, &CTimelineScene::OnDrag);
}

void CTimelineScene::ResetScene(CSharedProject * Project)
{
	if (!Project)
		delete m_root;
	else if (!m_root)
	{
		m_root = new CGraphicsFolder(&Project->Root());
		m_layout->insertItem(0, m_root);
		m_widget->adjustSize();
	}
	else
		m_root->SetFolder(&Project->Root());
}

void CTimelineScene::OnClientEvent(CBaseMsg * Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Msg_ProjSetup:
		ResetScene(CClient::Project());
		break;
	}
}

void CTimelineScene::OnLayerEvent(CBaseLayerMsg * Event)
{

}

void CTimelineScene::OnDrag()
{
	if (m_root)
	{
		int width = m_drag->pos().x();
		if (width < m_root->minimumWidth())
			width = m_root->minimumWidth();
		m_root->setPreferredWidth(width);
		m_widget->adjustSize();
	}
	m_layout->updateGeometry();
}
