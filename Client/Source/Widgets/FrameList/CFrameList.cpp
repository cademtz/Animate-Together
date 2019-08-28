/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 6/14/2019 7:33:28 PM
 */

#include "CFrameList.h"
#include <qshortcut.h>
#include <qpainter.h>
#include <qevent.h>
#include <qgraphicsitem.h>
#include <qgraphicswidget.h>
#include "Projects/CProject.h"
#include "Projects/Events/Events.h"
#include "Graphics/GraphicsFrame/CGraphicsFrame.h"

const QRectF frame_bounds(0, 0, 9, 18);
const QRectF circ_size(0, 0, 5, 5);

CFrameList::CFrameList(QWidget * Parent) : QGraphicsView(Parent)
{
	setContentsMargins(0, 0, 0, 0);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	setVerticalScrollBarPolicy(horizontalScrollBarPolicy());
	setAlignment(Qt::AlignTop | Qt::AlignLeft);

	setScene(new QGraphicsScene(this));
	m_grid = new QGraphicsGridLayout();
	m_grid->setSpacing(0);
	m_grid->setContentsMargins(0, 0, 0, 0);

	for (int r = 0; r < 3; r++)
	{
		for (int c = 0; c < 110; c++)
		{
			static bool filled = true;
			if (c > 1 && c % 2)
				filled = rand() % 2;

			CGraphicsFrame* frame = new CGraphicsFrame(filled ? CFrame::Key : CFrame::Hold);
			m_grid->addItem(frame, r, c, Qt::AlignLeft);
			scene()->addItem(frame);
		}
	}

	QGraphicsWidget* graphics_list = new QGraphicsWidget();
	graphics_list->setLayout(m_grid);
	scene()->addItem(graphics_list);

	m_scrubbar = scene()->addRect(QRectF(0, 0, scene()->width(), 18), QPen(Qt::transparent), QColor(50, 50, 50));
	m_playhead = new QGraphicsRectItem(QRectF(QPointF(0, 0), frame_bounds.size() - QSize(1, 0)), m_scrubbar);
	m_playline = new QGraphicsLineItem(m_playhead->rect().center().x(), m_playhead->rect().height(), m_playhead->rect().center().x(), sceneRect().height(), m_playhead);
	m_playhead->setBrush(QColor(180, 0, 0, 128));
	m_playhead->setPen(QColor(180, 0, 0));
	m_playline->setPen(m_playhead->pen());

	centerOn(0, 0);

	CProject::Listen([this](CProjectEvent* Event) { ProjectEvent(Event); });
	QShortcut* addframe = new QShortcut(Qt::Key_F7, this);
	connect(addframe, &QShortcut::activated, [] {
		CProject::ActiveProject()->ActiveLayer()->AddFrame(true);
		});

	/*CFrame::Listen([this](CFrameEvent* Event) {
		// On add, find the frame object in the needed position and set to key frame type
		// if none exists, add one in place
		switch (Event->Action())
		{
		case CFrameEvent::Replace:
			size_t index = Event->Frame()->Index();
			int x = index * 8, y = m_scrubbar->y() + m_scrubbar->boundingRect().height() + frame_bounds.height()* Event->Frame()->Layer()->Index();
			if (QGraphicsItem* frame = scene()->itemAt(x, y, transform()))
				if (frame->type() == CGraphicsFrame::GraphicsType)
					((CGraphicsFrame*)frame)->SetState(Event->Frame()->State());
			update();
			break;
		case CFrameEvent::Remove:
		case CFrameEvent::Add:
			break;
		}
		});*/

	/*CLayer::Listen([this](CLayerEvent* Event) {
		switch (Event->Action())
		{
		case CLayerEvent::Add:
			QRectF bruh = frame_bounds;
			bruh.moveTop(m_scrubbar->y() + m_scrubbar->boundingRect().height() + frame_bounds.height() * Event->Layer()->Index());
			bruh.moveTop(frame_bounds.height());

			auto& frames = Event->Layer()->Frames();
			for (size_t i = 0; i < frames.size(); i++)
			{
				QGraphicsItem* frame = new CGraphicsFrame(frames[i]->State());
				frame->setPos(bruh.topLeft());
				scene()->addItem(frame);

				bruh.moveLeft(bruh.left() + bruh.width() - 1);
			}
		}
		});*/
}

void CFrameList::Scrub(QMouseEvent * Event)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	QPoint p = mapToScene(Event->pos()).toPoint();
	if (m_scrubbar->contains(p)) // itemAt fails at weird times, so we just check if it's within our scrub bar
	{
		QPoint newpos((p.x() / 8) * 8, m_playhead->y());
		if (newpos.x() < 0 || newpos.x() + m_playhead->rect().width() > m_scrubbar->boundingRect().right())
			return;

		int frame = p.x() / 8;
		if (frame == proj->ActiveFrame())
			return;

		m_playhead->setPos(newpos);
		proj->SetActiveFrame(frame);
	}
}

void CFrameList::ProjectEvent(CProjectEvent * Event)
{
	switch (Event->Action())
	{
	case CProjectEvent::ActiveFrame:
		int frame = (m_playhead->x() - sceneRect().x()) / 8;
		if (frame != CProject::ActiveProject()->ActiveFrame())
			m_playhead->setPos(frame * 8, m_playhead->y());
		break;
	}
}

void CFrameList::mousePressEvent(QMouseEvent * Event)
{
	Scrub(Event);
}

void CFrameList::mouseMoveEvent(QMouseEvent * Event)
{
	Scrub(Event);
}
