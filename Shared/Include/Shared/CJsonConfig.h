/*	Description:
 *		The CJsonConfig class allows easy creation by creating files and properties if they are non-existent
 *		It can be used abstractly, or inherited to make configs that automatically set up default files and values
 *
 *	Author: Hold on!
 *	Created: 11/1/2019 5:32:19 PM
 */

#ifndef CJsonConfig_H
#define CJsonConfig_H
#ifdef _WIN32
#pragma once
#endif

#include <qfile.h>
#include <qjsondocument.h>
#include "Config.h"

class CJsonConfig
{
public:
	inline CJsonConfig(QString File = QString(), QString Dir = QString()) { Open(File, Dir); }

	bool Open(QString File = QString(), QString Dir = QString());
	void Close();
	const QFile& File() const { return m_file; }
	const QJsonDocument& Doc() const { return m_doc; }

	QString Str(const QString& Key, QString Default = QString());
	int Int(const QString& Key, int Default = -1);

	template<typename T>
	void SetVal(const QString& Key, const T& Val)
	{
		m_doc.object()[Key] = Val;
		Write();
	}

private:
	void Write();

	QFile m_file;
	QJsonDocument m_doc;
};

class CServerCfg : private CJsonConfig
{
public:
	CServerCfg() : CJsonConfig("Server.json", AT_CFG_SERVER) { }

	uint16_t Port() { return Int("MotdFile", AT_DEFPORT); }
	QString Password() { return Str("Password"); }
	QFile MotdFile() { return Str("MotdFile", "Motd.html"); }
};

#endif // CJsonConfig_H