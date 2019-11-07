/*	Description:
 *		Stores all variables and information needed to network a project.
 *		Makes up the core of what the software is entered around.
 *
 *		Details:
 *			All layers stored in a project are assumed to be owned by it.
 *			Destruction of a project will destroy all its layers.
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
#include "Events.h"
#include "CFolderLayer.h"

class CSharedProject : public CEventHandler<CProjectEventMsg>
{
public:
	inline QString Name() const { return m_name; }
	inline void SetName(const QString& Name) { m_name = Name; }
	inline const CFolderLayer* Layers() { return &m_layers; }

private:
	QString m_name;
	CFolderLayer m_layers;
};


#endif // CSharedProject_H