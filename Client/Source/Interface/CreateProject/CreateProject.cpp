#include "CreateProject.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

bool CreateProject::m_exists = false;

bool CreateProject::Open()
{
	if (m_exists)
		return false;
	new CreateProject();
	return true;
}

void CreateProject::OnCreate()
{
	CProject* project = new CProject(ui.line_name->text().toStdString(), QSize(ui.spin_width->value(), ui.spin_height->value()));

	// Should have more customization in project settings, but for now these defaults will do.
	project->AddLayer("Background")->Fill(Qt::white);
	project->AddLayer("New layer");

	CProject* last = CProject::ActiveProject();
	CProject::SetActiveProject(project);
	if (last)
		delete last;
	close();
}

CreateProject::CreateProject()
	: QWidget(&MainWindow::Get())
{
	m_exists = true;

	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setWindowModality(Qt::ApplicationModal);
	setAttribute(Qt::WidgetAttribute::WA_DeleteOnClose);
	setFixedSize(size());

	connect(ui.cancel_btn, &QPushButton::released, this, &CreateProject::close);
	connect(ui.create_btn, &QPushButton::released, this, &CreateProject::OnCreate);
	ui.cancel_btn->setShortcut(Qt::Key_Escape);
	ui.create_btn->setShortcut(Qt::Key_Return);

	show();
}