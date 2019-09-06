/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 5/27/2019 10:38:53 AM
 */

#include "CLayerArea.h"
#include <qpainter.h>
#include <qevent.h>

#include "Projects/CProject.h"
#include "Interface/EditLayer/EditLayer.h"

void CLayerArea::DrawItems(QPainter & paint)
{
	CProject* proj = CProject::ActiveProject();
	if (!proj)
		return;

	QRect region = GetLayerRegion();
	for (auto layer : proj->Layers())
	{
		DrawItem(paint, layer, region);
		region.moveTop(region.top() + region.height());
	}
}

QRect iconRegion(const QRect & region)
{
	constexpr int size = 9, margin = 3;
	return QRect(region.x() + margin, region.y() + (region.height() / 2 - size / 2), size, size);
}

void CLayerArea::DrawItem(QPainter & paint, const CLayer * Layer, const QRect & region)
{
	QPen old = paint.pen();
	if (Layer == CProject::ActiveProject()->ActiveLayer())
	{
		paint.setPen(QColor(100, 100, 100));
		paint.drawRect(region);
	}

	QRect icon = iconRegion(region);
	if (Layer->IsVisible())
		paint.fillRect(icon, QColor(100, 100, 100));
	paint.setPen(QColor(128, 128, 128));
	paint.drawRect(icon);

	paint.setPen(old);
	int tx = icon.x() + icon.width();
	paint.drawText(region.x() + tx + 8, region.y(), region.width() - tx - 8, region.height(), Qt::AlignVCenter, QString::fromStdString(Layer->Name()));
}

CLayer* CLayerArea::SelectLayer(QMouseEvent * Event)
{
	static bool bAssign; // Choose either on or off and STICK WITH IT! No rapid flickering!
	if (CProject* proj = CProject::ActiveProject())
	{
		QRect region = GetLayerRegion();
		for (auto layer : proj->Layers())
		{
			if ((m_drag == e_drag::null || m_drag == e_drag::hide) && iconRegion(region).contains(Event->pos()))
			{
				if (m_drag == e_drag::null) // Have we toggled 'hide' for the first time?
				{
					layer->SetVisible(!layer->IsVisible()); // Then toggle and only allow setting to that state
					bAssign = layer->IsVisible(), m_drag = e_drag::hide;
				}
				else if (layer->IsVisible() != bAssign)
					layer->SetVisible(bAssign); // Only set to that state if the layer isn't the same
				return 0;
			}
			else if (region.contains(Event->pos()))
			{
				if (m_drag == e_drag::null)
				{
					proj->SetActiveLayer(layer);
					m_drag = e_drag::layer;
				}
				else if (m_drag == e_drag::layer)
					update();
				return layer;
			}
			region.moveTop(region.top() + region.height());
		}
	}
	return 0;
}

CLayerArea::CLayerArea(QWidget * Parent) : QWidget(Parent)
{
	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(30, 30, 30));
	pal.setBrush(QPalette::Foreground, QColor(200, 200, 200));
	setPalette(pal);

	CLayer::Listen([this](CLayerEvent* Event) { update(); }); // BAD IDEA, listener can crash if the instance has been destroyed. Maybe a way to un-listen? (deafen haha)
}

QRect CLayerArea::GetLayerRegion() {
	return QRect(0, 0, width() - 1, 15);
}

void CLayerArea::resizeEvent(QResizeEvent * Event)
{
	resize(((QWidget*)parent())->size());
	if (CProject* proj = CProject::ActiveProject())
		setFixedHeight(GetLayerRegion().height() * proj->Layers().size() + 1);
}

void CLayerArea::paintEvent(QPaintEvent * Event)
{
	CProject* proj = CProject::ActiveProject();
	if (!proj)
		return;

	setFixedHeight(GetLayerRegion().height() * proj->Layers().size() + 1);

	QPainter paint = QPainter(this);
	DrawItems(paint);

	if (m_drag != e_drag::layer)
		return;

	QRect region = GetLayerRegion(), fill;
	for (int i = 0; i < proj->Layers().size(); i++)
	{
		if (proj->Layers()[i] != proj->ActiveLayer() && region.contains(m_cursor))
		{
			QPoint p = m_cursor - region.topLeft();
			QSize s = QSize(region.width(), 5);
			if (p.y() < region.height() / 2)
				fill = QRect(region.topLeft(), s), m_insert = i;
			else
				fill = QRect(region.bottomLeft() - QPoint(0, s.height()), QSize(region.width(), 5)), m_insert = i + 1;
			break;
		}
		region.moveTop(region.top() + region.height());
	}

	if (m_insert == -1)
		return;

	QPoint p = QPoint(m_cursor.x(), m_cursor.y() - region.height() / 2);
	if (p.y() < 0)
		region.moveTop(0);
	else if (p.y() + region.height() > height())
		region.moveTop(height() - region.height());
	else
		region.moveTop(p.y());

	QColor bkg = palette().window().color();
	bkg.setAlpha(128);
	paint.fillRect(region, bkg);
	paint.setPen(QColor(0, 128, 255, 128));
	paint.drawRect(region);
	paint.fillRect(fill, QColor(0, 128, 255));
}

void CLayerArea::mouseMoveEvent(QMouseEvent * Event)
{
	m_cursor = Event->pos();
	if (m_pressed)
		SelectLayer(Event);
}

void CLayerArea::mousePressEvent(QMouseEvent * Event)
{
	m_cursor = Event->pos();
	m_pressed = true;
	SelectLayer(Event);
}

void CLayerArea::mouseReleaseEvent(QMouseEvent * Event)
{
	e_drag drag = m_drag;
	m_pressed = false, m_drag = e_drag::null;
	if (drag == e_drag::layer) // Were we previously dragging a layer?
	{
		CProject* proj;
		if (m_insert != -1 && (proj = CProject::ActiveProject()))
		{
			proj->ShiftLayer(proj->ActiveLayer(), m_insert);
			m_insert = -1;
		}
		update();
	}
}

void CLayerArea::mouseDoubleClickEvent(QMouseEvent * Event)
{
	m_cursor = Event->pos();
	if (CLayer* layer = SelectLayer(Event))
		EditLayer::Open(CProject::ActiveProject(), layer);
}