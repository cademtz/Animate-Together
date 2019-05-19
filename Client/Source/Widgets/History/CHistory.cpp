/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/6/2019 10:06:10 PM
 */

#include "CHistory.h"
#include <qpainter.h>
#include <qstatictext.h>
#include <qevent.h>
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

CHistory::CHistory(MainWindow * Window, QWidget * Parent) : QWidget(Parent), m_parent(Window)
{
	setStyleSheet("background-color: rgb(30, 30, 30); color: rgb(200, 200, 200)");
}

void CHistory::paintEvent(QPaintEvent * Event)
{
	QPainter paint = QPainter(this);

	static QStaticText name("History");
	paint.fillRect(0, 0, width(), name.size().height() + 6, QColor(50, 50, 50));
	paint.setPen(m_focused ? QColor(0, 128, 255) : QColor(15, 15, 15));
	paint.drawRect(0, 0, width() - 1, height() - 1);
	paint.setPen(QColor(200, 200, 200));
	paint.drawStaticText(6, 3, name);

	if (CProject* proj = m_parent->GetActiveProj())
	{
		size_t y = name.size().height() + 7;
		for (auto action : proj->GetUndos().GetActions())
		{
			QString text = action->WasUndone() ? "- " : "+ ";
			paint.drawText(6, y += name.size().height(), text + action->GetTypeStr());
		}
	}
}