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

std::list<LayerEvent_t> CProject::m_listeners;

void CProject::PutBack(CLayer * Layer, size_t Index)
{
	m_layers.insert(m_layers.begin() + Index, Layer);
	LayerEvent(e_layerevent::add);
}

void CProject::TakeBack(size_t Index)
{
	auto pos = m_layers.begin() + Index;
	if (*pos == m_pActiveLayer)
		SetActiveLayer(0);
	m_layers.erase(pos);
	LayerEvent(e_layerevent::remove);
}

std::deque<CLayer*>::iterator CProject::GetLayerPos(const CLayer * Layer)
{
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		if (*it == Layer)
			return it;
	return m_layers.end();
}

CProject::CProject(const std::string& Name, QSize Dimensions) : m_layers()
{
	SetName(Name);
	SetDimensions(Dimensions);
}

CProject::~CProject()
{
	for (auto layer : GetLayers())
		delete layer;
	m_layers.clear();
}

void CProject::SetActiveLayer(CLayer * Layer)
{
	if (Layer && !HasLayer(Layer))
		return;

	m_pActiveLayer = Layer;
	LayerEvent(e_layerevent::switched);
}

size_t CProject::GetLayerIndex(const CLayer * Layer)
{
	auto pos = GetLayerPos(Layer);
	if (pos != m_layers.end())
		return pos - m_layers.begin();
	return UINT_MAX;
}

bool CProject::HasLayer(const CLayer * Layer)
{
	for (auto layer : m_layers)
		if (Layer == layer)
			return true;
	return false;
}

bool CProject::IsLayerSelected(const CLayer * Layer)
{
	for (auto layer : m_pSelectedLayers)
		if (Layer == layer)
			return true;
	return false;
}

void CProject::SelectLayer(CLayer * Layer)
{
	if (!HasLayer(Layer) || IsLayerSelected(Layer))
		return;
	m_pSelectedLayers.push_back(Layer);
	SetActiveLayer(Layer);
}

CLayer * CProject::AddLayer(const std::string & Name, bool Private, bool Visible)
{
	if (m_layers.size() >= MAX_LAYERS)
		return 0;

	std::deque<CLayer*>::iterator pos = m_layers.begin();
	if (m_pActiveLayer)
	{
		pos = GetLayerPos(m_pActiveLayer);
		if (pos == m_layers.end())
			return 0;
	}
	CLayer* layer = new CLayer(this, Name, m_dimensions, Private, Visible);
	pos = m_layers.insert(pos, layer);
	m_undo.Push(new CUndoLayerAdd(*this, layer, pos - m_layers.begin()));
	LayerEvent(e_layerevent::add);
	SetActiveLayer(layer);
	return layer;
}

CLayer * CProject::InsertLayer(size_t Index, const std::string & Name, bool Private, bool Visible)
{
	if (m_layers.size() >= MAX_LAYERS)
		return 0;
	CLayer* layer = *m_layers.insert(m_layers.begin() + Index, new CLayer(this, Name, GetDimensions(), Private, Visible));
	LayerEvent(e_layerevent::add);
	SetActiveLayer(layer);
	return layer;
}

const bool CProject::RemoveLayer(const CLayer * Layer)
{
	if (!Layer)
		return false;
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
	{
		if (*it == Layer)
		{
			if (*it == m_pActiveLayer)
				SetActiveLayer(0);
			m_undo.Push(new CUndoLayerDelete(*this, *it, it - m_layers.begin()));
			m_layers.erase(it);
			LayerEvent(e_layerevent::remove);
			return true;
		}
	}
	return false;
}

bool CProject::ShiftLayer(CLayer * Layer, size_t Index)
{
	if (!HasLayer(Layer))
		return false;
	if (Index > GetLayers().size())
		Index = GetLayers().size();

	size_t prev = GetLayerIndex(Layer);
	m_layers.insert(m_layers.begin() + Index, Layer);
	m_layers.erase(m_layers.begin() + (Index < prev ? prev + 1 : prev));

	LayerEvent(e_layerevent::moved);

	return true;
}

void CProject::LayerEvent(e_layerevent Event)
{
	for (auto fn : m_listeners)
		fn(Event);
}

#include "Interface/MainWindow/MainWindow.h"
void CProject::Export(e_export Type)
{
	auto& layers = GetLayers();
	if (!layers.size())
		return;

	QFileDialog dlg = QFileDialog(&MainWindow::Get(), "Save image as...");
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
		QImage img = QImage(GetDimensions(), QImage::Format::Format_ARGB32);
		img.fill(Qt::GlobalColor::transparent);
		QPainter paint(&img);
		for (auto it = layers.rbegin(); it != layers.rend(); it++)
		{
			if ((*it)->IsVisible())
				paint.drawPixmap(0, 0, *(*it)->GetPixmap());
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
			layer->GetPixmap()->save(QString::fromStdString(path + name + " - " + layer->GetName() + '.' +  type), "png");
		break;
	}
	}
}