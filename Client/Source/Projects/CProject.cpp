/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/25/2019 8:35:02 PM
 */

#include "CProject.h"
#include <cstring>
#include <qfiledialog.h>
#include <qpainter.h>

CProject* CProject::m_activeproj = nullptr;
std::list<ProjectCallback_t> CProject::m_listeners;

void CProject::ProjectEvent(CProjectEvent::e_action Action)
{
	CProjectEvent e(this, Action);
	for (auto fn : m_listeners)
		fn(&e);
}

void CProject::PutBack(CLayer * Layer, size_t Index)
{
	m_layers.insert(m_layers.begin() + Index, Layer);
	Layer->LayerEvent(CLayerEvent::Add);
}

void CProject::TakeBack(size_t Index)
{
	auto pos = m_layers.begin() + Index;
	if (*pos == m_activelayer)
		SetActiveLayer(0);
	CLayer* layer = *pos;
	m_layers.erase(pos);
	CLayer::LayerEvent(CLayerEvent(layer, CLayerEvent::Remove, Index));
}

std::deque<CLayer*>::iterator CProject::GetLayerPos(const CLayer * Layer)
{
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		if (*it == Layer)
			return it;
	return m_layers.end();
}

CProject::CProject(const std::string& Name, QSize Dimensions) : m_layers(), m_palette(this)
{
	SetName(Name);
	SetDimensions(Dimensions);
}

CProject::~CProject()
{
	for (auto layer : Layers())
		delete layer;
	m_layers.clear();
}

void CProject::SetActiveProject(CProject * Project)
{
	m_activeproj = Project;
	m_activeproj->ProjectEvent(CProjectEvent::ActiveProject);
}

void CProject::SetActiveFrame(size_t Frame)
{
	m_activeframe = Frame;
	ProjectEvent(CProjectEvent::ActiveFrame);
}

void CProject::SetActiveLayer(CLayer * Layer)
{
	if (Layer && !HasLayer(Layer))
		return;

	m_activelayer = Layer;
	CLayer::LayerEvent(CLayerEvent(Layer, CLayerEvent::Switched));
}

size_t CProject::IndexOf(const CLayer * Layer)
{
	auto pos = GetLayerPos(Layer);
	if (pos != m_layers.end())
		return pos - m_layers.begin();
	return UINT_MAX;
}

bool CProject::HasLayer(const CLayer * Layer) const
{
	for (auto layer : m_layers)
		if (Layer == layer)
			return true;
	return false;
}

bool CProject::IsLayerSelected(const CLayer * Layer)
{
	for (auto layer : m_selectedLayers)
		if (Layer == layer)
			return true;
	return false;
}

bool CProject::SelectLayer(CLayer * Layer, bool Selected)
{
	if (!HasLayer(Layer))
		return false;
	else if (IsLayerSelected(Layer) == Selected)
		return true;

	if (Selected)
	{
		m_selectedLayers.push_back(Layer);
		SetActiveLayer(Layer);
	}
	else
		m_selectedLayers.erase(GetLayerPos(Layer));
	return true;
}

CLayer * CProject::AddLayer(const std::string & Name, bool Private, bool Visible)
{
	if (m_layers.size() >= MAX_LAYERS)
		return 0;

	std::deque<CLayer*>::iterator pos = m_layers.begin();
	if (m_activelayer)
	{
		pos = GetLayerPos(m_activelayer);
		if (pos == m_layers.end())
			return 0;
	}
	CLayer* layer = new CLayer(this, Name, m_dimensions, Private, Visible);
	pos = m_layers.insert(pos, layer);
	m_undo.Push(new CUndoLayerAdd(*this, layer, pos - m_layers.begin()));
	SetActiveLayer(layer);
	layer->LayerEvent(CLayerEvent::Add);
	return layer;
}

CLayer * CProject::InsertLayer(size_t Index, const std::string & Name, bool Private, bool Visible)
{
	if (m_layers.size() >= MAX_LAYERS)
		return 0;
	CLayer* layer = *m_layers.insert(m_layers.begin() + Index, new CLayer(this, Name, Dimensions(), Private, Visible));
	SetActiveLayer(layer);
	layer->LayerEvent(CLayerEvent::Add);
	return layer;
}

const bool CProject::RemoveLayer(CLayer * Layer)
{
	if (!Layer)
		return false;
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if (*it == Layer)
		{
			if (*it == m_activelayer)
				SetActiveLayer(0);
			size_t prev = IndexOf(Layer);
			m_undo.Push(new CUndoLayerDel(*this, *it, it - m_layers.begin()));
			m_layers.erase(it);
			CLayer::LayerEvent(CLayerEvent(Layer, CLayerEvent::Remove, prev));
			return true;
		}
	}
	return false;
}

bool CProject::ShiftLayer(CLayer * Layer, size_t Index)
{
	if (!HasLayer(Layer))
		return false;
	if (Index > Layers().size())
		Index = Layers().size();

	size_t prev = IndexOf(Layer);
	m_layers.insert(m_layers.begin() + Index, Layer);
	m_layers.erase(m_layers.begin() + (Index < prev ? prev + 1 : prev));
	CLayer::LayerEvent(CLayerEvent(Layer, CLayerEvent::Moved, prev));
	return true;
}

#include "Interface/MainWindow/MainWindow.h"
void CProject::Export(e_export Type)
{
	auto& layers = Layers();
	if (!layers.size())
		return;

	QFileDialog dlg = QFileDialog(nullptr, "Save image as...");
	dlg.setFileMode(QFileDialog::AnyFile);
	dlg.setAcceptMode(QFileDialog::AcceptSave);
	dlg.setNameFilters({
		"Portable Network Graphics (*.png)",
		"Joint Photographic Experts Group (*.jpg)",
		"Tagged Image File Format (*.tiff)",
		"Windows Bitmap	(*.bmp)" });

	std::string file;
	if (dlg.exec() && dlg.selectedFiles().length())
		file = dlg.selectedFiles()[0].toStdString();
	else
		return;

	switch (Type)
	{
	case e_export::flat:
	{
		QImage img = QImage(Dimensions(), QImage::Format::Format_ARGB32);
		img.fill(Qt::GlobalColor::transparent);
		QPainter paint(&img);
		for (auto it = layers.rbegin(); it != layers.rend(); it++)
		{
			if ((*it)->IsVisible())
				paint.drawPixmap(0, 0, *(*it)->Pixmap());
		}
		paint.end();
		img.save(QString::fromStdString(file));
		break;
	}
	case e_export::layers:
	{
		std::string path, name, type;
		size_t pos;
		if ((pos = file.find_last_of('/')) != std::string::npos)
			path = file.substr(0, pos + 1);
		if ((pos = file.find_last_of('.')) != std::string::npos)
			type = file.substr(pos + 1);
		name = file.substr(path.length(), file.length() - type.length() - path.length() - 1);
		
		for (auto layer : layers)
			layer->Pixmap()->save(QString::fromStdString(path + name + " - " + layer->GetName() + '.' +  type), "png");
		break;
	}
	}
}

CFrame * CProject::LastFrame() const
{
	CLayer* greatest = nullptr;
	for (auto layer : Layers())
	{
		if (!greatest || layer->Frames().size() > greatest->Frames().size())
			greatest = layer;
	}
	return greatest->LastFrame();
}
