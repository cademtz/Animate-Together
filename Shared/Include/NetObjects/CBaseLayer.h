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

class CSharedProject;
class CFolderLayer;

class CBaseLayer : public CNetObject
{
public:
	enum EType : uint8_t
	{
		Layer_Folder,
		Layer_Raster,
		Layer_Vector,
		Layer_Stick,
		Layer_Audio
	};

	inline EType Type() const { return m_type; }
	CFolderLayer* Root() const;
	CSharedProject* RootProject() const;
	inline CFolderLayer* Parent() const { return m_parent; }
	inline void SetParent(CFolderLayer* Parent) { m_parent = Parent; }
	inline QString Name() const { return m_name; }
	inline void SetName(const QString& Name) { m_name = Name; }

protected:
	CBaseLayer(EType Type, CFolderLayer* Parent = nullptr) : m_type(Type), m_parent(Parent) { }
	virtual ~CBaseLayer() { }

private:
	EType m_type;
	CFolderLayer* m_parent = 0;
	QString m_name;
};

#endif // CBaseLayer_H