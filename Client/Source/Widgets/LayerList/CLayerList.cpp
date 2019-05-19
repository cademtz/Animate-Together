/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/7/2019 11:24:24 PM
 */

#include "CLayerList.h"
#include <qpainter.h>
#include <qstatictext.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qevent.h>
#include "Interface/MainWindow/MainWindow.h"
#include "Interface/EditLayer/EditLayer.h"
#include "Projects/CProject.h"

void CLayerList::DrawItems(QPainter & paint)
{
	CProject* proj = m_parent->GetActiveProj();
	if (!proj)
		return;
	
	QRect region = GetLayerRegion();
	for (auto layer : proj->GetLayers())
	{
		DrawItem(paint, layer, region);
		region.moveTop(region.top() + region.height());
	}
}

QRect iconRegion(const QRect & region)
{
	constexpr int size = 9, margin = 6;
	return QRect(region.x() + region.width() - margin - size, region.y() + (region.height() / 2 - size / 2), size, size);
}

void CLayerList::DrawItem(QPainter & paint, const CLayer * Layer, const QRect & region)
{
	QPen old = paint.pen();
	if (Layer == m_parent->GetActiveProj()->GetActiveLayer())
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
	paint.drawText(region.x() + 4, region.y(), region.width() - 8, region.height(), Qt::AlignVCenter, QString::fromStdString(Layer->GetName()));
}

