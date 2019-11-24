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
	CFolderLayer* folder;
	if (Type() == Layer_Folder)
		folder = (CFolderLayer*)this;
	else
		folder = Parent();
	while (folder->Parent())
		folder = folder->Parent();
	return folder;
}

CSharedProject * CBaseLayer::RootProject() const {
	return Root()->_Project();
}

inline int CBaseLayer::Index() const {
	return Parent()->IndexOf(this->Handle());
}
