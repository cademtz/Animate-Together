/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/28/2019 1:04:17 AM
 */

#include "CCanvas.h"
#include <qevent.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qbitmap.h>
#include <qmath.h>
#include <qapplication.h>
#include <qshortcut.h>

#include "Interface/MainWindow/MainWindow.h"
#include "Interface/EditBrush/EditBrush.h"
#include "Widgets/MiniPalette/CMiniPalette.h"
#include "Widgets/ToolBar/CToolBar.h"
#include "Projects/CProject.h"
#include "Brushes/Brushes.h"

void CCanvas::stepBrush(int Step)
{
	BrushScale_t* brush = getBrush();
	if (brush->min + Step >= 1 && brush->min + Step <= 100)
		brush->min += Step;
	if (brush->max + Step >= 1 && brush->max + Step <= 100)
		brush->max += Step;
	updateCursor(1);
}

CCanvas::CCanvas(QWidget* Parent)
	: QWidget(Parent)
	, m_tool(e_tool::Brush)
	, m_color(Qt::black)
	, m_brush(m_color)
	, m_brushScale({ 2, 5 })
	, m_eraserScale({ 1, 10 })
	, m_pen(m_brush, 1.0, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)
	, m_drawmode(e_drawmode::up)
	, m_scroll(0, 0)
	, m_dragging(false)
{
	QPalette pal = palette();
	pal.setColor(QPalette::Background, QColor(25, 25, 25));
	setPalette(pal);

	setAutoFillBackground(true);
	setAttribute(Qt::WA_TabletTracking);

	QShortcut* dec = new QShortcut(Qt::Key::Key_BracketLeft, this),
		*inc = new QShortcut(Qt::Key::Key_BracketRight, this);
	connect(dec, &QShortcut::activated, [this] { stepBrush(-1); });
	connect(inc, &QShortcut::activated, [this] { stepBrush(1); });

	CLayer::Listen([this](CLayerEvent* e) { UpdateCanvas(); });
	CToolBar::Listen([this](e_tool tool) { setTool(tool); });
	CPalette::Listen([this](CPaletteEvent* p) { setColor(p->Palette()->First()); });
	CUndoStack::Listen([this](const CUndoAction* Undo) { UpdateCanvas(); });

	CProject::Listen([this](CProjectEvent* e)
		{
			switch (e->Action())
			{
			case CProjectEvent::ActiveProject:
				m_proj = e->Project();
				update();
				break;
			case CProjectEvent::ActiveFrame:
				UpdateCanvas();
				break;
			}
		});
	CFrame::Listen([this](CFrameEvent* e)
		{
			switch (e->Action())
			{
			case CFrameEvent::Replace:
				if (e->Frame() && e->Frame() == e->Frame()->Layer()->ActiveFrame())
					UpdateCanvas();
			case CFrameEvent::Remove:
				// Very dangerous code. Fix as soon as any sort of undo/redo for frames is possible
				if (e->OldIndex() == e->Frame()->Layer()->Project()->ActiveFrame())
					UpdateCanvas();
			}
		});

	updateCursor(1);
}

void CCanvas::UpdateCanvas()
{
	if (m_drawmode == e_drawmode::up)
		m_drawmode = e_drawmode::update;
	update(QRect(m_scroll, m_pixcanvas.size()));
}
void CCanvas::setTool(e_tool Tool) {
	m_tool = Tool;
	updateCursor(1);
}
void CCanvas::setTabletDevice(QTabletEvent * event) {
	updateCursor(event->pressure());
}

void CCanvas::tabletEvent(QTabletEvent * event)
{
	CLayer* layer;
	if (!m_proj)
		return;

	if (m_tool == e_tool::Eyedrop) // Eyedropper is independent of active layers
	{
		if (event->type() == QEvent::TabletPress || event->type() == QEvent::TabletMove)
		{
			QRect r = m_imgcanvas.rect();
			r.moveTopLeft(m_scroll);
			if (r.contains(event->pos())) // Only pick a color if it's within the canvas
				m_proj->Palette().SetFirst(m_imgcanvas.pixel(event->pos() - m_scroll));
		}
		return;
	}

	if (!(layer = m_proj->ActiveLayer()) || !layer->IsVisible())
		return;

	switch (event->type()) {
	case QEvent::TabletPress:
		TabletPress(event, layer);
		break;
	case QEvent::TabletMove:
		TabletMove(event, layer);
		break;
	case QEvent::TabletRelease:
		TabletRelease(event, layer);
		update();
		break;
	default:
		break;
	}
	event->accept();
}

