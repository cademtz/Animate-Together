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

#include <qlist.h>
#include <qpoint.h>
#include "CNetMsg.h"

class CStroke
{
	struct PointStruct
	{
		QPoint point;
		qreal pressure;
	};

public:
	typedef QList<PointStruct> Points_t;
	CStroke(const Points_t& Points) : m_points(Points) { }
	CStroke() { }

	inline Points_t Points() { return m_points; }
	inline CStroke& operator+(const CStroke& Other)
	{
		m_points.append(Other.m_points);
		return *this;
	}
	inline CStroke& operator+=(const CStroke& Other) { return *this + Other; }

private:
	Points_t m_points;
};

#endif // CStroke_H