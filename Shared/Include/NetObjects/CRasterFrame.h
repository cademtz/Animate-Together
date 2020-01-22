/*	Description:
 *		A frame which holds a raster image. Used in raster layers.
 *
 *	Author: Hold on!
 *	Created: 1/21/2020 10:15:29 PM
 */

#ifndef CRasterFrame_H
#define CRasterFrame_H
#ifdef _WIN32
#pragma once
#endif

#include "CBaseFrame.h"

class CRasterFrame : public CBaseFrame
{
public:
	CRasterFrame(bool IsKey) : CBaseFrame(IsKey) { }

	bool IsEmpty() const override { return true; }

protected:
	// TO DO: Carry over data to be implemented in future (such as the actual raster image)
	CBaseFrame* NewClone() const override { return new CRasterFrame(IsKey()); }
};

#endif // CRasterFrame_H