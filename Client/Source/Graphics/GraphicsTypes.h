/*	Description:
 *		Contains all custom graphics item types
 *
 *	Author: Hold on!
 *	Created: 8/28/2019 9:44:02 PM
 */

#ifndef GraphicsTypes_H
#define GraphicsTypes_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsitem.h>

enum class e_graphicstype
{
	null = QGraphicsItem::UserType,
	Frame,
	Layer,
	FolderLayer,
	FrameLayout,
	LayerLayout,
	ScrubBar,
	Stroke,
	Pixmap,
	Text,
};

#endif // GraphicsTypes_H