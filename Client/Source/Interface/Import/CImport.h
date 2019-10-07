#pragma once

#include <QWidget>
#include "ui_Import.h"

class CProject;
class CImport : public QWidget
{
	Q_OBJECT

public:
	static void Open(CProject* Project);
	void Confirm();

	~CImport();

private:
	CImport(CProject* Project);

	Ui::Import ui;
	CProject* m_proj;
};
