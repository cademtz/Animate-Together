#include "ColorPicker.h"
#include <qshortcut.h>
#include <qpainter.h>
#include <functional>

#include "Interface/MainWindow/MainWindow.h"
#include "Widgets/QuickPaint/CQuickPaint.h"
#include "Widgets/ColorBox/CColorBox.h"
#include "Brushes/Brushes.h"

bool ColorPicker::m_exists = false;

bool ColorPicker::Open(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live)
{
	if (m_exists)
		return false;
	new ColorPicker(Color, OnChange, Live);
	return true;
}

ColorPicker::ColorPicker(QColor Color, std::function<void(ColorPicker*)> OnChange, bool Live)
	: QWidget(&MainWindow::Get()), m_color(Color), m_oldcolor(Color), m_callback(OnChange), m_live(Live)
{
	m_exists = true;
	ui.setupUi(this);

	setAttribute(Qt::WA_DeleteOnClose);
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
	m_freebox = new CColorBox(m_color, ui.f_freebox);
	//m_slidebox = new CQuickPaint([this]() { PaintSlideBox(); }, this);
	m_colorslide = new CColorBox(Qt::red);
	m_preview->setStyleSheet("border: 1px solid black;");
	m_freebox->setStyleSheet(m_preview->styleSheet());
	//m_slidebox->setStyleSheet(m_preview->styleSheet());
	
	ui.l_preview->addWidget(m_preview);
	//ui.l_slidebox->addWidget(m_slidebox);
	ui.l_slidebox->addWidget(m_colorslide);

	ui.btn_cancel->setShortcut(Qt::Key_Escape);
	m_freebox->OnChange([this](CColorBox* c) { OnColorBox(c); });
	m_freebox->setGeometry(1, 1, 255, 255);
	m_colorslide->SetDirection(false, true);
	m_colorslide->SetColormode(CColorBox::Mode_Hue, CColorBox::Mode_Hue);
	m_colorslide->OnChange([this](CColorBox* c) { OnSlideBox(c); });

	show();
}

void ColorPicker::UpdateColor()
{
	m_color = QColor(ui.spin_red->value(), ui.spin_green->value(), ui.spin_blue->value(), ui.spin_alpha->value());
	if (m_live)
		m_callback(this);

	m_preview->update();
	m_freebox->SetColor(m_color);
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
	QPainter paint(m_freebox);
	QImage img = QImage(m_freebox->size().width() - 2, m_freebox->size().height() - 2, QImage::Format::Format_RGB888);
	for (int y = 0; y < img.height(); y++)
		for (int x = 0; x < img.width(); x++)
			img.setPixel(x, y, QColor::fromHsv(m_color.hue(), 255 * (float(x) / img.width()), 255 - 255 * (float(y) / img.height())).rgb());
	paint.drawImage(1, 1, img);
}

void ColorPicker::PaintSlideBox()
{
	/*QPainter paint(m_slidebox);
	QImage img = QImage(m_slidebox->size().width() - 2, m_slidebox->size().height() - 2, QImage::Format::Format_RGB888);
	for (int y = 0; y < img.height(); y++)
		for (int x = 0; x < img.width(); x++)
			img.setPixel(x, y, QColor::fromHsv(360.f * x / img.width(), 220, 220).rgb());
	paint.drawImage(1, 1, img);*/
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
	m_color = ColorBox->Color();
	m_color.setAlpha(ui.spin_alpha->value());

	ui.spin_red->blockSignals(true);
	ui.spin_green->blockSignals(true);
	ui.spin_blue->blockSignals(true);

	ui.spin_red->setValue(ColorBox->Color().red());
	ui.spin_green->setValue(ColorBox->Color().green());
	ui.spin_blue->setValue(ColorBox->Color().blue());

	ui.spin_red->blockSignals(false);
	ui.spin_green->blockSignals(false);
	ui.spin_blue->blockSignals(false);

	m_preview->repaint();
}

void ColorPicker::OnSlideBox(CColorBox * SlideBox)
{
	QColor color = m_freebox->Color();
	color.setHsvF(SlideBox->Color().hueF(), color.saturationF(), color.valueF(), color.alphaF());
	m_freebox->SetColor(color);
}
