/*	Description:
 * 		A shared class that contains the internal functions and variables needed for all layer types
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
#include "Shared/CEventHandler.h"
#include "Shared/CNetEvent.h"
#include "Shared/CSerialize.h"

class CSharedProject;
class CFolderLayer;
class CBaseLayer;

typedef QList<CBaseLayer*> LayerList_t;

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
	virtual ~CBaseLayer() { }

	int Index() const;
	inline EType Type() const { return (EType)m_type; }
	CFolderLayer* Root() const;
	CSharedProject* RootProject() const;
	inline bool IsRoot() { return !m_parent; }
	inline CFolderLayer* Parent() const { return m_parent; }
	inline void SetParent(CFolderLayer* Parent) { m_parent = Parent; }
	inline QString Name() const { return m_name; }
	inline void SetName(const QString& Name) { m_name = Name; }
	inline bool IsVisible() const { return m_visible; }
	inline void SetVisible(bool Visible) { m_visible = Visible; }

protected:
	CBaseLayer(EType Type, CFolderLayer* Parent = nullptr) : m_type(Type), m_parent(Parent) { }

	void SerializeCustom(CSerialize& Data) const override;

private:
	uint8_t m_type;
	CFolderLayer* m_parent = 0;
	QString m_name;
	bool m_visible = true;
};

#endif // CBaseLayer_H