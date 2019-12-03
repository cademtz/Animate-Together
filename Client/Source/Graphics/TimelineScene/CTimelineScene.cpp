/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 5:05:48 PM
 */

#include "CTimelineScene.h"
#include <Shared/CSharedProject.h>
#include "Client/CClient.h"
#include "Graphics/GraphicsFolder/CGraphicsFolder.h"

CTimelineScene::CTimelineScene()
{
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
		addItem(m_root);
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
