#pragma once

#include <QWidget>
#include "ui_Export.h"

class CProject;

class CExport : public QWidget
{
	Q_OBJECT

public:
	// - Opens a new export options window for specified project
	static void Open(CProject* Project);

protected:
	void Confirm();

private:
	CExport(CProject* Project);
	~CExport() { }

	Ui::Export ui;
	CProject* m_proj;
};
