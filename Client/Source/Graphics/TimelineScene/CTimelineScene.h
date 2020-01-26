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
class CSharedProject;
class CGraphicsFolder;
class QGraphicsLinearLayout;

class CTimelineScene : public QGraphicsScene
{
public:
	static inline CTimelineScene& Scene()
	{
		static CTimelineScene scene;
		return scene;
	}

private:
	CTimelineScene();
	~CTimelineScene() { }

	// - The root folder item is reset to the specified Project's root
	// - If 'Project' is left default or null, all graphical layers get destroyed
	void ResetScene(CSharedProject* Project = nullptr);

	void OnClientEvent(CBaseMsg* Msg);
	void OnLayerEvent(CBaseLayerMsg* Msg);

	QGraphicsWidget* m_widget, * m_drag;
	QGraphicsLinearLayout* m_layout;
	CGraphicsFolder* m_root;
};

#endif // CTimelineScene_H