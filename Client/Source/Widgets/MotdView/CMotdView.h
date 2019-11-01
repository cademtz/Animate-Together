/*	Description:
 *		Provides a widget which can display a server's MOTD either as a widget or window
 *
 *	Author: Hold on!
 *	Created: 10/31/2019 10:42:47 AM
 */

#ifndef CMotdView_H
#define CMotdView_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget>
#include <qlayout.h>

class CWelcomeMsg;

class CMotdView : public QWidget
{
public:
	CMotdView(const CWelcomeMsg& Msg, QWidget* Parent = 0);

	// - Creates a new window that displays the MOTD
	static void Open(const CWelcomeMsg& Msg);

private:
	QWidget* m_display;
	QVBoxLayout* m_layout;
};

#endif // CMotdView_H