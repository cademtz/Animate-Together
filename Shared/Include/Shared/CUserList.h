/*	Description:
 *		A static list of users that must be listening to HandleMsg calls to automatically manage users.
 *		Used for iterating users or retrieving a user by handle.
 *
 *	Author: Hold on!
 *	Created: 11/10/2019 6:44:37 PM
 */

#ifndef CUserList_H
#define CUserList_H
#ifdef _WIN32
#pragma once
#endif

#include <qlist.h>
#include "NetObjects/CUser.h"

class CUserList
{
public:
	// - Gets a CUser instance linked with a handle
	// - Returns a null pointer if not found
	CUser* FromHandle(const CNetObject& Object);
	inline QList<CUser*>& Users() { return m_users; }

private:
	QList<CUser*> m_users;
};

#endif // CUserList_H