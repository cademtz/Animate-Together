/*	Description:
 *		A scene that holds interactive graphical versions of objects such as strokes, selections, images, users, and more
 *
 *	Author: Hold on!
 *	Created: 11/4/2019 10:18:39 AM
 */

#ifndef CCanvasScene_H
#define CCanvasScene_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicsscene.h>

class CCanvasScene : public QGraphicsScene
{
public:
	CCanvasScene(QObject* Parent = 0) : QGraphicsScene(Parent) { }

};

#endif // CCanvasScene_H