#pragma once

#include <QWidget>
#include "ui_Connect.h"

class CConnect : public QWidget
{
	Q_OBJECT

public:
	CConnect(QWidget *parent = Q_NULLPTR);
	static inline void Open() { new CConnect(); }

protected:
	void Connect();

private:
	Ui::Connect ui;
};
