/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:10:42 PM
 */

#include "CBaseLayer.h"
#include "CBaseFrame.h"
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

CFolderLayer * CBaseLayer::Root()
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

CSharedProject * CBaseLayer::RootProject() {
	return Root()->Project();
}

int CBaseLayer::IndexOf(const CNetObject & Obj) const
{
	for (int i = 0; i < m_frames.size(); i++)
		if (m_frames[i]->Handle() == Obj.Handle())
			return i;
	return -1;
}

CBaseFrame * CBaseLayer::LastKey(int Index)
{
	if (!m_frames.size())
		return nullptr;

	for (auto it = m_frames.begin() + Index; it != m_frames.begin(); it--)
		if ((*it)->IsKey())
			return *it;
	return m_frames.front();
}

void CBaseLayer::InsertFrame(int Index, CBaseFrame * Frame)
{
	Frame->SetParent(this);
	m_frames.insert(Index, Frame);
}

void CBaseLayer::SerializeCustom(CSerialize & Data) const {
	Data.Add(m_parent->Handle(), m_name, m_type);
}