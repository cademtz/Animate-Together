#ifndef ColorPicker_H
#define ColorPicker_H
#ifdef _WIN32
#pragma once
#endif

#include <QWidget>
#include <functional>
#include "ui_ColorPicker.h"

class CQuickPaint;
class CColorBox;

class ColorPicker : public QWidget
{
	Q_OBJECT

public:
	// - Opens the ColorPicker interface with an initial color
	// - Returns false and does not show if an existing ColorPicker window exists
	// @ Color:		Original or initial color
	// @ OnChange:	A callback with a pointer to the ColorPicker when the color updates
	// @ Live:		When true, OnChange is called every time the color updates
	static bool Open(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live = false);
	~ColorPicker() { m_exists = false; }

	void PaintPreview();
	void PaintHues();
	void PaintSlideBox();

	QColor m_color, m_oldcolor;

private:
	Ui::ColorPicker ui;
	bool m_ok = false, m_live;
	std::function<void(ColorPicker*)> m_callback;
	static bool m_exists;

	CQuickPaint* m_preview;// , *m_slidebox;
	CColorBox* m_freebox, * m_colorslide;

	ColorPicker(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live);

protected:
	void UpdateColor();
	void Confirm();
	void RevertColor();
	void OnColorBox(CColorBox* ColorBox);
	void OnSlideBox(CColorBox* SlideBox);
};

#endif // ColorPicker_H