/*	Description:
 *		Provides a basic QGraphicsLayoutItem class which inherits typical graphics items work with layouts.
 *		T is assumed to be a class inherting QGraphicsItem.
 *
 *	Author: Hold on!
 *	Created: 12/1/2019 9:38:24 PM
 */

#ifndef CLayoutWrap_H
#define CLayoutWrap_H
#ifdef _WIN32
#pragma once
#endif

#include <qgraphicslayoutitem.h>

template <typename T>
class CLayoutWrap : public T, public QGraphicsLayoutItem
{
	using T::T;
public:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint) const override { return boundingRect().size(); }
};


#endif // CLayoutWrap_H