void CCanvas::paintEvent(QPaintEvent * event)
{
	if (!m_proj)
		return;

	QPainter paint(this);
	paint.fillRect(QRect(m_scroll, m_proj->Dimensions()), Brushes::checkers);
	paint.end();

	switch (m_drawmode)
	{
	case e_drawmode::down: // We must redraw all layers to show the buffer correctly
	{
		paint.begin(this);
		const std::deque<CLayer*>& layers = m_proj->Layers();
		for (auto it = layers.rbegin(); it != layers.rend(); it++)
		{
			if (!(*it)->IsVisible())
				continue;
			bool showBuffer = *it == m_proj->ActiveLayer();
			if (showBuffer && m_tool == e_tool::Eraser)
			{
				QImage mask = m_buffer.createAlphaMask();
				mask.invertPixels();
				QRegion reg(QBitmap::fromImage(mask));
				reg.translate(m_scroll);
				paint.setClipRegion(reg);
			}
			if ((*it)->Pixmap())
				paint.drawPixmap(m_scroll, *(*it)->Pixmap());
			if (showBuffer)
			{
				if (m_tool == e_tool::Brush)
					paint.drawImage(m_scroll, m_buffer);
				else if (m_tool == e_tool::Eraser)
					paint.setClipRegion(event->region());
			}
		}
		paint.end();
		break;
	}
	case e_drawmode::released: // BRUH
	{
		m_drawmode = e_drawmode::up;
		CLayer* Layer;
		if (!(Layer = m_proj->ActiveLayer()) || !Layer->Pixmap())
			break;

		if (Layer->Pixmap())
		{
			int l = m_buffer.width(), r = 0, t = m_buffer.height(), b = 0;
			for (int y = 0; y < m_buffer.height(); ++y) // Find opaque area of buffer
			{
				QRgb *row = (QRgb*)m_buffer.scanLine(y);
				bool rowFilled = false;
				for (int x = 0; x < m_buffer.width(); ++x)
				{
					if (qAlpha(row[x]))
					{
						rowFilled = true, r = std::max(r, x);
						if (l > x) // Only search for new right bound from here
							l = x, x = r;
					}
				}
				if (rowFilled)
				{
					t = std::min(t, y);
					b = y;
				}
			}
			r++, b++;

			paint.begin(Layer->Pixmap());
			switch (m_tool)
			{
			case e_tool::Brush:
				m_proj->Undos().Push(new CUndoStroke(*Layer->Pixmap(), l, t, r - l, b - t));
				paint.drawImage(l, t, m_buffer.copy(l, t, r - l, b - t));
				break;
			case e_tool::Eraser:
				m_proj->Undos().Push(new CUndoErase(*Layer->Pixmap(), l, t, r - l, b - t));
				paint.setCompositionMode(QPainter::CompositionMode_Source);
				paint.setClipRegion(QBitmap::fromImage(m_buffer.createAlphaMask()));
				paint.fillRect(l, t, r - l, b - t, QColor(0, 0, 0, 0));
			}
			paint.end();
		}
	}
	case e_drawmode::update:
	{
		m_drawmode = e_drawmode::up;

		// Create the final image to optimize redrawing while there's no change
		if (m_pixcanvas.size() != m_proj->Dimensions())
			m_pixcanvas = QPixmap(m_proj->Dimensions());
		m_pixcanvas.fill(Qt::transparent);

		paint.begin(&m_pixcanvas);
		const std::deque<CLayer*>& layers = m_proj->Layers();
		for (auto it = layers.rbegin(); it != layers.rend(); it++)
			if ((*it)->IsVisible() && (*it)->Pixmap())
				paint.drawPixmap(0, 0, *(*it)->Pixmap());
		paint.end();
		m_imgcanvas = m_pixcanvas.toImage();
	}
	case e_drawmode::up:
	{
		paint.begin(this);
		paint.drawPixmap(m_scroll, m_pixcanvas);
		paint.end();
		break;
	}
	}
}

