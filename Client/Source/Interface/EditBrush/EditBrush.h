#pragma once

#include <QWidget>
#include "ui_EditBrush.h"

struct BrushScale
{
	double min, max;
	inline double getSize(double Pressure) { return Pressure * (max - min) + min; }
};

class EditBrush : public QWidget
{
	Q_OBJECT

public:
	// - Opens the EditBrush interface at an initial location
	// - Returns pointer to existing instance if already open.
	static bool Open(const QPoint& Pos, BrushScale* Scale);
	// - Closes the existing EditBrush interface
	// - Return true if an instance existed and was closed
	static bool Close();
	~EditBrush();

private:
	static EditBrush* inst;
	BrushScale* m_pScale;
	EditBrush(const QPoint& Pos, BrushScale* Scale, QWidget *parent = Q_NULLPTR);
	void paintEvent(QPaintEvent* Event);
public:
	Ui::EditBrush ui;
};
