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
	inline const CFolderLayer& Root() { return m_root; }
	inline unsigned Framerate() const { return m_framerate; }

private:
	QString m_name;
	CFolderLayer m_root;
	unsigned m_framerate;
};

#endif // CSharedProject_H