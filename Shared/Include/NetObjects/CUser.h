/*	Description:
 *		Provides a class to manage and network user data
 *
 *	Author: Hold on!
 *	Created: 11/8/2019 11:09:55 PM
 */

#ifndef CUser_H
#define CUser_H
#ifdef _WIN32
#pragma once
#endif

#include <qstring.h>
#include "CNetObject.h"

class CUser : public CNetObject
{
public:

	// - Creates an entirely new networkable user
	CUser(QString Name) : m_name(Name) { }

	inline QString Name() const { return m_name; }

private:
	QString m_name;
};

#endif // CUser_H