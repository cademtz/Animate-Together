/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 8:03:35 PM
 */

#include <qgraphicsscene.h>
#include "CLayerLayout.h"
#include "Projects/CProject.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Events/Events.h"

#define PAD_FRAMES 100

CLayerLayout::CLayerLayout(QGraphicsLayoutItem * Parent)
	: QGraphicsLinearLayout(Qt::Vertical, Parent)
{
	setSpacing(0);
	setContentsMargins(0, 0, 0, 0);
}

int CLayerLayout::IndexOf(CFrameLayout * Layer)
{
	for (int i = 0; i < count(); i++)
		if (itemAt(i) == Layer)
			return i;
	return -1;
}

void CLayerLayout::HandleEvent(CCustomEvent * Event, QGraphicsScene* Scene)
{
	switch (Event->Type())
	{
	case CCustomEvent::FrameEvent:
	{
		CFrameEvent* e = (CFrameEvent*)Event;
		int layer = e->Frame()->Layer()->Index();
		Layer(layer)->HandleFrameEvent(e, Scene);
		break;
	}
	case CCustomEvent::LayerEvent:
	{
		CLayerEvent* e = (CLayerEvent*)Event;
		if (!e->Layer() || !e->Project() || e->Layer()->Project() != CProject::ActiveProject())
			break;

		size_t index = e->OldIndex(), size = e->Project()->Layers().size(), newindex = e->Layer()->Index();

		switch (e->Action())
		{
		case CLayerEvent::Moved:
		{
			auto layer = Layer(index);

			removeItem(layer);
			if (newindex >= size)
				addItem(layer);
			else if (newindex > index)
				insertItem(newindex + 1, layer);
			else if (newindex < index)
				insertItem(newindex, layer);
			break;
		}
		case CLayerEvent::Remove:
		{
			auto layer = Layer(index);

			while (layer->count())
				delete layer->Frame(layer->count() - 1);
			delete layer;
			break;
		}
		case CLayerEvent::Add:
		{
			auto layer = new CFrameLayout(this);

			if (index >= size)
				addItem(layer);
			else
				insertItem(index, layer);

			for (int i = 0; i < e->Layer()->Frames().size() + PAD_FRAMES; i++)
			{
				auto gframe = new CGraphicsFrame();
				layer->addItem(gframe);
				Scene->addItem(gframe);
			}
			break;
		}
		}
	}
	}
}
