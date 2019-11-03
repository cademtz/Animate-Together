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
#include <qjsonobject.h>
#include "Config.h"

class CJsonConfig
{
public:
	inline CJsonConfig(QString File = QString(), QString Dir = QString()) { Open(File, Dir); }
	inline ~CJsonConfig() { Close(); }

	bool Open(QString File = QString(), QString Dir = QString());
	void Close();
	const QFile& File() const { return m_file; }
	const QJsonObject& JsonObject() const { return m_obj; }

	QString Str(const QString& Key, QString Default = QString());
	int Int(const QString& Key, int Default = -1);

private:
	void Write();

	QFile m_file;
	QJsonObject m_obj;
};

class CServerCfg : private CJsonConfig
{
public:
	CServerCfg() : CJsonConfig("Server.json", AT_CFG_SERVER) { }

	uint16_t Port() { return Int("Port", AT_DEFPORT); }
	QString Password() { return Str("Password"); }
	QString MotdFile() { return Str("MotdFile", AT_CFG_SERVER "/Motd.html"); }
};

#endif // CJsonConfig_H