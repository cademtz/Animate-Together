#include "CRasterFrame.h"
#include "Projects/CProject.h"

/*	Description:
 *		(See header)
 *
 *	Author:
 *	Created: 10/7/2019 2:42:39 PM
 */

CRasterFrame::CRasterFrame(CLayer * Layer, bool Hold) : CFrame(Layer, Raster, Hold)
{
	//if (!Hold)
		//m_pixmap = QPixmap(Layer->Project()->Dimensions());
}