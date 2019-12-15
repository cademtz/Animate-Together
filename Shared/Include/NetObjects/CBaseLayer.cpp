/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:10:42 PM
 */

#include "CBaseLayer.h"
#include "CFolderLayer.h"
#include "Shared/CSharedProject.h"

CBaseLayer::Listeners_t CBaseLayer::m_listeners;

CBaseLayer::CBaseLayer(EType Type, CSharedProject * Proj, SerialStream& Data) : CNetObject(Data), m_type(Type)
{
	unsigned parent;
	uint8_t type;
	Data >> parent >> m_name >> type;

	assert(m_type == type);
	m_parent = Proj->FindLayer<CFolderLayer>(parent, Layer_Folder);
}

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

int CBaseLayer::IndexOf(const CNetObject & Obj) const
{
	//for (CBaseFrame* frame : m_frames)
		//if (frame->Handle() == Obj.Handle())
			//
	return -1;
}

void CBaseLayer::SerializeCustom(CSerialize & Data) const {
	Data.Add(m_parent->Handle(), m_name, m_type);
}