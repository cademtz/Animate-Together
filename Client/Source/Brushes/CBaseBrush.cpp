/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/17/2019 4:40:02 PM
 */

#include "CBaseBrush.h"
#include <qpixmap.h>
#include <qpainter.h>

namespace BrushesPrivate
{
	std::list<CBaseBrush*>* CBaseBrush::init_list = new std::list<CBaseBrush*>();

	CBaseBrush::CBaseBrush()
	{
		if (!init_list)
			init_list = new std::list<CBaseBrush*>();
		init_list->push_back(this);
	}

	void CBaseBrush::InitBrushes()
	{
		for (auto b : *init_list)
			b->Init();
		delete init_list;
	}

	void CCheckerBrush::Init()
	{
		QPixmap checkers;
		checkers = QPixmap(20, 20);
		QPainter draw(&checkers);
		draw.fillRect(checkers.rect(), QColor(100, 100, 100));
		for (char i = 0; i < 2; i++)
			draw.fillRect(i * 10, i * 10, 10, 10, QColor(60, 60, 60));
		setTexture(checkers);
	}
	
}