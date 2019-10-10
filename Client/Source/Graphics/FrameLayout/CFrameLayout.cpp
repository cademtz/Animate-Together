/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/9/2019 8:09:14 PM
 */

#include <qgraphicsscene.h>
#include "CFrameLayout.h"
#include "Projects/CProject.h"
#include "Projects/Frame/CFrame.h"
#include "Graphics/LayerLayout/CLayerLayout.h"

CFrameLayout::CFrameLayout(CLayerLayout * Parent)
	: QGraphicsLinearLayout(Qt::Horizontal, Parent)
{
	setSpacing(0);
	setContentsMargins(0, 0, 0, 0);
	setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
}

int CFrameLayout::IndexOf(CGraphicsFrame * Frame)
{
	for (int i = 0; i < count(); i++)
		if (itemAt(i) == Frame)
			return i;
	return -1;
}

int CFrameLayout::Index() {
	return Parent()->IndexOf(this);
}

void CFrameLayout::HandleFrameEvent(CFrameEvent * Event, QGraphicsScene* Scene)
{
	if (Event->Frame()->Project() != CProject::ActiveProject())
		return;

	CLayer* layer = Event->Frame()->Layer();
	size_t index = Event->Frame()->Index();
	
	switch (Event->Action())
	{
	case CFrameEvent::Replace:
	{
		for (int i = -1; i <= 1; i++)
			if (auto frame = Frame(index))
				frame->update();
		break;
	}
	case CFrameEvent::Remove:
		if (auto frame = Frame(Event->OldIndex()))
			if (layer->Frames().size())
				delete frame;
		break;
	case CFrameEvent::Add:
	{
		auto frame = new CGraphicsFrame();
		if (index >= count())
			addItem(frame);
		else
			insertItem(index, frame);
		Scene->addItem(frame);
		break;
	}
	}
}

