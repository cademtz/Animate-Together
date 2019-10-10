/*	Description:
 *		A layout class that makes managing the frame graphics easier
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 7:41:00 PM
 */

#ifndef CFrameLayout_H
#define CFrameLayout_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicslinearlayout.h>
#include "Graphics/GraphicsTypes.h"
#include "Graphics/GraphicsFrame/CGraphicsFrame.h"

class CLayerLayout;
class CFrameEvent;
class QGraphicsScene;

class CFrameLayout : public QGraphicsLinearLayout
{
public:
	CFrameLayout(CLayerLayout* Parent);

	inline CLayerLayout* Parent() { return (CLayerLayout*)parentLayoutItem(); }
	CGraphicsFrame* Frame(int Index) { return (CGraphicsFrame*)itemAt(Index); }
	int IndexOf(CGraphicsFrame* Frame);
	int Index();

	void HandleFrameEvent(CFrameEvent* Event, QGraphicsScene* Scene);

	int type() { return (int)e_graphicstype::FrameLayout; }
};

#endif // CFrameLayout_H