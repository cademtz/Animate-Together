/*	Description:
 *		A small two-color palette that holds and switches between two main brush colors
 *
 *	Author: Hold on!
 *	Created: 4/18/2019 8:32:33 PM
 */

#ifndef CMiniPalette_H
#define CMiniPalette_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <functional>
#include <list>

class CMiniPalette : public QWidget
{
	static const QRect firstRect();
	const QRect secondRect();

	static std::list<std::function<void(CMiniPalette*)>> m_listeners;

public:
	CMiniPalette(QWidget* Parent = nullptr);

	void Swap();
	void SetToDefault();

	// - When called, shows a color picker window
	// @ First:	Sets the first color when true, otherwise the second
	void PickColor(bool First = true);

	// - Adds a pointer to your function to the listener list
	// - Listeners are called when one of the colors changes
	static inline void Listen(std::function<void(CMiniPalette*)> Func) { m_listeners.push_back(Func); }

	// - Calls all event listeners
	void PalletteEvent();

protected:
	void paintEvent(QPaintEvent* e);
	void mousePressEvent(QMouseEvent* e);
};

#endif // CMiniPalette_H