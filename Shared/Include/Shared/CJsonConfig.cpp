/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/1/2019 5:39:28 PM
 */

#include "CJsonConfig.h"
#include <qdir.h>

bool CJsonConfig::Open(QString File, QString Dir)
{
	bool opened = false;
	if (!Dir.isEmpty())
	{
		QDir().mkpath(Dir);
		File.prepend(Dir + '/');
	}
	if (!File.isEmpty())
		m_file.setFileName(File);
	if (!File.isEmpty() || (!m_file.isOpen() && !m_file.exists()))
		opened = m_file.open(QIODevice::ReadWrite | QIODevice::Text);
	if (opened)
		m_doc = QJsonDocument::fromJson(m_file.readAll());
	return opened;
}

void CJsonConfig::Close()
{
	m_file.close();
	m_doc = QJsonDocument();
}
