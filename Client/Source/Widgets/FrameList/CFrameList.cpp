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

#define PAD_FRAMES 100

CFrameList::CFrameList(QWidget * Parent) : QGraphicsView(Parent)
{
	setContentsMargins(0, 0, 0, 0);
	setFrameStyle(QFrame::NoFrame);
	
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setAlignment(Qt::AlignTop | Qt::AlignLeft);

	setScene(new QGraphicsScene(this));
	m_rows = new QGraphicsLinearLayout(Qt::Vertical);
	m_rows->setSpacing(0);
	m_rows->setContentsMargins(0, 0, 0, 0);

	m_widget = new QGraphicsWidget();
	connect(m_widget, &QGraphicsWidget::widthChanged, this, &CFrameList::UpdateScrub);
	m_widget->setLayout(m_rows);
	scene()->addItem(m_widget);
	scene()->setStyle(style());

	m_scrubbar = new CGraphicsScrubBar(m_widget);
	m_playhead = new QGraphicsRectItem(QRectF(QPointF(), CGraphicsFrame::m_rect.size() - QSize(1, 1)), m_scrubbar);
	m_playline = new QGraphicsLineItem(m_playhead);
	m_boxoverlay = new QGraphicsRectItem();

	m_playhead->setBrush(QColor(180, 0, 0, 128));
	m_playhead->setPen(QColor(180, 0, 0));
	m_playline->setPen(m_playhead->pen());
	m_boxoverlay->setBrush(QColor(64, 128, 255, 128));
	m_boxoverlay->setPen(QColor(0, 128, 255));

	m_rows->addItem(m_scrubbar);
	scene()->addItem(m_scrubbar);
	UpdateScrub();
	centerOn(0, 0);

	QShortcut* addframe = new QShortcut(Qt::Key_F7, this), * addhold = new QShortcut(Qt::Key_F5, this),
		* delframe = new QShortcut(Qt::Key_Delete, this);

	connect(addframe, &QShortcut::activated, [this, addframe] { ShortcutEvent(addframe); });
	connect(addhold, &QShortcut::activated, [this, addhold] { ShortcutEvent(addhold); });
	connect(delframe, &QShortcut::activated, [this, delframe] { ShortcutEvent(delframe); });

	CProject::Listen([this](CProjectEvent* Event) { ProjectEvent(Event); });
	CFrame::Listen([this](CFrameEvent* Event) { FrameEvent(Event); });
	CLayer::Listen([this](CLayerEvent* Event) { LayerEvent(Event); });
}

void CFrameList::UpdateScrub()
{
	if (m_scrubbar)
		if (CProject* proj = CProject::ActiveProject())
			m_scrubbar->SetWidth((proj->EndFrame() + 1) * 8);
	if (m_playline && m_playhead)
	{
		m_playline->setLine(
			m_playhead->rect().center().x(), m_playhead->rect().height(),
			m_playhead->rect().center().x(), m_widget->boundingRect().height());
	}
}

void CFrameList::ProjectEvent(CProjectEvent * Event)
{
	switch (Event->Action())
	{
	case CProjectEvent::ActiveProject:

		while (m_rows->count() > 1) // Don't delete first item, which is the scrub bar
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
		if (Event->Project()->IsPlaying())
			centerOn(m_playhead);
		break;
	}
}

