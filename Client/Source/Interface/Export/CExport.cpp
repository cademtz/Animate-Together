#include "CExport.h"
#include "Interface/MainWindow/MainWindow.h"

void CExport::Open(CProject* Project)
{
	new CExport(Project);
}

CExport::CExport(CProject* Project)
	: QWidget(&MainWindow::Get()), m_proj(Project)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::ApplicationModal);
	setFixedSize(size());

	ui.cmb_export->addItems({ "Image sequence", "Current frame" });
	ui.cmb_format->addItems({ "Flat image", "Separate layers" });

	show();
}