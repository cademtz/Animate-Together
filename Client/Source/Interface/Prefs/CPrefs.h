#pragma once

#include <QWidget>
#include "ui_Prefs.h"

class CPrefs : public QWidget
{
	Q_OBJECT

public:
	CPrefs(QWidget *parent = Q_NULLPTR);
	~CPrefs();

private:
	Ui::Prefs ui;
};
