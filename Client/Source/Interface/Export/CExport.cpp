#include "CExport.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

void CExport::Open(CProject* Project)
{
	new CExport(Project);
}

void CExport::Confirm()
{
	m_proj->Export(ui.cmb_export->currentIndex() == 1, ui.cmb_format->currentIndex() == 0);
	close();
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

	connect(ui.btn_cancel, &QPushButton::released, this, &CExport::close);
	connect(ui.btn_export, &QPushButton::released, this, &CExport::Confirm);
	ui.btn_cancel->setShortcut(Qt::Key_Escape);
	ui.btn_export->setShortcut(Qt::Key_Return);

	show();
}