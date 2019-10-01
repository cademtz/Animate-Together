/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/6/2019 10:06:10 PM
 */

#include "CHistoryPanel.h"
#include <qpainter.h>
#include <qstatictext.h>
#include <qevent.h>
#include "Projects/CProject.h"

CHistoryPanel::CHistoryPanel(QWidget * Parent) : QWidget(Parent) {
	setStyleSheet("background-color: rgb(30, 30, 30); color: rgb(200, 200, 200)");
}

void CHistoryPanel::paintEvent(QPaintEvent * Event)
{
	QPainter paint = QPainter(this);

	static QStaticText name("History");
	paint.fillRect(0, 0, width(), name.size().height() + 6, QColor(50, 50, 50));
	paint.setPen(QColor(200, 200, 200));
	paint.drawStaticText(6, 3, name);

	if (CProject* proj = CProject::ActiveProject())
	{
		size_t y = name.size().height() + 7;
		for (auto action : proj->Undos().GetActions())
		{
			QString text = action->Undone() ? "- " : "+ ";
			paint.drawText(6, y += name.size().height(), text + action->TypeStr());
		}
	}
}