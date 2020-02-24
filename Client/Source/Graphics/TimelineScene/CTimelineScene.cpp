/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 5:05:48 PM
 */

#include "CTimelineScene.h"
#include <qcursor.h>
#include <Shared/CSharedProject.h>
#include <qgraphicslinearlayout.h>
#include "Client/CClient.h"
#include "Graphics/GraphicsFolder/CGraphicsFolder.h"
#include "Graphics/GraphicsFrameList/CGraphicsFrameList.h"

CTimelineScene::CTimelineScene()
{
	m_widget = new QGraphicsWidget();
	m_drag = new QGraphicsWidget();
	m_layerframes = new QGraphicsWidget();
	m_layerframeslayout = new QGraphicsLinearLayout(Qt::Vertical);
	m_layout = new QGraphicsLinearLayout(Qt::Horizontal, m_widget);

	QPalette pal = m_widget->palette();
	pal.setBrush(QPalette::Window, Qt::black);
	m_widget->setPalette(pal);
	m_widget->setAutoFillBackground(true);
	m_widget->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	m_drag->setPreferredWidth(5);
	m_drag->setFlag(QGraphicsItem::ItemIsMovable);
	m_drag->setCursor(QCursor(Qt::SizeHorCursor));
	m_layerframes->setLayout(m_layerframeslayout);

	addItem(m_widget);

	m_layout->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
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
		m_root->adjustSize();
		m_drag->setPos(m_root->preferredWidth(), 0);
		m_layout->insertItem(0, m_root);
		auto layers = Project->Root().Layers1D();
		for (auto layer : layers)
			m_layerframeslayout->addItem(new CGraphicsFrameList(layer));
		m_layout->addItem(m_layerframes);
	}
	else
	{
		for (int i = m_layerframeslayout->count() - 1; i >= 0; i--)
			delete m_layerframeslayout->itemAt(i);
		auto layers = Project->Root().Layers1D();
		for (auto layer : layers)
			m_layerframeslayout->addItem(new CGraphicsFrameList(layer));
		m_root->SetFolder(&Project->Root());
	}
	OnDrag();
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
	switch (Event->EventType())
	{
	case CNetEvent::Event_LayerAdd:
	{
		CLayerAddMsg* add = (CLayerAddMsg*)Event;
		if (add->IsAdd())
			m_layerframeslayout->insertItem(add->Index(), new CGraphicsFrameList(add->Layer()));
		else
			m_layerframeslayout->removeAt(add->Index());
		break;
	}
	}
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
