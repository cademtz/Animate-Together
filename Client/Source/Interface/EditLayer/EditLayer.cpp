#include "EditLayer.h"
#include <qshortcut.h>
#include <qevent.h>
#include <qsizegrip.h>
#include "Interface/MainWindow/MainWindow.h"
#include "Projects/CProject.h"

bool EditLayer::bExists = false;

bool EditLayer::Open(CProject * Project, CLayer* Layer)
{
	if (bExists)
		return false;
	new EditLayer(Project, Layer);
	return true;
}

EditLayer::EditLayer(CProject* Project, CLayer* Layer)
	: QWidget(&MainWindow::Get()), m_proj(Project), m_layer(Layer)
{
	bExists = true;
	ui.setupUi(this);
	setWindowFlags(Qt::Dialog | Qt::CustomizeWindowHint | Qt::WindowTitleHint);
	setAttribute(Qt::WA_DeleteOnClose);
	setWindowModality(Qt::ApplicationModal);
	setFixedSize(size());

	if (Layer)
	{
		ui.tb_layername->setText(Layer->Name().c_str());
		ui.cb_hidden->setChecked(!Layer->IsVisible());
		ui.cb_private->setChecked(Layer->IsPrivate());
	}

	QShortcut* enter = new QShortcut(Qt::Key_Enter, this), *ret = new QShortcut(Qt::Key_Return, this);
	connect(enter, &QShortcut::activated, this, &EditLayer::Confirm);
	connect(ret, &QShortcut::activated, this, &EditLayer::Confirm);
	connect(ui.btn_cancel, &QPushButton::released, this, &EditLayer::close);
	connect(ui.btn_ok, &QPushButton::released, this, &EditLayer::Confirm);

	ui.btn_cancel->setShortcut(Qt::Key_Escape);

	show();
}

void EditLayer::Confirm()
{
	m_ok = true;
	close();
}

void EditLayer::closeEvent(QCloseEvent * Event)
{
	if (!m_ok || CProject::ActiveProject() != m_proj)
		return;
	if (!m_layer)
		m_proj->AddLayer(ui.tb_layername->text().isEmpty() ? "New layer " + std::to_string(m_proj->Layers().size() + 1) : ui.tb_layername->text().toStdString(), ui.cb_private->isChecked());
	else if (m_proj->HasLayer(m_layer))
	{
		m_layer->SetName(ui.tb_layername->text().isEmpty() ? "Layer " + std::to_string(m_proj->IndexOf(m_layer) + 1) : ui.tb_layername->text().toStdString());
		m_layer->SetVisible(!ui.cb_hidden->isChecked());
		m_layer->SetPrivate(ui.cb_private->isChecked());
		m_layer->LayerEvent(CLayerEvent::Edit);
	}
}