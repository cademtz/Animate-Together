/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 5/11/2019 4:25:14 PM
 */

#include "CTimeline.h"
#include "Widgets/FrameList/CFrameList.h"

CTimeline::CTimeline(QWidget * Parent) : QWidget(Parent)
{
	setFixedHeight(75);
	m_layers = new QWidget(this), m_framelist = new CFrameList(this);
	
	m_layers->setMinimumSize(100, 32);
	m_layers->setFixedWidth(200);
	m_layers->setStyleSheet("background-color: rgb(40, 40, 40);");
	m_framelist->setStyleSheet(m_layers->styleSheet());
	m_framelist->setMinimumSize(m_layers->minimumSize());

	m_divide = new QFrame(this);
	m_divide->setFrameShape(QFrame::VLine);
	m_divide->setStyleSheet("color: rgb(0, 0, 0);");
	m_divide->setContentsMargins(0, 2, 0, 2);
	m_divide->setFixedWidth(2);

	m_layout = new QHBoxLayout(this);
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	m_layout->addWidget(m_layers);
	m_layout->addWidget(m_divide);
	m_layout->addWidget(m_framelist);

	//CLayer::Listen([this](CLayerEvent* e) { LayerEvent(e); });
}

void CTimeline::LayerEvent(CLayerEvent* Event)
{
	/*switch (Event->Action())
	{
	case CLayerEvent::Add:
		lists.push_back(new CFrameList(nullptr, m_frames));
		m_lframes->addWidget(lists[lists.size() - 1]);
		break;
	case CLayerEvent::Remove:
		m_lframes->removeWidget(lists[lists.size() - 1]);
		delete lists[lists.size() - 1];
		if (lists.size())
			lists.pop_back();
		break;
	}*/
}