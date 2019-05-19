/*	Description:
 *		A widget purely for painting.
 *		Handy for adding to layouts made in Qt Designer, where C++ widgets are otherwise not visible.
 *		This prevents me from blindly placing widgets in code manually any time the layout changes.
 *
 *	Author: Hold on!
 *	Created: 3/16/2019 10:06:45 PM
 */

#ifndef CQuickPaint_H
#define CQuickPaint_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <functional>

class CQuickPaint : public QWidget
{
	std::function<void()> m_paintcall = 0;
	std::function<void(QMouseEvent*)> m_press, m_release;

public:
	CQuickPaint(std::function<void()> OnPaint, QWidget* Parent = Q_NULLPTR)
		: QWidget(Parent), m_paintcall(OnPaint) { }
	void HookMouse(std::function<void(QMouseEvent*)> OnPress = 0, std::function<void(QMouseEvent*)> OnRelease = 0) {
		m_press = OnPress, m_release = OnRelease;
	}

protected:
	void paintEvent(QPaintEvent* Event) { m_paintcall(); }
	void mousePressEvent(QMouseEvent* Event) { if (m_press) m_press(Event); }
	void mouseReleaseEvent(QMouseEvent* Event) { if (m_release) m_release(Event); }
};

#endif // CQuickPaint_H