void CFrameList::FrameEvent(CFrameEvent * Event)
{
	if (Event->Frame()->Layer()->Project() != CProject::ActiveProject())
		return;

	size_t index = Event->Frame()->Index();
	auto row = (QGraphicsLinearLayout*)m_rows->itemAt(Event->Frame()->Layer()->Index() + 1);
	if (!row)
		return;

	switch (Event->Action())
	{
	case CFrameEvent::Replace:
	{
		for (int i = -1; i <= 1; i++)
			if (auto frame = (CGraphicsFrame*)row->itemAt(index + i))
				frame->update();
		break;
	}
	case CFrameEvent::Remove:
		if (auto frame = (CGraphicsFrame*)row->itemAt(Event->OldIndex()))
		{
			if (Event->Frame()->Layer()->Frames().size())
				delete frame;
			else
				frame->setVisible(false); // To prevent the layout from automatically collapsing
		}
		break;
	case CFrameEvent::Add:
	{
		if (!(m_rows->count() - 1))
			break;

		auto frame = new CGraphicsFrame();
		if (index >= row->count())
			row->addItem(frame);
		else
			row->insertItem(index, frame);
		scene()->addItem(frame);
		break;
	}
	}
	UpdateScrub();
}

void CFrameList::LayerEvent(CLayerEvent * Event)
{
	if (!Event->Layer() || !Event->Project() ||
		Event->Layer()->Project() != CProject::ActiveProject())
		return;

	size_t index = Event->OldIndex(), count = Event->Project()->Layers().size(), newindex = Event->Layer()->Index();

	switch (Event->Action())
	{
	case CLayerEvent::Moved:
	{
		if (index + 1 >= m_rows->count())
			break;
		auto row = (QGraphicsLinearLayout*)m_rows->itemAt(index + 1);
		if (!row)
			break;

		m_rows->removeItem(row);
		if (newindex >= count - 1)
			m_rows->addItem(row);
		else if (newindex > index)
			m_rows->insertItem(newindex + 2, row);
		else if (newindex < index)
			m_rows->insertItem(newindex + 1, row);
		break;
	}
	case CLayerEvent::Remove:
	{
		if (index + 1 >= m_rows->count())
			break;
		auto row = (QGraphicsLinearLayout*)m_rows->itemAt(index + 1);
		if (!row)
			break;

		while (row->count())
			delete row->itemAt(row->count() - 1);
		delete row;
		break;
	}
	case CLayerEvent::Add:
		int count = m_rows->count();
		auto row = new QGraphicsLinearLayout(Qt::Horizontal);
		row->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
		row->setSpacing(0);
		row->setContentsMargins(0, 0, 0, 0);

		if (index >= count - 1)
			m_rows->addItem(row);
		else
			m_rows->insertItem(index + 1, row);

		for (int i = 0; i < Event->Layer()->Frames().size() + PAD_FRAMES; i++)
		{
			auto gframe = new CGraphicsFrame();
			row->addItem(gframe);
			scene()->addItem(gframe);
		}
		break;
	}
	UpdateScrub();
}

void CFrameList::MouseEvent(QMouseEvent * Event)
{
	if (CProject* proj = CProject::ActiveProject())
		proj->Pause();

	if (!Scrub(Event))
		Select(Event);
}

bool CFrameList::Scrub(QMouseEvent * Event)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return false;

	static bool drag = false;
	QPoint p = mapToScene(Event->pos()).toPoint();

	switch (Event->type())
	{
	case QEvent::MouseButtonPress:
		if (p.y() >= m_scrubbar->y() && p.y() <= m_scrubbar->boundingRect().bottom())
			drag = true;
		break;
	case QEvent::MouseButtonRelease:
		drag = false;
		update();
		break;
	}

	if (drag)
	{
		proj->DeselectFrames();

		if (p.x() > m_scrubbar->boundingRect().right()) // Clamp to last frame
		{
			int index = proj->LastFrame()->Index();
			if (index != proj->ActiveFrame())
				proj->SetActiveFrame(index);
			return true;
		}

		QPoint newpos((p.x() / 8) * 8, m_playhead->y());
		if (newpos.x() < 0)
			newpos.setX(0);
		else if (newpos.x() + m_playhead->rect().width() > m_scrubbar->boundingRect().right())
			newpos.setX(m_scrubbar->boundingRect().right() - m_playhead->boundingRect().width() + 1);

		int frame = newpos.x() / 8;
		if (frame != proj->ActiveFrame())
		{
			m_playhead->setPos(newpos);
			proj->SetActiveFrame(frame);
		}
		return true;
	}
	return false;
}

