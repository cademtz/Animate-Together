/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 4/14/2019 2:59:33 PM
 */

#include "CColorBox.h"
#include <qpainter.h>
#include <qevent.h>

CColorBox::CColorBox(QColor Color, QWidget * Parent) : QWidget(Parent) {
	UpdateColor(Color);
}

void CColorBox::paintEvent(QPaintEvent * Event)
{
	QPainter paint(this);
	paint.drawImage(0, 0, m_gradient);

	paint.setPen(Qt::white);
	paint.drawEllipse(m_cursor, 5, 5);
	paint.setPen(Qt::black);
	paint.drawEllipse(m_cursor, 6, 6);
}

void CColorBox::mousePressEvent(QMouseEvent * Event) {
	UpdateColor(Event->pos());
}
void CColorBox::mouseMoveEvent(QMouseEvent * Event) {
	UpdateColor(Event->pos());
}
void CColorBox::mouseReleaseEvent(QMouseEvent * Event) {
	UpdateColor(Event->pos());
}
void CColorBox::resizeEvent(QResizeEvent * Event) {
	UpdateColor(m_color);
}

int CColorBox::FromMode(EColorMode Mode) const
{
	switch (Mode)
	{
	case Mode_Hue:
		return m_base.hue();
	case Mode_Sat:
		return m_base.saturation();
	case Mode_Val:
		return m_base.value();
	}
	return 0;
}

QColor CColorBox::ColorAt(int X, int Y)
{
	if (X < 0)
		X = 0;
	else if (X > width())
		X = width();
	if (Y < 0)
		Y = 0;
	else if (Y > height())
		Y = height();

	float fx = float(X) / width(), fy = float(Y) / height();
	float h = m_base.hueF(), s = m_base.saturationF(), v = m_base.valueF();

	if (m_up)
		fy = 1.f - fy;
	switch (m_vert)
	{
	case Mode_Hue:
		h = fy; break;
	case Mode_Sat:
		s = fy; break;
	case Mode_Val:
		v = fy; break;
	}

	if (m_left)
		fx = 1.f - fx;
	switch (m_hor)
	{
	case Mode_Hue:
		h = fx; break;
	case Mode_Sat:
		s = fx; break;
	case Mode_Val:
		v = fx; break;
	}

	return QColor::fromHsvF(h, s, v);
}

QPoint CColorBox::PosAt(const QColor& Color)
{
	QPoint pos;
	float val;
	
	switch (m_vert)
	{
	case Mode_Hue:
		val = Color.hueF(); break;
	case Mode_Sat:
		val = Color.saturationF(); break;
	case Mode_Val:
		val = Color.valueF();
	}
	if (m_up)
		val = 1.f - val;
	pos.setY(val * height());
	
	switch (m_hor)
	{
	case Mode_Hue:
		val = Color.hueF(); break;
	case Mode_Sat:
		val = Color.saturationF(); break;
	case Mode_Val:
		val = Color.valueF();
	}
	if (m_left)
		val = 1.f - val;
	pos.setX(val* width());

	return pos;
}

void CColorBox::UpdateColor(QPoint Cursor)
{
	int w = width(), h = height();
	if (Cursor.x() < 0)
		Cursor.setX(0);
	else if (Cursor.x() > w)
		Cursor.setX(w);
	if (Cursor.y() < 0)
		Cursor.setY(0);
	else if (Cursor.y() > h)
		Cursor.setY(h);

	m_color = ColorAt(m_cursor.x(), m_cursor.y());
	QPoint oldcursor = m_cursor;
	m_cursor = Cursor;
	update(QRect(m_cursor - QPoint(6, 6), m_cursor + QPoint(6, 6)));
	update(QRect(oldcursor - QPoint(6, 6), oldcursor + QPoint(6, 6)));

	if (m_callback)
		m_callback(this);
}

void CColorBox::UpdateColor(QColor Color)
{
	if (Color.hue() == -1)
		Color = QColor::fromHsv(0, Color.saturation(), Color.value(), Color.alpha());
	m_base = m_color = Color;
	m_cursor = PosAt(m_base);

	m_gradient = QImage(size(), QImage::Format::Format_RGB888);
	for (int y = 0; y < height(); y++)
		for (int x = 0; x < width(); x++)
			m_gradient.setPixel(x, y, ColorAt(x, y).rgb());
	update();

	if (m_callback)
		m_callback(this);
}

void CColorBox::UpdateCursor()
{

}
