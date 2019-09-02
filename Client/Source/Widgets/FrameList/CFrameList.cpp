/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 6/14/2019 7:33:28 PM
 */

#include "CFrameList.h"
#include <qevent.h>
#include <qpainter.h>
#include <qshortcut.h>
#include <qgraphicsitem.h>
#include <qgraphicswidget.h>
#include <qgraphicslinearlayout.h>
#include "Projects/CProject.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Events/Events.h"
#include "Graphics/GraphicsFrame/CGraphicsFrame.h"
#include "Graphics/GraphicsScrubBar/CGraphicsScrubBar.h"

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
	m_rows = new QGraphicsLinearLayout(Qt::Vertical);
	m_rows->setSpacing(0);
	m_rows->setContentsMargins(0, 0, 0, 0);

	m_widget = new QGraphicsWidget();
	connect(m_widget, &QGraphicsWidget::widthChanged, this, &CFrameList::SceneWidthChanged);
	m_widget->setLayout(m_rows);
	scene()->addItem(m_widget);

	m_scrubbar = new CGraphicsScrubBar(m_widget);
	m_playhead = new QGraphicsRectItem(QRectF(QPointF(0, 0), frame_bounds.size() - QSize(1, 0)), m_scrubbar);
	m_playline = new QGraphicsLineItem(m_playhead->rect().center().x(), m_playhead->rect().height(), m_playhead->rect().center().x(), m_widget->boundingRect().height() + m_playhead->rect().height(), m_playhead);
	m_playhead->setBrush(QColor(180, 0, 0, 128));
	m_playhead->setPen(QColor(180, 0, 0));
	m_playline->setPen(m_playhead->pen());
	m_rows->addItem(m_scrubbar);
	scene()->addItem(m_scrubbar);

	centerOn(0, 0);

	QShortcut* addframe = new QShortcut(Qt::Key_F7, this);
	QShortcut* addhold = new QShortcut(Qt::Key_F5, this);

	connect(addframe, &QShortcut::activated, [this, addframe] { ShortcutEvent(addframe); });
	connect(addhold, &QShortcut::activated, [this, addhold] { ShortcutEvent(addhold); });

	CProject::Listen([this](CProjectEvent* Event) { ProjectEvent(Event); });
	CFrame::Listen([this](CFrameEvent* Event) { FrameEvent(Event); });
	CLayer::Listen([this](CLayerEvent* Event) { LayerEvent(Event); });
}

void CFrameList::ProjectEvent(CProjectEvent * Event)
{
	switch (Event->Action())
	{
	case CProjectEvent::ActiveProject:
		// Clear all items except the first (For our scrub bar)
		// For some odd reason, a linearlayout is always the first item even though nothing is added before the scrub bar
		while (m_rows->count() > 2)
		{
			auto item = (QGraphicsLinearLayout*)m_rows->itemAt(m_rows->count() - 1);
			while (item->count())
				delete item->itemAt(item->count() - 1);
			delete item;
		}
		m_scrubbar->SetWidth();

		if (!Event->Project())
			return;

		// For now it will just send a load of Layer Add events to get them all in
		for (auto layer : Event->Project()->Layers())
		{
			CLayerEvent e(layer, CLayerEvent::Add);
			LayerEvent(&e);
		}
		break;
	case CProjectEvent::ActiveFrame:
		m_playhead->setPos(Event->Project()->ActiveFrame() * 8, m_playhead->y());
		break;
	}
}

