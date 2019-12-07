/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:10:42 PM
 */

#include "CBaseLayer.h"
#include "CFolderLayer.h"
#include "CNetObject.h"

CBaseLayer::Listeners_t CBaseLayer::m_listeners;

int CBaseLayer::Index() const {
	return Parent()->IndexOf(this->Handle());
}

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

// Extremely gross, needs a whole separate method for doing it a not-easily-screwed-up way
CBaseLayer::CBaseLayer(EType Type, SerialStream& Data)
	: CNetObject(*(unsigned*)Data.data), m_type(Type)
{
	CNetObject obj = 0;
	int type;
	Data >> obj >> type >> m_name;
}
