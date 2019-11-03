/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 10/31/2019 10:45:25 AM
 */

#include "CMotdView.h"
#include <Shared/CNetMsg.h>
#include <qtextbrowser.h>
#include <qicon.h>

CMotdView::CMotdView(const CWelcomeMsg & Msg, QWidget * Parent) :	QWidget(Parent)
{
	m_layout = new QVBoxLayout(this);
	m_layout->setMargin(0);
	if (Msg.IsUrl())
	{
		// TO DO: Use a much smaller browser like UltraLight. Qt's makes the release size more than 6x the size
	}
	else
	{
		QTextBrowser* browser = new QTextBrowser(this);
		browser->setLineWrapMode(QTextBrowser::NoWrap);
		browser->setDocument(new QTextDocument(browser));
		browser->document()->setHtml(Msg.Motd());
		browser->setFrameStyle(0);
		m_display = browser;
	}

	m_layout->addWidget(m_display);
}

void CMotdView::Open(const CWelcomeMsg& Msg)
{
	CMotdView* view = new CMotdView(Msg);
	view->setAttribute(Qt::WA_DeleteOnClose);
	view->setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint | Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint);
	view->setWindowTitle("Message of the day");
	view->show();
}
