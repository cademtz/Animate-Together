#include "CImport.h"
#include "Projects/CProject.h"

void CImport::Open(CProject* Project)
{
	new CImport(Project);
}

void CImport::Confirm()
{
	int start;
	if (ui.r_start->isChecked())
		start = 0;
	else if (ui.r_current->isChecked())
		start = m_proj->ActiveFrame();
	else if (ui.r_end->isChecked())
		start = m_proj->EndFrame() + 1;
	else if (ui.r_custom->isChecked())
		start = ui.spin_frame->value() - 1;

	m_proj->ImportSequence(start);
	close();
}

CImport::CImport(CProject* Project) : m_proj(Project)
{
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowCloseButtonHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::ApplicationModal);
	setFixedSize(size());

	ui.spin_frame->setDisabled(!ui.r_custom->isChecked());

	connect(ui.r_custom, &QRadioButton::toggled, [this](bool checked) { ui.spin_frame->setDisabled(!checked); });
	connect(ui.btn_cancel, &QPushButton::released, this, &CImport::close);
	connect(ui.btn_ok, &QPushButton::released, this, &CImport::Confirm);

	show();
}

CImport::~CImport()
{
}
