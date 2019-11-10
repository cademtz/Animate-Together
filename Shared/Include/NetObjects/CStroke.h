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
#include "CNetObject.h"
#include "CBrush.h"

class CStroke : public CNetObject
{
	struct PointStruct
	{
		QPoint point;
		float pressure;
	};

public:
	typedef QList<PointStruct> Points_t;

	CStroke(const Points_t& Points, const CNetObject& BrushHandle) : m_points(Points) { }

	inline Points_t Points() { return m_points; }
	inline CStroke& operator+(const CStroke& Other)
	{
		m_points.append(Other.m_points);
		m_brush = Other.m_brush;
		return *this;
	}
	inline CStroke& operator+=(const CStroke& Other) { return *this + Other; }

private:
	Points_t m_points;
	const CBrush* m_brush;
};

#endif // CStroke_H