void CFrameList::FrameEvent(CFrameEvent * Event)
{
	size_t index = Event->Frame()->Index();
	auto row = (QGraphicsLinearLayout*)m_rows->itemAt(Event->Frame()->Layer()->Index() + 2);
	if (!row)
		return;

	switch (Event->Action())
	{
	case CFrameEvent::Replace:
	{
		if (auto frame = (CGraphicsFrame*)row->itemAt(Event->Frame()->Index()))
			frame->SetFrame(Event->Frame());
		else
		{
			frame = new CGraphicsFrame(Event->Frame());
			row->insertItem(index, frame);
			scene()->addItem(frame);
		}
		break;
	}
	case CFrameEvent::Remove:
		break;
	case CFrameEvent::Insert:
	{
		if (!(m_rows->count() - 1))
			break;
		auto frame = new CGraphicsFrame(Event->Frame(), row);
		row->insertItem(index, frame);
		scene()->addItem(frame);
		SceneWidthChanged();
		break;
	}
	case CFrameEvent::Add:
	{
		if (!(m_rows->count() - 1))
			break;
		auto frame = new CGraphicsFrame(Event->Frame(), row);
		row->addItem(frame);
		scene()->addItem(frame);
		SceneWidthChanged();
		break;
	}
	}
}

void CFrameList::SceneWidthChanged()
{
	m_rows->updateGeometry();
	if (m_scrubbar)
		m_scrubbar->SetWidth(m_rows->contentsRect().width());
}

void CFrameList::LayerEvent(CLayerEvent * Event)
{
	size_t index = Event->OldIndex();

	int count = m_rows->count();
	for (int i = 0; i < count; i++)
	{
		auto item = m_rows->itemAt(i);
		auto geom = item->geometry();
	}

	switch (Event->Action())
	{
	case CLayerEvent::Moved:
	{
		auto oldrow = (QGraphicsLinearLayout*)m_rows->itemAt(index + 1), newrow = (QGraphicsLinearLayout*)m_rows->itemAt(Event->Layer()->Index() + 2);
		if (!oldrow || !newrow)
			break;
		m_rows->removeItem(oldrow);
		m_rows->insertItem(Event->Layer()->Index() + 1, oldrow);
	}
	case CLayerEvent::Remove:
	{
		if (index + 2 > m_rows->count())
			break;
		auto row = (QGraphicsLinearLayout*)m_rows->itemAt(index + 1);
		if (!row)
			break;

		m_rows->removeItem(row);
		while (row->count())
			delete row->itemAt(row->count() - 1);
		delete row;
		m_rows->activate();
		break;
	}
	case CLayerEvent::Add:
		auto row = new QGraphicsLinearLayout(Qt::Horizontal);
		row->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
		row->setSpacing(0);
		row->setContentsMargins(0, 0, 0, 0);

		if (index >= Event->Layer()->Project()->Layers().size() - 1)
			m_rows->addItem(row);
		else
			m_rows->insertItem(index + 1, row);

		for (auto frame : Event->Layer()->Frames())
		{
			auto gframe = new CGraphicsFrame(frame, row);
			row->addItem(gframe);
			scene()->addItem(gframe);
		}
		break;
	}
}

void CFrameList::Scrub(QMouseEvent * Event)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	static bool drag = false;
	QPoint p = mapToScene(Event->pos()).toPoint();

	switch (Event->type())
	{
	case QEvent::MouseButtonPress:
		if (m_scrubbar->boundingRect().contains(p)) // itemAt fails at weird times, so we just check if it's within our scrub bar
			drag = true;
		break;
	case QEvent::MouseButtonRelease:
		drag = false;
		break;
	}

	if (drag)
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

void CFrameList::ShortcutEvent(const QShortcut * Shortcut)
{
	QKeySequence key = Shortcut->key();
	if (key == Qt::Key_F7 || key == Qt::Key_F6 || key == Qt::Key_F5)
	{
		if (CProject* proj = CProject::ActiveProject())
			if (CLayer* layer = proj->ActiveLayer())
				layer->AddFrame(key == Qt::Key_F7);
	}
}

void CFrameList::mousePressEvent(QMouseEvent * Event) {
	Scrub(Event);
}
void CFrameList::mouseReleaseEvent(QMouseEvent * Event) {
	Scrub(Event);
}
void CFrameList::mouseMoveEvent(QMouseEvent * Event) {
	Scrub(Event);
}
