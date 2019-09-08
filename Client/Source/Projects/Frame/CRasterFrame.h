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
	CRasterFrame(CLayer* Layer, bool IsHold = false) : CFrame(Layer, Raster, IsHold ? Hold : Empty) { }//m_parent = HoldParent; }

	inline QPixmap* Pixmap() { return State() == Hold ? Parent<CRasterFrame>()->Pixmap() : &m_pixmap; }
	inline QPoint Pos() { return State() == Hold ? Parent<CRasterFrame>()->Pos() : m_pos; }
};


#endif // CRasterFrame_H