void CCanvas::mousePressEvent(QMouseEvent * Event)
{
	switch (Event->button())
	{
	case Qt::RightButton:
		EditBrush::Open(Event->globalPos(), &(m_tool == e_tool::Eraser ? m_eraserScale : m_brushScale));
		break;
	case Qt::MiddleButton:
		m_dragging = true;
		m_pmouse = Event->pos();
		break;
	default:
		tabletEvent(new QTabletEvent(QEvent::TabletPress, Event->localPos(), Event->globalPos(), QTabletEvent::Stylus, QTabletEvent::Pen, 1, 0, 0, 0, 0, 0, Event->modifiers(), 0));
	}
}
void CCanvas::mouseMoveEvent(QMouseEvent * Event)
{
	if (m_dragging)
	{
		m_scroll += Event->pos() - m_pmouse, m_pmouse = Event->pos();
		update();
	}
	else
		tabletEvent(new QTabletEvent(QEvent::TabletMove, Event->localPos(), Event->globalPos(), QTabletEvent::Stylus, QTabletEvent::Pen, 1, 0, 0, 0, 0, 0, Event->modifiers(), 0));
}
void CCanvas::mouseReleaseEvent(QMouseEvent * Event)
{
	switch (Event->button())
	{
	case Qt::MiddleButton:
		m_dragging = false;
		break;
	default:
		tabletEvent(new QTabletEvent(QEvent::TabletRelease, Event->localPos(), Event->globalPos(), QTabletEvent::Stylus, QTabletEvent::Pen, 1, 0, 0, 0, 0, 0, 0, 0));
	}
}
void CCanvas::wheelEvent(QWheelEvent * Event)
{
	QPoint delta = Event->angleDelta();
	if (!(Event->modifiers() & Qt::ShiftModifier))
		delta /= 2;
	m_scroll += delta;
	update();
}

void CCanvas::paintPixmap(QPainter & painter, QTabletEvent * event)
{
	qreal maxPenRadius = pressureToWidth(1.0);
	painter.setRenderHint(QPainter::Antialiasing);

	switch (event->device())
	{
	case QTabletEvent::Puck:
	case QTabletEvent::FourDMouse:
	{
		const QString error(tr("This input device is not supported."));
#if QT_CONFIG(statustip)
		QStatusTipEvent status(error);
		QApplication::sendEvent(this, &status);
#else
		qWarning() << error;
#endif
	}
	break;
	default:
	{
		const QString error(tr("Unknown tablet device - treating as stylus"));
#if QT_CONFIG(statustip)
		QStatusTipEvent status(error);
		QApplication::sendEvent(this, &status);
#else
		qWarning() << error;
#endif
	}
	Q_FALLTHROUGH();
	case QTabletEvent::Stylus:
		if (m_tool == e_tool::Eraser)
			m_pen.setColor(Qt::black);
		painter.setPen(m_pen);
		painter.drawLine(lastPoint.pos - m_scroll, event->posF() - m_scroll);
		update(QRect(lastPoint.pos.toPoint(), event->pos()).normalized()
			.adjusted(-maxPenRadius, -maxPenRadius, maxPenRadius, maxPenRadius));
		break;
	}
}

qreal CCanvas::pressureToWidth(qreal pressure) {
	return m_tool == e_tool::Eraser ? m_eraserScale.getSize(pressure) : m_brushScale.getSize(pressure);
}

