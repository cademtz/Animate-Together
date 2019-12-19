/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 12/18/2019 8:12:10 PM
 */

#include "CChatPanel.h"
#include <NetObjects/CUser.h>
#include <qtextbrowser.h>
#include <qpushbutton.h>
#include <qscrollbar.h>
#include <qlineedit.h>
#include <qshortcut.h>
#include <qlayout.h>
#include "Client/CClient.h"

CChatPanel::CChatPanel(QWidget * Parent)
{
	m_doc = new QTextDocument(this);
	m_browser = new QTextBrowser(this);
	m_input = new QLineEdit(this);
	m_send = new QPushButton("Send", this);

	m_send->setFixedWidth(50);
	m_browser->setDocument(m_doc);

	QHBoxLayout* hbox = new QHBoxLayout();
	hbox->addWidget(m_input);
	hbox->addWidget(m_send);

	QVBoxLayout* vbox = new QVBoxLayout();
	vbox->addWidget(m_browser);
	vbox->addLayout(hbox);

	setLayout(vbox);

	QShortcut* m_enter = new QShortcut(Qt::Key_Return, this);

	CClient::Listen([this](CBaseMsg* const Msg) { HandleMsg(Msg); });
	connect(m_send, &QPushButton::pressed, this, &CChatPanel::OnSend);
	connect(m_enter, &QShortcut::activated, this, &CChatPanel::OnSend);
}

void CChatPanel::HandleMsg(CBaseMsg * const Msg)
{
	switch (Msg->Type())
	{
	case CBaseMsg::Msg_Chat:
	{
		auto chat = (CChatMsg* const)Msg;
		QScrollBar* scroll = m_browser->verticalScrollBar();
		bool atEnd = scroll->sliderPosition() == scroll->maximum();

		QTextCursor edit = QTextCursor(m_doc);
		edit.movePosition(QTextCursor::End);

		QTextCharFormat plain, bold;
		plain.setFont(QFont("Tahoma", 8));
		bold = plain;
		bold.setFontWeight(70);

		edit.setCharFormat(bold);
		edit.insertText(CClient::FromHandle(chat->User())->Name(), bold);
		edit.insertText(": " + chat->Text() + '\n', plain);

		if (atEnd)
			scroll->setSliderPosition(scroll->maximum());
	}
	}
}

void CChatPanel::OnSend()
{
	CClient::Send(CChatMsg(m_input->text()));
	m_input->clear();
}
