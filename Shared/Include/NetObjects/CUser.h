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

class CJoinMsg;

class CUser : public CNetObject
{
public:
	enum EPerms : uint8_t
	{
		Perm_Guest			= 0,		// - If not set, the user may only spectate
		Perm_Privating		= (1 << 0),	// - User can private layers
		Perm_ManageUsers	= (1 << 1),	// - Server allows ban and kick requests
		Perm_ManageLayers	= (1 << 2),	// - User may override layer privating
		Perm_ManageProj		= (1 << 3),	// - User can change project settings
		Perm_Admin			= (1 << 4),	// - Overrides any server restrictions on this user
	};

	// - Creates an entirely new networkable user
	CUser(QString Name, EPerms Perms) : m_name(Name), m_perms(Perms) { }

	// - Create user from a join msg
	CUser(const CJoinMsg& Joined);

	inline QString Name() const { return m_name; }
	inline uint8_t Perms() const { return m_perms; }
	inline void SetPerms(EPerms Perms) { m_perms = Perms; }
	inline void AddAction(CNetEvent* Action) { m_actions.push_back(Action); }

private:
	QString m_name;
	EPerms m_perms;

	QList<CNetEvent*> m_actions;
};

#endif // CUser_H