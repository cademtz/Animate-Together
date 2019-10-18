#include "CConnect.h"

CConnect::CConnect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setFixedSize(size());

	show();
}

CConnect::~CConnect()
{
}
