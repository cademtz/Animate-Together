/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/15/2019 11:18:47 PM
 */

#include "CUser.h"

CUser::CUser(const CJoinMsg & Joined)
	: CNetObject(Joined.Handle()), m_name(Joined.Name()), m_perms((EPerms)Joined.Perms()) { }
