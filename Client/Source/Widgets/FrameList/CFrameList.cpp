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
#include <qgraphicslinearlayout.h>
#include "Projects/CProject.h"
#include "Projects/Layer/CLayer.h"
#include "Projects/Events/Events.h"
#include "Graphics/GraphicsFrame/CGraphicsFrame.h"
#include "Graphics/GraphicsScrubBar/CGraphicsScrubBar.h"

const QRectF frame_bounds(0, 0, 9, 18);
const QRectF circ_size(0, 0, 5, 5);

void CFrameList::SceneWidthChanged()
{
	m_rows->updateGeometry();
	if (m_scrubbar)
		m_scrubbar->SetWidth(m_rows->contentsRect().width());
}

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
	m_rows->insertItem(0, m_scrubbar);
	scene()->addItem(m_scrubbar);

	centerOn(0, 0);

	CProject::Listen([this](CProjectEvent* Event) { ProjectEvent(Event); });
	QShortcut* addframe = new QShortcut(Qt::Key_F7, this);
	connect(addframe, &QShortcut::activated, [] {
		CProject::ActiveProject()->ActiveLayer()->AddFrame(true);
		});
	QShortcut* addhold = new QShortcut(Qt::Key_F6, this);
	connect(addhold, &QShortcut::activated, [] {
		CProject::ActiveProject()->ActiveLayer()->AddFrame(false);
		});

	CFrame::Listen([this](CFrameEvent* Event) {
		FrameEvent(Event);
		});

	CLayer::Listen([this](CLayerEvent* Event) {
		LayerEvent(Event);
		});
}

void CFrameList::FrameEvent(CFrameEvent * Event)
{
	size_t index = Event->Frame()->Index();
	CLayer* layer = Event->Frame()->Layer();
	auto row = (QGraphicsLinearLayout*)m_rows->itemAt(Event->Frame()->Layer()->Index() + 1);
	if (!row)
		return;

	switch (Event->Action())
	{
	case CFrameEvent::Replace:
	{
		auto frame = (CGraphicsFrame*)row->itemAt(Event->Frame()->Index());
		if (frame)
		{
			frame->SetFrame(Event->Frame());
			scene()->update(frame->geometry());
		}
		else
		{
			CGraphicsFrame* frame = new CGraphicsFrame(Event->Frame());
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
		auto frame = new CGraphicsFrame(Event->Frame());
		row->insertItem(index, frame);
		scene()->addItem(frame);
		SceneWidthChanged();
		break;
	}
	case CFrameEvent::Add:
	{
		if (!(m_rows->count() - 1))
			break;
		auto frame = new CGraphicsFrame(Event->Frame());
		row->addItem(frame);
		scene()->addItem(frame);
		SceneWidthChanged();
		break;
	}
	}
}

void CFrameList::LayerEvent(CLayerEvent * Event)
{
	switch (Event->Action())
	{
	case CLayerEvent::Add:
		auto col = new QGraphicsLinearLayout(Qt::Horizontal);
		col->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
		col->setSpacing(0);
		col->setContentsMargins(0, 0, 0, 0);
		m_rows->addItem(col);
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

void CFrameList::ProjectEvent(CProjectEvent * Event)
{
	switch (Event->Action())
	{
	case CProjectEvent::ActiveFrame:
		m_playhead->setPos(Event->Project()->ActiveFrame() * 8, m_playhead->y());
		break;
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