void CCanvas::updateBrush(const QTabletEvent * event)
{
	int hue, saturation, value, alpha;
	m_color.getHsv(&hue, &saturation, &value, &alpha);

	int vValue = int(((event->yTilt() + 60.0) / 120.0) * 255);
	int hValue = int(((event->xTilt() + 60.0) / 120.0) * 255);

	m_pen.setWidthF(pressureToWidth(event->pressure()));

	if (event->pointerType() == QTabletEvent::Eraser) {
		m_brush.setColor(Qt::white);
		m_pen.setColor(Qt::white);
		m_pen.setWidthF(event->pressure() * 10 + 1);
	}
	else {
		m_brush.setColor(m_color);
		m_pen.setColor(m_color);
	}
}

void CCanvas::updateCursor(qreal Pressure)
{
	QCursor cursor;

	if (m_tool == e_tool::Brush || m_tool == e_tool::Eraser)
	{
		qreal size = getBrush()->getSize(Pressure);
		QPixmap pixmap;
		if (size > 4)
		{
			pixmap = QPixmap(size + 1, size + 1);
			pixmap.fill(Qt::transparent);
			QPointF center = QPointF(size / 2, size / 2);
			QPainter paint(&pixmap);
			paint.setPen(Qt::white);
			paint.drawEllipse(center, (size / 2) - 1, (size / 2) - 1);
			paint.setPen(Qt::black);
			paint.drawEllipse(center, size / 2, size / 2);
		}
		else
		{
			pixmap = QPixmap(18, 18);
			int center = 17 / 2;
			pixmap.fill(Qt::transparent);
			QPainter paint(&pixmap);
			QColor glow = QColor(255, 255, 255, 50);
			paint.fillRect(center, 0, 3, 5, glow);
			paint.fillRect(0, center, 5, 3, glow);
			paint.fillRect(center, center + 5, 3, 5, glow);
			paint.fillRect(center + 5, center, 5, 3, glow);
			paint.setPen(Qt::black);
			paint.drawLine(center + 1, 1, center + 1, 4);
			paint.drawLine(1, center + 1, 4, center + 1);
			paint.drawLine(center + 1, center + 5, center + 1, center + 9);
			paint.drawLine(center + 5, center + 1, center + 9, center + 1);
		}
		setCursor(QCursor(pixmap));
		return;
	}
	setCursor(QCursor());
}

void CCanvas::TabletPress(QTabletEvent * Event, CLayer * Layer)
{
	if (!Layer->Pixmap() || m_drawmode != e_drawmode::up)
		return;

	m_drawmode = e_drawmode::down;
	if (Event->button() == Qt::MouseButton::RightButton)
	{
		EditBrush::Open(Event->globalPos(), &(m_tool == e_tool::Eraser ? m_eraserScale : m_brushScale));
		return;
	}

	switch (m_tool)
	{
	case e_tool::Brush:
	case e_tool::Eraser:
	{
		if (m_buffer.size() != Layer->Pixmap()->size())
			m_buffer = QImage(Layer->Pixmap()->size(), QImage::Format_ARGB32);
		m_buffer.fill(QColor(0, 0, 0, 0));

		lastPoint.pos = Event->posF();
		lastPoint.pressure = Event->pressure();
		lastPoint.rotation = Event->rotation();

		QPainter painter(&m_buffer);
		paintPixmap(painter, Event);
		break;
	}
	}
}

void CCanvas::TabletMove(QTabletEvent * Event, CLayer * Layer)
{
	if (!Layer->Pixmap())
#ifndef Q_OS_IOS
		updateCursor(Event->pressure());
#endif
	if (m_drawmode == e_drawmode::down) {
		updateBrush(Event);
		QPainter painter(&m_buffer);
		paintPixmap(painter, Event);
		lastPoint.pos = Event->posF();
		lastPoint.pressure = Event->pressure();
		lastPoint.rotation = Event->rotation();
	}
}

void CCanvas::TabletRelease(QTabletEvent * Event, CLayer * Layer)
{
	if (m_drawmode != e_drawmode::down || Event->buttons() != Qt::NoButton)
		return;
	m_drawmode = e_drawmode::released;
	UpdateCanvas();
}

BrushScale_t * CCanvas::getBrush()
{
	return m_tool == e_tool::Eraser ? &m_eraserScale : &m_brushScale;
}