/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 4/6/2019 1:52:36 PM
 */

#include "CToolBar.h"
#include <qpushbutton.h>
#include "Widgets/MiniPalette/CMiniPalette.h"

std::list<ToolEvent_t> CToolBar::m_listeners;
CMiniPalette* CToolBar::m_palette = nullptr;

class CTool : public QPushButton
{
	e_tool m_type = e_tool::null;
public:
	CTool(QWidget* Parent, e_tool Type, const char* Icon) : QPushButton(Parent), m_type(Type)
	{
		setStyleSheet("\
			QPushButton { border: none; background-color: rgb(30, 30, 30); } \
			QPushButton:hover { background-color: rgb(60, 60, 60); } \
			QPushButton:checked { background-color: rgb(25, 25, 25); border: 1px solid rgb(75, 75, 75); }");
		setIcon(QIcon(Icon));
		setIconSize(icon().availableSizes()[0]);
		setFixedSize(25, 25);
		setCheckable(true);
		setAutoExclusive(true);
		connect(this, &CTool::toggled, this, &CTool::SelectEvent);
	}
protected:
	void SelectEvent(bool val)
	{
		if (val)
			CToolBar::ToolEvent(m_type);
	}
};

CToolBar::CToolBar(QWidget * Parent) : QWidget(Parent)
{
	setStyleSheet(".QWidget { background-color: rgb(30, 30, 30); }");
	setFixedHeight(35);

	layout = new QGridLayout(this);
	layout->setSpacing(5);
	layout->setMargin(5);
	layout->setAlignment(Qt::AlignLeft | Qt::AlignTop);

	if (!m_palette)
		m_palette = new CMiniPalette(this);

	m_brush = new CTool(this, e_tool::Brush, ":/Icons/Brush.png");
	m_eraser = new CTool(this, e_tool::Eraser, ":/Icons/Eraser.png");
	m_eyedrop = new CTool(this, e_tool::Eyedrop, ":/Icons/Eyedropper.png");
	m_rect = new CTool(this, e_tool::Rect, ":/Icons/Rectangle.png");

	m_brush->setChecked(true);

	layout->addWidget(m_palette, 0, 0);
	layout->addWidget(m_brush, 0, 1);
	layout->addWidget(m_eraser, 0, 2);
	layout->addWidget(m_eyedrop, 0, 3);
	layout->addWidget(m_rect, 0, 4);

	m_brush->setShortcut(Qt::Key_B);
	m_eraser->setShortcut(Qt::Key_E);
	m_eyedrop->setShortcut(Qt::Key_I);
	m_rect->setShortcut(Qt::Key_R);
}

void CToolBar::ToolEvent(e_tool Event)
{
	for (auto fn : m_listeners)
		fn(Event);
}