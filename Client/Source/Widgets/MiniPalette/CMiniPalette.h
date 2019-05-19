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
	QColor m_first, m_second;
	static const QRect firstRect();
	const QRect secondRect();

	static std::list<std::function<void(CMiniPalette*)>> m_listeners;

public:
	CMiniPalette(QWidget* Parent = nullptr);

	void swap();
	void reset();

	inline QColor first() const { return m_first; }
	inline void setFirst(QColor First) {
		m_first = First;
		update();
		PalletteEvent();
	}
	inline QColor second() const { return m_second; }
	inline void setSecond(QColor Second) {
		m_second = Second;
		update();
		PalletteEvent();
	}

	// - When called, shows a color picker window
	// @ First:	Sets the first color when true, otherwise the second
	void pickColor(bool First = true);

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