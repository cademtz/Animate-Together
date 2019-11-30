/*	Description:
 *		A scene that contains and updates graphical representation of a project's layers and frames.
 *		Being a singleton, multiple view widgets may use it at the same time.
 *
 *	Author: Hold on!
 *	Created: 11/29/2019 5:00:37 PM
 */

#ifndef CTimelineScene_H
#define CTimelineScene_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsscene.h>

class CBaseMsg;
class CBaseLayerMsg;

class CTimelineScene : public QGraphicsScene
{
public:
	inline const CTimelineScene& Scene() const
	{
		static CTimelineScene scene;
		return scene;
	}

private:
	CTimelineScene();
	~CTimelineScene() { }

	void OnClientEvent(CBaseMsg* Msg);
	void OnLayerEvent(CBaseLayerMsg* Msg);
};

#endif // CTimelineScene_H