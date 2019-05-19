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
	project->AddLayer("New layer");

	CProject* last = MainWindow::Get().GetActiveProj();
	MainWindow::Get().SetActiveProj(project);
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