#pragma once

#include <QWidget>
#include "ui_CreateProject.h"

class CreateProject : public QWidget
{
	Q_OBJECT

public:
	static bool Open();

private:
	static bool m_exists;
	void OnCreate();

	CreateProject();
	~CreateProject() { m_exists = false; }

	Ui::CreateProject ui;
};
