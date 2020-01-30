/*	Description:
 *		A layout class that makes managing the layer graphics easier
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 8:01:36 PM
 */

#ifndef CLayerLayout_H
#define CLayerLayout_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicslinearlayout.h>
#include "Graphics/GraphicsTypes.h"
#include "Graphics/FrameLayout/CFrameLayout.h"

class CCustomEvent;
class QGraphicsScene;

class CLayerLayout : public QGraphicsLinearLayout
{
public:
	CLayerLayout(QGraphicsLayoutItem* Parent = 0);

	CFrameLayout* Layer(int Index) { return (CFrameLayout*)itemAt(Index); }
	int IndexOf(CFrameLayout* Layer);
	int type() { return (int)EGraphicsType::LayerLayout; }

	void HandleEvent(CCustomEvent* Event, QGraphicsScene* Scene);
};

#endif // CLayerLayout_H