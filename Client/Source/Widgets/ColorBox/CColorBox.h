/*	Description:
 *		A rectangular area that can display a gradient between any two properties (hue, value, saturation)
 *		Takes user input and provides a picker to drag around	
 *
 *	Author: Hold on!
 *	Created: 4/13/2019 5:34:13 PM
 */

#ifndef CColorBox_H
#define CColorBox_H
#ifdef _WIN32
#pragma once
#endif

#include <qwidget.h>
#include <functional>

class CColorBox : public QWidget
{
public:
	enum e_colormode
	{
		ColorMode_SaturationValue,
		ColorMode_SaturationHue,
		ColorMode_ValueHue
	};

	CColorBox(QColor Color, QWidget* Parent = nullptr);

	inline QColor color() const { return m_color; }
	inline e_colormode colormode() const { return m_colormode; }

	// - Changes the base color and repaints
	inline void setColor(QColor Color) {
		updateColor(Color);
		update();
	}
	// - Changes the colormode and repaints
	void updateColormode(e_colormode Colormode);
	// - Sets a callback to be called when the color is changed by the user
	inline void onChange(std::function<void(CColorBox*)> Callback) { m_callback = Callback; }

protected:
	void paintEvent(QPaintEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void resizeEvent(QResizeEvent* Event);

private:
	e_colormode m_colormode;
	QPoint m_cursor;
	QColor m_color, m_base;
	QImage m_gradient;

	std::function<void(CColorBox*)> m_callback = nullptr;

	void updateColor(QPoint Cursor);
	void updateColor(QColor Color);
};

#endif // CColorBox_H