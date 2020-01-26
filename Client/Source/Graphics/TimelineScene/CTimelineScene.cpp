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
	pal.setBrush(QPalette::Window, Qt::red);
	m_widget->setPalette(pal);

	pal = m_drag->palette();
	pal.setBrush(QPalette::Window, Qt::black);
	m_drag->setPalette(pal);
	m_drag->setPreferredWidth(10);

	addItem(m_widget);

	m_layout->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
	m_layout->addItem(m_drag);

	//addItem(m_drag);

	CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
	CClient::Listen([this](CBaseMsg* Msg) { OnClientEvent(Msg); });
}

void CTimelineScene::ResetScene(CSharedProject * Project)
{
	if (!Project)
		delete m_root;
	else if (!m_root)
	{
		m_root = new CGraphicsFolder(&Project->Root());
		m_root->setPreferredWidth(800);
		//addItem(m_root);
		m_layout->insertItem(0, m_root);
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
