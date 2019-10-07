/*	Description:
 *		A frame that holds raster images, specifically for raster layers.
 *
 *	Author: Hold on!
 *	Created: 8/24/2019 5:40:37 PM
 */

#ifndef CRasterFrame_H
#define CRasterFrame_H
#ifdef _WIN32
#pragma once
#endif

#include <qpixmap.h>
#include "CFrame.h"

class CRasterFrame : public CFrame
{
	QPixmap m_pixmap;
	QPoint m_pos;

public:
	CRasterFrame(CLayer* Layer, bool Hold = false);
	CRasterFrame(CRasterFrame& Frame, CLayer* Layer) : CRasterFrame(Frame) { m_layer = Layer; }

	inline QPixmap* Pixmap() { return IsKey() ? &m_pixmap : Parent<CRasterFrame>()->Pixmap(); }
	inline QPoint Pos() { return IsKey() ? m_pos : Parent<CRasterFrame>()->Pos(); }

	// - Incorrect on raster due to good performance being near impossible in multiplayer
	bool IsEmpty()
	{
		if (!IsKey())
			return Parent()->IsEmpty();
		if (m_pixmap.isNull())
			return false;
		return false;
	}
};


#endif // CRasterFrame_H