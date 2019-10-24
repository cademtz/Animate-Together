#include "CConnect.h"
#include "Client/CClient.h"

CConnect::CConnect(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setFixedSize(size());

	QPalette pal = palette();
	pal.setBrush(QPalette::Window, QColor(50, 50, 50));
	setPalette(pal);

	connect(ui.btn_connect, &QPushButton::pressed, this, &CConnect::Connect);

	show();
}

void CConnect::Connect()
{
	CClient::Connect(ui.tb_host->text());
	close();
}
