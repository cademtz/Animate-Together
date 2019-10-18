#pragma once

#include <QWidget>
#include "ui_Connect.h"

class CConnect : public QWidget
{
	Q_OBJECT

public:
	static inline void Open() { new CConnect(); }

	CConnect(QWidget *parent = Q_NULLPTR);
	~CConnect();

private:
	Ui::Connect ui;
};
