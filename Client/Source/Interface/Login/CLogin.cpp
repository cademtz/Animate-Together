/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/3/2019 3:51:07 PM
 */

#include "CLogin.h"
#include <qshortcut.h>
#include <qlayout.h>
#include "Client\CClient.h"

CLogin::CLogin(bool HasPass)
{
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setWindowTitle("Log in");

	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(50, 50, 50));
	setPalette(pal);

	m_name = new QLineEdit("Nickname", this), m_pass = new QLineEdit("Password", this);
	m_ok = new QPushButton("Join", this);

	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->addWidget(m_name);
	if (HasPass)
		layout->addWidget(m_pass);
	else
		m_pass->setVisible(false);
	layout->addWidget(m_ok);
	setLayout(layout);

	m_ok->setShortcut(Qt::Key_Return);
	QShortcut* esc = new QShortcut(Qt::Key_Escape, this);
	connect(esc, &QShortcut::activated, this, &CLogin::close);
	connect(m_ok, &QPushButton::pressed, this, &CLogin::Confirm);
}

void CLogin::Confirm()
{
	CClient::Login(m_name->text(), m_pass->text());
	close();
}