bool CFrameList::Select(QMouseEvent * Event)
{
	QPointF pos = mapToScene(Event->pos());
	m_boxselect.setSize(QSize(
		pos.x() - m_boxselect.left(),
		pos.y() - m_boxselect.top()));

	if (Event->type() == QEvent::MouseButtonRelease && m_selecting)
	{
		m_selecting = false;
		scene()->removeItem(m_boxoverlay);

		QRect rect = m_boxoverlay->rect().normalized().toRect();
		rect.moveTopLeft(rect.topLeft() + QPoint(2, 0));
		QPoint p = rect.topLeft(), end = rect.bottomRight();

		while (p.y() <= end.y())
		{
			while (p.x() <= end.x())
			{
				QGraphicsItem* item = scene()->itemAt(p, transform());
				if (item && item->type() == (int)e_graphicstype::Frame)
				{
					CGraphicsFrame* frame = (CGraphicsFrame*)item;
					frame->SelectFrame();
				}
				p.setX(p.x() + CGraphicsFrame::m_rect.width());
			}
			p.setY(p.y() + CGraphicsFrame::m_rect.height());
			p.setX(rect.left());
		}

		return false;
	}
	else if (Event->type() == QEvent::MouseButtonPress && Event->button() == Qt::MouseButton::LeftButton && !m_selecting)
	{
		if (CProject* proj = CProject::ActiveProject())
			for (auto layer : proj->Layers())
				layer->ClearSelected();

		m_selecting = true;

		m_boxselect.setTopLeft(pos.toPoint());
		m_boxselect.setSize(QSize());
		m_boxoverlay->setRect(m_boxselect);
		scene()->addItem(m_boxoverlay);
	}
	else if (!m_selecting)
		return false;
	
	QRect _rect = m_boxselect.normalized();
	if (!_rect.height())
		_rect.setHeight(1);
	if (!_rect.width())
		_rect.setWidth(1);
	QRectF rect = _rect;

	if (rect.top() < m_widget->geometry().top())
		rect.setTop(m_widget->geometry().top());
	if (rect.left() < m_widget->geometry().left())
		rect.setLeft(m_widget->geometry().left());
	if (rect.bottom() > m_widget->geometry().bottom())
		rect.setBottom(m_widget->geometry().bottom());

	QSize step = CGraphicsFrame::m_rect.size().toSize();
	rect.setLeft(floor(rect.left() / step.width()) * step.width() - 1);
	rect.setRight(ceil(rect.right() / step.width()) * step.width() - 1);
	rect.setTop(floor(rect.top() / step.height()) * step.height());
	rect.setBottom(ceil(rect.bottom() / step.height()) * step.height());

	m_boxoverlay->setRect(rect.normalized());

	return true;
}

void CFrameList::ShortcutEvent(const QShortcut * Shortcut)
{
	CProject* proj = CProject::ActiveProject();
	if (!proj)
		return;

	QKeySequence key = Shortcut->key();
	switch (key[0])
	{
	case Qt::Key_F7:
		if (CLayer* layer = proj->ActiveLayer())
			layer->AddFrame(true);
		break;
	case Qt::Key_F6:
	case Qt::Key_F5:
	{
		proj->Undos().Compound(true, "Add frames");
		bool selection = false;
		for (auto layer : proj->Layers())
		{
			if (layer->SelectedFrames().size())
			{
				selection = true;
				layer->AddFrame(false);
			}
		}
		if (selection)
			break;

		int active = proj->ActiveFrame();
		for (auto layer : proj->Layers())
			if (!active || layer->Frames().size() > active)
				layer->AddFrame(false, active);
		proj->Undos().Compound(false);
		break;
	}
	case Qt::Key_Delete:
		proj->RemoveSelectedFrames();
	}
}