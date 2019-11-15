/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/10/2019 9:42:12 PM
 */

#include "CUserList.h"

CUser * CUserList::FromHandle(const CNetObject & Object)
{
	for (CUser* user : m_users)
		if (Object.Handle() == user->Handle())
			return user;
	return 0;
}
