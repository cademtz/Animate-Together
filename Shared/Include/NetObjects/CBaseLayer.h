/*	Description:
 * 		A shared class that contains the internal functions and variables needed for all layer types.
 *
 *		Details:
 *			All frames stored in a layer are assumed to be owned by it.
 *			Destruction of a layer will destroy all its frames.
 *
 *	Author: Hold on!
 *	Created: 11/6/2019 5:02:50 PM
 */

#ifndef CBaseLayer_H
#define CBaseLayer_H
#ifdef _WIN32
#pragma once
#endif

#include <qstring.h>
#include "CNetObject.h"
#include "CBaseFrame.h"
#include "Shared/CEventHandler.h"
#include "Shared/CNetEvent.h"
#include "Shared/CSerialize.h"

class CSharedProject;
class CFolderLayer;
class CBaseFrame;
class CBaseLayer;

typedef QList<CBaseLayer*> LayerList_t;
typedef QList<const CBaseLayer*> ConstLayerList_t;

class CBaseLayer : public CNetObject, public CEventHandler<CBaseLayerMsg>
{
public:
	enum EType : uint8_t
	{
		Layer_Null = 0, // - Reserved for class to describe not-yet deserialized layer data
		Layer_Folder,
		Layer_Raster,
		Layer_Vector,
		Layer_Stick,
		Layer_Audio
	};
	CBaseLayer(EType Type, CSharedProject* Proj, SerialStream& Data);
	virtual ~CBaseLayer()
	{
		for (CBaseFrame* frame : m_frames)
			delete frame;
	}

	int Index() const;
	inline EType Type() const { return (EType)m_type; }
	CFolderLayer* Root();
	const CFolderLayer* Root() const;
	CSharedProject* RootProject();
	const CSharedProject* RootProject() const;
	inline bool IsRoot() const { return !m_parent; }
	inline CFolderLayer* Parent() { return m_parent; }
	inline const CFolderLayer* Parent() const { return m_parent; }
	inline void SetParent(CFolderLayer* Parent) { m_parent = Parent; }
	inline QString Name() const { return m_name; }
	inline void SetName(const QString& Name) { m_name = Name; }
	inline bool IsVisible() const { return m_visible; }
	inline void SetVisible(bool Visible) { m_visible = Visible; }
	inline const CNetObject& Owner() const { return m_owner; }
	inline void SetOwner(const CNetObject& Owner) { m_owner = Owner; }
	inline bool IsPrivate() const { return m_owner.Handle() != 0; }

	// ----- Frame functions ----- //

	const QList<CBaseFrame*>& Frames() { return m_frames; }
	const QList<const CBaseFrame*>& Frames() const { return (QList<const CBaseFrame*>&)m_frames; }
	CBaseFrame* LastKey(int Index);
	const CBaseFrame* LastKey(int Index) const { return LastKey(Index); }
	void InsertFrame(int Index, CBaseFrame* Frame);
	bool RemoveFrame(CBaseFrame* Frame) { return m_frames.removeOne(Frame); }
	inline void AppendFrame(CBaseFrame* Frame) { InsertFrame(m_frames.size(), Frame); }

	int IndexOf(const CBaseFrame* Frame) const { return IndexOf(Frame->Handle()); }
	int IndexOf(const CNetObject& Obj) const;
	CBaseFrame* FindFrame(unsigned Handle);
	const CBaseFrame* FindFrame(unsigned Handle) const;

protected:
	CBaseLayer(EType Type, CFolderLayer* Parent = nullptr) : m_type(Type), m_parent(Parent) { }

	// - Subtypes of CBaseLayer must implement these if they need additional networked data

	virtual void SerializeLayer(CSerialize& Data) const { }
	virtual void DeserializeLayer(SerialStream& Data) { }

private:
	void SerializeCustom(CSerialize& Data) const override;

	uint8_t m_type;
	CFolderLayer* m_parent = 0;
	QString m_name;
	bool m_visible = true;
	CNetObject m_owner = 0;
	QList<CBaseFrame*> m_frames;
};

#endif // CBaseLayer_H