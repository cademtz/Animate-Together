/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 8/15/2019 5:33:14 PM
 */

#include "CPalette.h"

std::list<PaletteCallback_t> CPalette::m_listeners;

void CPalette::PaletteEvent()
{
	CPaletteEvent e(this);
	for (auto fn : m_listeners)
		fn(&e);
}

void CPalette::SetFirst(QColor First)
{
	m_first = First;
	PaletteEvent();
}

void CPalette::SetSecond(QColor Second)
{
	m_second = Second;
	PaletteEvent();
}

void CPalette::SetDefaultPalette()
{
	m_first = QColor(0, 0, 0), m_second = QColor(255, 255, 255);
	PaletteEvent();
}

void CPalette::Swap()
{
	QColor temp = m_first;
	m_first = m_second, m_second = temp;
	PaletteEvent();
}
