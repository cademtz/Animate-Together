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
	enum EColorMode : uint8_t
	{
		Mode_Hue,
		Mode_Sat,
		Mode_Val
	};

	CColorBox(QColor Color, QWidget* Parent = nullptr);

	inline const QColor& Color() const { return m_color; }
	inline EColorMode VerticalColorMode() const { return m_vert; }
	inline EColorMode HorizontalColorMode() const { return m_hor; }
	inline bool Up() const { return m_up; }
	inline bool Left() const { return m_left; }

	// - Sets the base color and repaints
	inline void SetColor(QColor Color) { UpdateColor(Color); }

	// - Sets the colormode and repaints
	inline void SetColormode(EColorMode Vert, EColorMode Hor)
	{
		m_vert = Vert, m_hor = Hor;
		update();
	}

	// - Sets the vertical and horizontal direction and repaints
	inline void SetDirection(bool Up, bool Left)
	{
		m_up = Up, m_left = Left;
		update();
	}

	// - Sets a callback to be called when the color is changed by the user
	inline void OnChange(std::function<void(CColorBox*)> Callback) { m_callback = Callback; }

protected:
	void paintEvent(QPaintEvent* Event);
	void mousePressEvent(QMouseEvent* Event);
	void mouseMoveEvent(QMouseEvent* Event);
	void mouseReleaseEvent(QMouseEvent* Event);
	void resizeEvent(QResizeEvent* Event);


private:
	EColorMode m_vert = Mode_Val, m_hor = Mode_Sat;
	bool m_up = true, m_left = false;
	QPoint m_cursor;
	QColor m_color = Qt::white, m_base;
	QImage m_gradient;

	std::function<void(CColorBox*)> m_callback = nullptr;

	int FromMode(EColorMode Mode) const;
	inline int HorVal() const { return FromMode(m_hor); }
	inline int VertVal() const { return FromMode(m_vert); }

	QColor ColorAt(int X, int Y);
	QPoint PosAt(const QColor& Color);

	void UpdateColor(QPoint Cursor);
	void UpdateColor(QColor Color);
	void UpdateCursor();
};

#endif // CColorBox_H