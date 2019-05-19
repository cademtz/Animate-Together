#include "EditBrush.h"
#include <qpainter.h>

EditBrush* EditBrush::inst = 0;

bool EditBrush::Open(const QPoint& Pos, BrushScale_t* Scale)
{
	if (!inst)
		inst = new EditBrush(Pos, Scale);
	return inst;
}

EditBrush::EditBrush(const QPoint& Pos, BrushScale_t* Scale, QWidget *parent)
	: QWidget(parent), m_pScale(Scale)
{
	ui.setupUi(this);
	move(Pos);

	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setWindowFlags(Qt::WindowType::Popup);
	setWindowModality(Qt::ApplicationModal);

	connect(ui.spin_min, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui.slide_min, &QSlider::setValue);
	connect(ui.slide_min, &QSlider::valueChanged, ui.spin_min, &QDoubleSpinBox::setValue);
	connect(ui.spin_min, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]
	{
		if (ui.spin_min->value() > ui.spin_max->value()) 
			ui.spin_max->setValue(ui.spin_min->value());
	});

	connect(ui.spin_max, QOverload<double>::of(&QDoubleSpinBox::valueChanged), ui.slide_max, &QSlider::setValue);
	connect(ui.slide_max, &QSlider::valueChanged, ui.spin_max, &QDoubleSpinBox::setValue);
	connect(ui.spin_max, QOverload<double>::of(&QDoubleSpinBox::valueChanged), [this]
	{
		if (ui.spin_max->value() < ui.spin_min->value())
			ui.spin_min->setValue(ui.spin_max->value());
	});

	ui.spin_min->setValue(Scale->min);
	ui.spin_max->setValue(Scale->max);

	show();
}

void EditBrush::paintEvent(QPaintEvent * Event)
{
	QPainter paint(this);
	QLinearGradient grad = QLinearGradient(0, 0, 0, (qreal)height() - 1);
	grad.setColorAt(0, QColor(0, 128, 255));
	grad.setColorAt(1, QColor(100, 150, 255));
	paint.setPen(QPen(grad, 1));
	paint.drawRect(0, 0, width() - 1, height() - 1);
}

bool EditBrush::Close()
{
	if (inst)
		return inst->close();
	return false;
}

EditBrush::~EditBrush()
{
	m_pScale->min = ui.spin_min->value(), m_pScale->max = ui.spin_max->value();
	inst = 0;
}