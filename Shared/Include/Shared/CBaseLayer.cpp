/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:10:42 PM
 */

#include "CBaseLayer.h"
#include "CFolderLayer.h"

CFolderLayer * CBaseLayer::Root() const
{
	return nullptr;
}

inline CSharedProject * CBaseLayer::RootProject() const {
	return Root()->RootProject();
}
