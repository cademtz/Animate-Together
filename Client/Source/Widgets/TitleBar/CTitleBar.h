/*	Description:
 *		Provides a title bar widget that drags a main window
 *
 *	Author: Hold on!
 *	Created: 2/18/2019 6:01:55 PM
 */

#ifndef CTitleBar_H
#define CTitleBar_H
#ifdef _WIN32
#pragma once
#endif

#include <QtWidgets\QMainWindow>
#include <qpushbutton.h>

enum class e_export;
class QHBoxLayout;
class QLabel;

class CTitleBar : public QWidget
{
	QMainWindow* m_parent;
	QFont m_fButtons;

	QHBoxLayout* m_layout;
	QLabel* m_name;

	QPoint m_cursor;
	QColor m_background;
	QPushButton *m_hide, *m_minmax, *m_close;
	QMenuBar* m_menubar;
	QSize m_oldsize;

public:
	CTitleBar(QMainWindow* Window, QColor Background = (QRgb)0);

	// - Returns the background color of the title bar
	inline const QColor& GetBackground() const { return m_background; }
	// - Sets a new background color for the title bar
	void SetBackground(const QColor& Col);

protected:
	void mouseDoubleClickEvent(QMouseEvent *Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void resizeEvent(QResizeEvent* Event);
	// - Calls 'close()' on the parent, triggering the close event
	void CloseParent();
	// - Toggles normal and maximum parent size with 'showNormal()' and 'showMaximized()'
	void MinMaxParent();
	// - Calls 'hide()' on the parent, triggering the hide event
	void HideParent();
};

#endif // CTitleBar_H