/*	Description:
 *		A panel that displays the user's action history 
 *
 *	Author: Hold on!
 *	Created: 3/6/2019 9:06:26 PM
 */

#ifndef CHistoryPanel_H
#define CHistoryPanel_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>

class CHistoryPanel : public QWidget
{
	bool m_focused = false;

public:
	CHistoryPanel(QWidget* Parent = 0);

protected:
	void paintEvent(QPaintEvent* Event);
};

#endif // CHistoryPanel_H