/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 11/1/2019 5:39:28 PM
 */

#include "CJsonConfig.h"
#include <qdir.h>
#include <qjsondocument.h>
#include <qdebug.h>

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
		opened = m_file.open(QIODevice::ReadOnly);
	if (opened)
	{
		QJsonDocument doc = QJsonDocument::fromJson(m_file.readAll());
		if (doc.isObject())
			m_obj = doc.object();
	}
	return opened;
}

void CJsonConfig::Close()
{
	Write();
	m_file.close();
	m_obj = QJsonObject();
}

QString CJsonConfig::Str(const QString& Key, QString Default)
{
	if (m_obj.contains(Key))
		return m_obj[Key].toString();
	m_obj[Key] = QJsonValue(Default);
	m_change = true;
	return Default;
}

int CJsonConfig::Int(const QString & Key, int Default)
{
	if (m_obj.contains(Key))
		return m_obj[Key].toInt();
	m_obj[Key] = Default;
	m_change = true;
	return Default;
}

void CJsonConfig::Write()
{
	if (m_file.isOpen())
		m_file.close();
	if (!m_change)
		return;
	if (m_file.open(QIODevice::WriteOnly | QIODevice::Truncate))
		m_file.write(QJsonDocument(m_obj).toJson());
	m_file.close();
}
