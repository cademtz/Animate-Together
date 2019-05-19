#include "ColorPicker.h"
#include <qshortcut.h>
#include <qpainter.h>
#include <functional>

#include "Interface/MainWindow/MainWindow.h"
#include "Widgets/QuickPaint/CQuickPaint.h"
#include "Widgets/ColorBox/CColorBox.h"
#include "Brushes/Brushes.h"

bool ColorPicker::bExists = false;

bool ColorPicker::Open(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live)
{
	if (bExists)
		return false;
	new ColorPicker(Color, OnChange, Live);
	return true;
}

ColorPicker::ColorPicker(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live)
	: QWidget(&MainWindow::Get()), m_color(Color), m_oldcolor(Color), m_callback(OnChange), m_live(Live)
{
	bExists = true;
	ui.setupUi(this);

	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowModality(Qt::ApplicationModal);
	setFixedSize(size());

	ui.spin_red->setValue(m_oldcolor.red());
	ui.spin_green->setValue(m_oldcolor.green());
	ui.spin_blue->setValue(m_oldcolor.blue());
	ui.spin_alpha->setValue(m_oldcolor.alpha());
	
	QShortcut* enter = new QShortcut(Qt::Key_Enter, this), *ret = new QShortcut(Qt::Key_Return, this);
	connect(enter, &QShortcut::activated, this, &ColorPicker::Confirm);
	connect(ret, &QShortcut::activated, this, &ColorPicker::Confirm);
	connect(ui.btn_cancel, &QPushButton::released, this, &ColorPicker::close);
	connect(ui.btn_ok, &QPushButton::released, this, &ColorPicker::Confirm);
	connect(ui.btn_revert, &QPushButton::released, this, &ColorPicker::RevertColor);

	connect(ui.spin_red, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::UpdateColor);
	connect(ui.spin_green, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::UpdateColor);
	connect(ui.spin_blue, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::UpdateColor);
	connect(ui.spin_alpha, QOverload<int>::of(&QSpinBox::valueChanged), this, &ColorPicker::UpdateColor);

	m_preview = new CQuickPaint([this]() { PaintPreview(); }, this);
	m_hues = new CColorBox(m_color, ui.f_huebox);
	m_hueslide = new CQuickPaint([this]() { PaintHueSlider(); }, this);
	m_preview->setStyleSheet("border: 1px solid black;");
	m_hues->setStyleSheet(m_preview->styleSheet());
	m_hueslide->setStyleSheet(m_preview->styleSheet());
	
	ui.l_preview->addWidget(m_preview);
	//ui.f_huebox->addWidget(m_hues);
	ui.l_hueslide->addWidget(m_hueslide);

	ui.btn_cancel->setShortcut(Qt::Key_Escape);
	m_hues->onChange([this](CColorBox* c) { OnColorBox(c); });
	m_hues->setGeometry(1, 1, 255, 255);
	show();
}

void ColorPicker::UpdateColor()
{
	m_color = QColor(ui.spin_red->value(), ui.spin_green->value(), ui.spin_blue->value(), ui.spin_alpha->value());
	if (m_live)
		m_callback(this);

	m_preview->update();
	m_hues->setColor(m_color);
}

void ColorPicker::PaintPreview()
{
	QPainter paint(m_preview);
	QRect halb(1, 1, m_preview->size().width() - 2, (m_preview->size().height() - 2) / 2);
	paint.fillRect(halb, Brushes::checkers);
	paint.fillRect(halb, m_color);

	halb.moveTop(halb.height() + 1);
	halb.setBottom(m_preview->height() - 1);
	paint.fillRect(halb, Brushes::checkers);
	paint.fillRect(halb, m_oldcolor);
}

void ColorPicker::PaintHues()
{
	QPainter paint(m_hues);
	QImage img = QImage(m_hues->size().width() - 2, m_hues->size().height() - 2, QImage::Format::Format_RGB888);
	for (int y = 0; y < img.height(); y++)
		for (int x = 0; x < img.width(); x++)
			img.setPixel(x, y, QColor::fromHsv(m_color.hue(), 255 * (float(x) / img.width()), 255 - 255 * (float(y) / img.height())).rgb());
	paint.drawImage(1, 1, img);
}

void ColorPicker::PaintHueSlider()
{
	QPainter paint(m_hueslide);
	QImage img = QImage(m_hueslide->size().width() - 2, m_hueslide->size().height() - 2, QImage::Format::Format_RGB888);
	for (int y = 0; y < img.height(); y++)
		for (int x = 0; x < img.width(); x++)
			img.setPixel(x, y, QColor::fromHsv(360.f * x / img.width(), 220, 220).rgb());
	paint.drawImage(1, 1, img);
}

void ColorPicker::Confirm()
{
	m_ok = true;
	m_callback(this);
	close();
}

void ColorPicker::RevertColor()
{
	ui.spin_red->setValue(m_oldcolor.red());
	ui.spin_green->setValue(m_oldcolor.green());
	ui.spin_blue->setValue(m_oldcolor.blue());
	ui.spin_alpha->setValue(m_oldcolor.alpha());

	UpdateColor();
}

void ColorPicker::OnColorBox(CColorBox * ColorBox)
{
	m_color = ColorBox->color();
	m_color.setAlpha(ui.spin_alpha->value());

	ui.spin_red->blockSignals(true);
	ui.spin_green->blockSignals(true);
	ui.spin_blue->blockSignals(true);

	ui.spin_red->setValue(ColorBox->color().red());
	ui.spin_green->setValue(ColorBox->color().green());
	ui.spin_blue->setValue(ColorBox->color().blue());

	ui.spin_red->blockSignals(false);
	ui.spin_green->blockSignals(false);
	ui.spin_blue->blockSignals(false);

	m_preview->repaint();
}