CLayer* CLayerList::SelectLayer(QMouseEvent * Event)
{
	static bool bAssign; // Choose either On or Off and STICK WITH IT! No rapid flickering!
	if (CProject* proj = m_parent->GetActiveProj())
	{
		QRect region = GetLayerRegion();
		for (auto layer : proj->GetLayers())
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

CLayerList::CLayerList(MainWindow * Window, QWidget * Parent) : QWidget(Parent), m_parent(Window)
{
	setObjectName("CLayerList");
	setStyleSheet("\
		QWidget#CLayerList {					\
			background: #323232;				\
			border: 1px solid black;			\
			color: rgb(200, 200, 200);			\
		}										\
		QPushButton {							\
			border: none;						\
			background-color: rgb(50, 50, 50);	\
			color: rgb(200, 200, 200);			\
		}										\
		QPushButton:hover {						\
			border: 1px solid rgb(0, 128, 255);	\
			color: rgb(255, 255, 255);			\
		}										\
		QPushButton:hover:pressed {				\
			background-color: rgb(25, 25, 25);	\
		} " + MainWindow::GetGlobalStyle()); // AAAAAAAAAAAAAAAAA

	m_name = new QLabel("Layers", this);
	m_name->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
	m_name->setStyleSheet("color: rgb(200, 200, 200);");
	m_name->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Policy::Maximum);

	m_top = new QWidget(this), m_scroll = new QScrollBar(m_top), m_layers = new QWidget(m_top);
	//m_top->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Policy::Preferred);
	m_top->setStyleSheet("background-color: rgb(30, 30, 30);");
	m_scroll->setStyleSheet(m_layers->styleSheet());
	//m_scroll->setOrientation(Qt::Vertical);
	m_scroll->setMaximum(5);
	m_ltop = new QHBoxLayout(m_top);
	m_ltop->setMargin(0);
	m_ltop->setSpacing(0);
	m_ltop->setAlignment(Qt::AlignLeft);
	m_ltop->addWidget(m_scroll);
	m_ltop->addWidget(m_layers);

	m_buttons = new QWidget(this), m_add = new QPushButton("+", m_buttons), m_del = new QPushButton("-", m_buttons);
	m_buttons->setFixedHeight(16);
	m_add->setMinimumSize(16, 16);
	m_del->setMinimumSize(16, 16);

	m_lbuttons = new QHBoxLayout(m_buttons);
	m_lbuttons->setAlignment(Qt::AlignmentFlag::AlignBottom | Qt::AlignmentFlag::AlignLeft);
	m_lbuttons->setMargin(0);
	m_lbuttons->setSpacing(0);
	m_lbuttons->addWidget(m_add);
	m_lbuttons->addWidget(m_del);

	m_layout = new QVBoxLayout(this);
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_layout->addWidget(m_name);
	m_layout->addWidget(m_top);
	m_layout->addWidget(m_buttons);

	connect(m_add, &QPushButton::released, this, &CLayerList::AddLayer);
	connect(m_del, &QPushButton::released, this, &CLayerList::DelLayer);
	m_add->setShortcut(Qt::CTRL + Qt::SHIFT + Qt::Key_N);
}

QRect CLayerList::GetListRegion()
{
	return QRect(1, fontMetrics().height() + 7, width() - 2, height() - fontMetrics().height() - 7 - m_add->height());
}

QRect CLayerList::GetLayerRegion()
{
	QRect region = GetListRegion();
	region.setHeight(15);
	return region;
}

void CLayerList::AddLayer()
{
	if (CProject* proj = m_parent->GetActiveProj())
		EditLayer::Open(proj);
}

void CLayerList::DelLayer()
{
	if (CProject* proj = m_parent->GetActiveProj())
		proj->RemoveLayer(proj->GetActiveLayer());
}

void CLayerList::paintEvent(QPaintEvent * Event)
{
	/*QPainter paint = QPainter(this);

	static QStaticText name("Layers");
	paint.fillRect(0, 0, width(), name.size().height() + 6, QColor(50, 50, 50));
	paint.setPen(m_focused ? QColor(0, 128, 255) : QColor(15, 15, 15));
	paint.drawRect(0, 0, width() - 1, height() - 1);
	paint.setPen(QColor(200, 200, 200));
	paint.drawStaticText(6, 3, name);

	DrawItems(paint);

	if (m_drag == e_drag::layer)
	{
		CProject* proj = m_parent->GetActiveProj();
		if (!proj)
			return;

		QRect region = GetLayerRegion(), fill;
		for (int i = 0; i < proj->GetLayers().size(); i++)
		{
			if (proj->GetLayers()[i] != proj->GetActiveLayer() && region.contains(m_cursor))
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
		if (p.y() < GetListRegion().y())
			region.moveTop(GetListRegion().y());
		else if (p.y() + region.height() > GetListRegion().y() + GetListRegion().height())
			region.moveTop(GetListRegion().y() + GetListRegion().height() - region.height());
		else
			region.moveTop(p.y());

		QColor bkg = palette().background().color();
		bkg.setAlpha(128);
		paint.fillRect(region, bkg);
		paint.setPen(QColor(0, 128, 255, 128));
		paint.drawRect(region);
		paint.fillRect(fill, QColor(0, 128, 255));
	}*/
}

void CLayerList::wheelEvent(QWheelEvent * Event) {
	m_scroll->setValue(m_scroll->value() - (Event->delta() / abs(Event->delta())));
}

void CLayerList::mouseMoveEvent(QMouseEvent * Event)
{
	/*m_cursor = Event->pos();
	if (m_pressed)
		SelectLayer(Event);*/
}

void CLayerList::mousePressEvent(QMouseEvent * Event)
{
	/*m_cursor = Event->pos();
	if (!GetListRegion().contains(Event->pos()))
		return;
	m_pressed = true;
	SelectLayer(Event);*/
}

void CLayerList::mouseReleaseEvent(QMouseEvent * Event)
{
	/*e_drag drag = m_drag;
	m_pressed = false, m_drag = e_drag::null;
	if (drag == e_drag::layer) // Were we previously dragging a layer?
	{
		CProject* proj;
		if (m_insert != -1 && (proj = m_parent->GetActiveProj()))
		{
			proj->ShiftLayer(proj->GetActiveLayer(), m_insert);
			m_insert = -1;
		}
		update();
	}*/
}

void CLayerList::mouseDoubleClickEvent(QMouseEvent * Event)
{
	/*m_cursor = Event->pos();
	if (CLayer* layer = SelectLayer(Event))
		EditLayer::Open(m_parent->GetActiveProj(), layer);*/
}
