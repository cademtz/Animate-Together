/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 4/14/2019 2:59:33 PM
 */

#include "CColorBox.h"
#include <qpainter.h>
#include <qevent.h>

CColorBox::CColorBox(QColor Color, QWidget * Parent) : QWidget(Parent), m_colormode(ColorMode_SaturationValue)
{
	updateColor(Color);
}

void CColorBox::updateColormode(e_colormode Colormode)
{
	m_colormode = Colormode;
	update();
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
	updateColor(Event->pos());
}
void CColorBox::mouseMoveEvent(QMouseEvent * Event) {
	updateColor(Event->pos());
}
void CColorBox::mouseReleaseEvent(QMouseEvent * Event) {
	updateColor(Event->pos());
}
void CColorBox::resizeEvent(QResizeEvent * Event) {
	updateColor(m_color);
}

void CColorBox::updateColor(QPoint Cursor)
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

	m_color = QColor::fromHsv(m_base.hue(), 255 * (float(Cursor.x()) / w), 255 - 255 * (float(Cursor.y()) / h));
	QPoint oldcursor = m_cursor;
	m_cursor = Cursor;
	update(QRect(m_cursor - QPoint(6, 6), m_cursor + QPoint(6, 6)));
	update(QRect(oldcursor - QPoint(6, 6), oldcursor + QPoint(6, 6)));

	if (m_callback)
		m_callback(this);
}

void CColorBox::updateColor(QColor Color)
{
	if (Color.hue() == -1)
		Color = QColor::fromHsv(0, m_base.saturation(), m_base.value(), m_base.alpha());
	m_base = m_color = Color;
	switch (m_colormode)
	{
	case ColorMode_SaturationValue:
		m_cursor.setX((float(m_base.saturation()) / 255) * width());
		m_cursor.setY(((float(m_base.value()) - 255) / -255) * height());
		break;
	default:
		m_cursor = QPoint(0, 0);
	}

	m_gradient = QImage(size(), QImage::Format::Format_RGB888);
	for (int y = 0; y < height(); y++)
		for (int x = 0; x < width(); x++)
			m_gradient.setPixel(x, y, QColor::fromHsv(m_base.hue(), 255 * (float(x) / width()), 255 - 255 * (float(y) / height())).rgb());
}