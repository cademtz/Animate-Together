/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/1/2019 5:39:28 PM
 */

#include "CJsonConfig.h"
#include <qdir.h>
#include <qjsonobject.h>

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
	{
		QJsonDocument doc = QJsonDocument::fromJson(m_file.readAll());
		if (doc.isObject())
			m_doc = doc;
	}
	return opened;
}

void CJsonConfig::Close()
{
	m_file.close();
	m_doc = QJsonDocument();
}

QString CJsonConfig::Str(const QString& Key, QString Default)
{
	if (m_doc.object().contains(Key))
		return m_doc.object()[Key].toString();
	SetVal(Key, Default);
	return Default;
}

int CJsonConfig::Int(const QString & Key, int Default)
{
	if (m_doc.object().contains(Key))
		return m_doc.object()[Key].toInt();
	SetVal(Key, Default);
	return Default;
}

void CJsonConfig::Write()
{
	m_file.write(m_doc.toBinaryData());
}
