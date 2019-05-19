/*	Description:
 *		A panel that displays the user's action history 
 *
 *	Author: Hold on!
 *	Created: 3/6/2019 9:06:26 PM
 */

#ifndef CHistory_H
#define CHistory_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>

class MainWindow;
class CHistory : public QWidget
{
	bool m_focused = false;
	MainWindow* m_parent;

public:
	CHistory(MainWindow* Window, QWidget* Parent = 0);

protected:
	void paintEvent(QPaintEvent* Event);
};

#endif // CHistory_H