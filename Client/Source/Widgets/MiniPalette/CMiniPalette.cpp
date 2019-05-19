/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 4/18/2019 8:32:19 PM
 */

#include "CMiniPalette.h"
#include <qevent.h>
#include <qpainter.h>
#include <qshortcut.h>
#include "Interface/ColorPicker/ColorPicker.h"

std::list<std::function<void(CMiniPalette*)>> CMiniPalette::m_listeners;

const QRect CMiniPalette::firstRect()
{
	static QRect r(0, 0, 16, 16);
	return r;
}

const QRect CMiniPalette::secondRect()
{
	static QRect r(width() - firstRect().width() - 1, height() - firstRect().height() - 1, firstRect().height(), firstRect().width());
	return r;
}

CMiniPalette::CMiniPalette(QWidget * Parent) : QWidget(Parent), m_first(0, 0, 0), m_second(255, 255, 255)
{
	setFixedSize(25, 25);
	
	QShortcut* x = new QShortcut(Qt::Key_X, this);
	QShortcut* d = new QShortcut(Qt::Key_D, this);
	connect(x, &QShortcut::activated, this, &CMiniPalette::swap);
	connect(d, &QShortcut::activated, this, &CMiniPalette::reset);
}

void CMiniPalette::swap()
{
	QColor c = m_second;
	m_second = m_first, m_first = c;
	update();
	PalletteEvent();
}

void CMiniPalette::reset()
{
	m_first = QColor(0, 0, 0), m_second = QColor(255, 255, 255);
	update();
	PalletteEvent();
}

void CMiniPalette::pickColor(bool First)
{
	QColor* color = First ? &m_first : &m_second;
	ColorPicker::Open(*color, [this, color](ColorPicker* c) {
		*color = c->m_color;
		update();
		PalletteEvent();
		});
}

void CMiniPalette::PalletteEvent()
{
	for (auto func : m_listeners)
		func(this);
}

void CMiniPalette::paintEvent(QPaintEvent * e)
{
	QPainter paint(this);

	QRect a = firstRect(), b = secondRect();

	paint.fillRect(b, m_second);
	paint.setPen(Qt::black);
	paint.drawRect(b);
	paint.setPen(Qt::white);
	b.moveTopLeft(b.topLeft() + QPoint(1, 1));
	b.setSize(b.size() - QSize(2, 2));
	paint.drawRect(b);

	paint.fillRect(a, m_first);
	paint.setPen(Qt::black);
	paint.drawRect(a);
	paint.setPen(Qt::white);
	a.moveTopLeft(a.topLeft() + QPoint(1, 1));
	a.setSize(a.size() - QSize(2, 2));
	paint.drawRect(a);
}

void CMiniPalette::mousePressEvent(QMouseEvent * e)
{
	QColor* color;
	if ((firstRect().contains(e->pos()) && (color = &m_first)) || (secondRect().contains(e->pos()) && (color = &m_second)))
		pickColor(color == &m_first);
		/*ColorPicker::Open(*color, [this, color](ColorPicker* c) {
				*color = c->m_color;
				update();
				PalletteEvent();
			});*/
}