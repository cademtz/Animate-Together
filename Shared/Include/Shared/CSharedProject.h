/*	Description:
 *		Stores all variables and information needed to network a project.
 *		Makes up the core of what Animate Together is centered around.
 *
 *		Details:
 *			All layers stored in a project are assumed to be owned by it.
 *			Destruction of a project will destroy all its layers.
 *
 *			Project framerate is optional and not obeyed by clients that desire different playback
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 4:57:11 PM
 */

#ifndef CSharedProject_H
#define CSharedProject_H
#ifdef _WIN32
#pragma once
#endif

#include <qstring.h>
#include <qlist.h>
#include "CEventHandler.h"
#include "CNetEvent.h"
#include "NetObjects/CFolderLayer.h"

class CSharedProject : public CEventHandler<CSharedProjectMsg>
{
public:
	CSharedProject();

	inline QString Name() const { return m_name; }
	inline void SetName(const QString& Name) { m_name = Name; }
	inline CFolderLayer& Root() { return m_root; }
	inline const CFolderLayer& Root() const { return Root(); }
	inline unsigned Framerate() const { return m_framerate; }
	inline void SetFramerate(unsigned Fps) { m_framerate = Fps; }

	// - Finds a child layer by handle
	// - Return is null if the layer is not listed
	template<typename T = CBaseLayer>
	inline T* FindLayer(const CNetObject& Obj, CBaseLayer::EType Type = CBaseLayer::Layer_Null) {
		return (T*)m_root.FindLayer(Obj, Type);
	}
	template<typename T = CBaseLayer>
	inline const T* FindLayer(const CNetObject& Obj, CBaseLayer::EType Type = CBaseLayer::Layer_Null) const {
		return (const T*)m_root.FindLayer(Obj, Type);
	}
	inline bool Contains(const CNetObject& Obj) { return m_root.Contains(Obj); }
	inline bool Contains(const CBaseLayer* Layer) { return Contains(Layer->Handle()); }

	inline const LayerList_t& Selected() const { return m_selected; }
	inline bool IsSelected(CBaseLayer* Layer) const { return m_selected.contains(Layer); }
	void SelectLayer(CBaseLayer* Layer, bool Select = true);

	// - Performs a net event and adds its pointer in the 'Events' list
	void AddEvent(CNetEvent* Event);
	inline const QList<CNetEvent*>& Events() const { return m_events; }

private:
	void OnLayerEvent(CNetEvent* Event);

	QString m_name;
	CFolderLayer m_root;
	unsigned m_framerate;
	QList<CBaseLayer*> m_selected;
	QList<CNetEvent*> m_events;
};

#endif // CSharedProject_H