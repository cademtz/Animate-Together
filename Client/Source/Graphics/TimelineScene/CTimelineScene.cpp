/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 5:05:48 PM
 */

#include "CTimelineScene.h"
#include <Shared/CSharedProject.h>
#include "Client/CClient.h"

CTimelineScene::CTimelineScene()
{
	CBaseLayer::Listen([this](CBaseLayerMsg* Event) { OnLayerEvent(Event); });
}

void CTimelineScene::OnClientEvent(CBaseMsg * Msg)
{
}

void CTimelineScene::OnLayerEvent(CBaseLayerMsg * Event)
{

}
