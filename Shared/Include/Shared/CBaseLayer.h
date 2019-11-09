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

class CSharedProject;
class CFolderLayer;

class CBaseLayer
{
public:
	enum EType
	{
		Layer_Folder,
		Layer_Raster,
		Layer_Vector,
		Layer_Stick,
		Layer_Audio
	};

	inline EType Type() const { return m_type; }
	inline CFolderLayer* Parent() const { return m_parent; }
	CFolderLayer* Root() const;
	CSharedProject* RootProject() const;

protected:
	CBaseLayer(EType Type, CFolderLayer* Parent) : m_type(Type), m_parent(Parent) { }
	virtual ~CBaseLayer() { }

private:
	EType m_type;
	CFolderLayer* m_parent = 0;
};

#endif // CBaseLayer_H