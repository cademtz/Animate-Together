/*	Description:
 *		Provides a class to manage and network user data
 *
 *	Author: Hold on!
 *	Created: 11/8/2019 11:09:55 PM
 */

#ifndef CUser_H
#define CUser_H
#ifdef _WIN32
#pragma once
#endif

#include <qstring.h>
#include "CNetObject.h"
#include "Shared/CNetEvent.h"

class CUser : public CNetObject
{
public:
	enum EPerms : uint8_t
	{
		Perm_Guest,			// - If not set, the user may only spectate
		Perm_Privating,		// - User can private layers
		Perm_ManageUsers,	// - Server allows ban and kick requests
		Perm_ManageLayers,	// - User may override layer privating
		Perm_ManageProj,	// - User can change project settings
		Perm_Admin,			// - Overrides any server restrictions on this user
	};

	// - Creates an entirely new networkable user
	CUser(QString Name) : m_name(Name) { }

	inline QString Name() const { return m_name; }
	inline void AddAction(CNetEvent* Action) { m_actions.push_back(Action); }

private:
	QString m_name;
	EPerms m_perms;

	QList<CNetEvent*> m_actions;
};

#endif // CUser_H