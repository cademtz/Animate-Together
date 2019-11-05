/*	Description:
 *		Holds a single stroke made from user input
 *
 *	Author: Hold on!
 *	Created: 11/4/2019 10:26:02 AM
 */

#ifndef CStroke_H
#define CStroke_H
#ifdef _WIN32
#pragma once
#endif

#include <list>
#include <qpoint.h>

class CStroke
{
public:
	typedef std::list<struct PointStruct> Points_t;
	CStroke(const Points_t& Points) : m_points(Points) { }
	CStroke() { }

	inline Points_t Points() { return m_points; }

private:
	struct PointStruct
	{
		QPoint point;
		qreal scale;
	};

	Points_t m_points;
};

#endif // CStroke_H