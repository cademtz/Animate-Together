/*	Description:
 *		Provides a class to manage and network users via a user ID.
 *		User IDs are the same as a handle, being used to attrubute users to data and efficiently network them
 *		Valid IDs are always non-zero and only necessary to be unique between sessions.
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

class CUser
{
public:
	CUser(QString Name) : m_name(Name)
	{
		static unsigned NextId = 1; // Top secret unique user ID generator
		m_id = NextId;
		NextId++;
	}

	inline QString Name() const { return m_name; }
	inline unsigned ID() const { return m_id; }

private:
	QString m_name;
	unsigned m_id;
};

#endif // CUser_H