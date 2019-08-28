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
#include "Projects/CProject.h"

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

CMiniPalette::CMiniPalette(QWidget * Parent) : QWidget(Parent)
{
	setFixedSize(25, 25);
	
	QShortcut* x = new QShortcut(Qt::Key_X, this);
	QShortcut* d = new QShortcut(Qt::Key_D, this);
	connect(x, &QShortcut::activated, this, &CMiniPalette::Swap);
	connect(d, &QShortcut::activated, this, &CMiniPalette::SetToDefault);

	CPalette::Listen([this](CPaletteEvent* e) { update(); });
}

void CMiniPalette::Swap()
{
	CProject * proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	proj->Palette().Swap();
	update();
	PalletteEvent();
}

void CMiniPalette::SetToDefault()
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	proj->Palette().SetDefaultPalette();
	update();
	PalletteEvent();
}

void CMiniPalette::PickColor(bool First)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	QColor color = First ? proj->Palette().First() : proj->Palette().Second();
	ColorPicker::Open(color, [this, proj, First](ColorPicker* c)
		{
			First ? proj->Palette().SetFirst(c->m_color) : proj->Palette().SetSecond(c->m_color);
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
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	QPainter paint(this);

	QRect a = firstRect(), b = secondRect();

	paint.fillRect(b, proj->Palette().Second());
	paint.setPen(Qt::black);
	paint.drawRect(b);
	paint.setPen(Qt::white);
	b.moveTopLeft(b.topLeft() + QPoint(1, 1));
	b.setSize(b.size() - QSize(2, 2));
	paint.drawRect(b);

	paint.fillRect(a, proj->Palette().First());
	paint.setPen(Qt::black);
	paint.drawRect(a);
	paint.setPen(Qt::white);
	a.moveTopLeft(a.topLeft() + QPoint(1, 1));
	a.setSize(a.size() - QSize(2, 2));
	paint.drawRect(a);
}

void CMiniPalette::mousePressEvent(QMouseEvent * e)
{
	CProject* proj;
	if (!(proj = CProject::ActiveProject()))
		return;

	bool first = false, second = false;
	if (firstRect().contains(e->pos()))
		first = true;
	else if (secondRect().contains(e->pos()))
		second = true;

	if (first || second) // Either will be true if one of the rects was clicked
		PickColor(first);
}