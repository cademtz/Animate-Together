/*	Description:
 *		A window that displays all necessary login fields
 *
 *	Author: Hold on!
 *	Created: 11/3/2019 3:51:29 PM
 */

#ifndef CLogin_H
#define CLogin_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <qlineedit.h>
#include <qpushbutton.h>

class CLogin : public QWidget
{
public:
	static void Open(bool HasPass) { (new CLogin(HasPass))->show(); }

private:
	CLogin(bool HasPass);
	void Confirm();
	void closeEvent(QCloseEvent *event) override;

	QLineEdit* m_name, * m_pass;
	QPushButton* m_ok;
	bool m_confirmed = false;
};

#endif // CLogin_H