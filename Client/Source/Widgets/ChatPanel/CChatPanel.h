/*	Description:
 *		Displays server chat in a panel
 *
 *	Author: Hold on!
 *	Created: 12/18/2019 8:08:08 PM
 */

#ifndef CChatPanel_H
#define CChatPanel_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>

class CBaseMsg;
class QTextDocument;
class QTextBrowser;
class QLineEdit;
class QPushButton;

class CChatPanel : public QWidget
{
public:
	CChatPanel(QWidget* Parent = nullptr);

private:
	void HandleMsg(CBaseMsg* const Msg);
	void OnSend();

	QTextDocument* m_doc;
	QTextBrowser* m_browser;
	QLineEdit* m_input;
	QPushButton* m_send;
};

#endif // CChatPanel_H