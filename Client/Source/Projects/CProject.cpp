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
CProject::Listeners_t CProject::m_listeners;

CProject::CProject(const std::string& Name, QSize Dimensions) : m_layers(), m_palette(this)
{
	SetName(Name);
	SetDimensions(Dimensions);

	m_timer = new QTimer();
	m_timer->connect(m_timer, &QTimer::timeout, [this] { TimerEvent(); });
}

CProject::~CProject()
{
	for (auto layer : Layers())
		delete layer;
	m_layers.clear();
	delete m_timer;
}

void CProject::SetActiveProject(CProject * Project)
{
	m_activeproj = Project;
	CreateEvent(CProjectEvent(m_activeproj, CProjectEvent::ActiveProject));
}

void CProject::SetFramerate(size_t Framerate)
{
	if (Framerate == m_framerate)
		return;
	m_framerate = Framerate;
	m_timer->setInterval(round(1000.f / Framerate));
	CreateEvent(CProjectEvent(this, CProjectEvent::Framerate));
}

void CProject::SetActiveFrame(int Frame)
{
	if (m_activeframe == Frame)
		return;
	m_activeframe = Frame;
	CreateEvent(CProjectEvent(this, CProjectEvent::ActiveFrame));
}

void CProject::SetActiveLayer(CLayer * Layer)
{
	if (Layer && !HasLayer(Layer))
		return;

	m_activelayer = Layer;
	CLayer::CreateEvent(CLayerEvent(Layer, CLayerEvent::Switched));
}

int CProject::IndexOf(const CLayer * Layer)
{
	auto pos = GetLayerPos(Layer);
	if (pos != m_layers.end())
		return pos - m_layers.begin();
	return -1;
}

void CProject::Play()
{
	m_timer->start(round(1000.f / Framerate()));
	CreateEvent(CProjectEvent(this, CProjectEvent::Play));
}

void CProject::Pause()
{
	m_timer->stop();
	CreateEvent(CProjectEvent(this, CProjectEvent::Pause));
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

	auto pos = ActivePos();
	CLayer* layer = new CLayer(this, Name, -1, Private, Visible);
	pos = m_layers.insert(pos, layer);
	m_undo.Push(new CUndoLayerAdd(*this, layer, pos - m_layers.begin()));
	layer->LayerEvent(CLayerEvent::Add);
	SetActiveLayer(layer);
	return layer;
}

CLayer * CProject::InsertLayer(int Index, const std::string & Name, bool Private, bool Visible)
{
	if (m_layers.size() >= MAX_LAYERS)
		return 0;
	CLayer* layer = *m_layers.insert(m_layers.begin() + Index, new CLayer(this, Name, -1, Private, Visible));
	layer->LayerEvent(CLayerEvent::Add);
	SetActiveLayer(layer);
	return layer;
}

void CProject::Duplicate(CLayer * Layer)
{
	CLayer* copy = new CLayer(*Layer, this);
	auto pos = ActivePos();
	pos = m_layers.insert(pos, copy);
	m_undo.Push(new CUndoLayerAdd(*this, copy, pos - m_layers.begin()));
	copy->LayerEvent(CLayerEvent::Add);
	SetActiveLayer(copy);
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
			int prev = IndexOf(Layer);
			m_undo.Push(new CUndoLayerDel(*this, *it, it - m_layers.begin()));
			m_layers.erase(it);
			CLayer::CreateEvent(CLayerEvent(Layer, CLayerEvent::Remove, prev));
			return true;
		}
	}
	return false;
}

bool CProject::ShiftLayer(CLayer * Layer, int Index)
{
	if (!HasLayer(Layer))
		return false;

	int prev = IndexOf(Layer);
	m_layers.insert(m_layers.begin() + Index, Layer);
	m_layers.erase(m_layers.begin() + (Index < prev ? prev + 1 : prev));
	if (IndexOf(Layer) != prev)
		CLayer::CreateEvent(CLayerEvent(Layer, CLayerEvent::Moved, prev));
	return true;
}

void CProject::Export(bool SingleFrame, bool Flatten)
{
	auto& layers = Layers();
	if (!layers.size())
		return;

	if (SingleFrame && Flatten) // Only one image file necessary
	{
		QFileDialog dlg(0, "Save images as...");
		dlg.setFileMode(QFileDialog::AnyFile);
		dlg.setAcceptMode(QFileDialog::AcceptSave);
		dlg.setNameFilters({
			"Portable Network Graphics (*.png)",
			"Joint Photographic Experts Group (*.jpg)",
			"Tagged Image File Format (*.tiff)",
			"Windows Bitmap	(*.bmp)" });

		if (!dlg.exec() || dlg.selectedFiles().length() < 1)
			return;
		Preview().save(dlg.selectedFiles()[0]);
		return;
	}

	QString dir = QFileDialog::getExistingDirectory(0, "Save images as...");
	if (dir.isEmpty())
		return;
	dir += '\\';

	int start = SingleFrame ? ActiveFrame() : 0, end = SingleFrame ? ActiveFrame() : LastFrame()->Index();
	for (int i = start; i <= end; i++)
	{
		std::string prefix = '_' + Name() + '_', suffix = std::to_string(i) + ".png";
		if (Flatten)
			Preview(i).save(dir + QString::fromStdString(prefix + suffix), "png");
		else
		{
			for (int l = 0; l < m_layers.size(); l++)
				if (m_layers[l]->IsVisible() && m_layers[l]->Pixmap(i))
					m_layers[l]->Pixmap(i)->save(dir + QString::fromStdString('L' + std::to_string(l) + prefix + m_layers[l]->Name() + '_' + suffix), "png");
		}
	}
}

void CProject::ImportSequence(int Start)
{
	QFileDialog dlg(0, "Select frame sequence");
	dlg.setFileMode(QFileDialog::FileMode::ExistingFiles);
	dlg.setAcceptMode(QFileDialog::AcceptOpen);
	dlg.setNameFilters({
		"Portable Network Graphics (*.png)",
		"Joint Photographic Experts Group (*.jpg)",
		"Tagged Image File Format (*.tiff)",
		"Windows Bitmap	(*.bmp)" });

	if (!dlg.exec() || dlg.selectedFiles().length() < 1)
		return;

	dlg.selectedFiles().sort();
	QString front = dlg.selectedFiles().front();
	int start = front.lastIndexOf('/') + 1, end = front.lastIndexOf('.');
	QStringRef name(&front, start, end - start);

	CLayer* layer = new CLayer(this, name.toString().toStdString(), 0);
	m_layers.push_front(layer);
	m_undo.Push(new CUndoLayerAdd(*this, layer, IndexOf(layer)));
	for (int i = 0; i < dlg.selectedFiles().length(); i++)
	{
		layer->AddFrame(true, Start + i);
		QPixmap* pix = layer->Pixmap(layer->Frames().size() - 1);
		QPainter paint(pix);
		paint.drawImage(QPointF(0, 0), QImage(dlg.selectedFiles()[i]));
	}
	CLayer::CreateEvent(CLayerEvent(layer, CLayerEvent::Add));
}

QImage CProject::Preview(int Frame)
{
	if (Frame == -1)
		Frame = ActiveFrame();

	QImage img = QImage(Dimensions(), QImage::Format_ARGB32);
	img.fill(Qt::transparent);

	QPainter paint(&img);
	for (auto it = Layers().rbegin(); it != Layers().rend(); it++)
	{
		if ((*it)->IsVisible() && (*it)->Pixmap(Frame))
			paint.drawPixmap(0, 0, *(*it)->Pixmap(Frame));
	}
	return img;
}

CFrame * CProject::LastFrame()
{
	CLayer* greatest = nullptr;
	for (auto layer : Layers())
	{
		if (!greatest || layer->Frames().size() > greatest->Frames().size())
			greatest = layer;
	}
	return greatest->LastFrame();
}

int CProject::EndFrame()
{
	int greatest = 0;
	for (auto layer : Layers())
	{
		if (layer->Frames().size() > greatest)
			greatest = layer->Frames().size() - 1;
	}
	return greatest;
}

void CProject::ShiftFrames(int LayerStart, int LayerEnd, int FrameStart, int FrameEnd, int Right, int Down)
{
	/*
	Don't shift if any source / destination layers do not match types (Because insane conversion would be required)

	For each layer from LayerStart - LayerEnd
	Get list of srcframes from all source layers before anything is replaced
	If no srcfame (out of range), leave null

	Get destlayer using Down


	{
		For each frame in srclayer from FrameEnd - FrameStart
		Get destframe from destlayer with index offset by Right

		If srcframe is key
			Replace srcframe in srclayer with hold
		Else if srcframe is hold and at FrameStart
			Set srcframe to key parent
		Else if destframe is hold
			Skip (Both are holds)

		
		If no srcframe, create hold
		Place srcframe

	}

	*/

	m_undo.Compound(true, "Frame shift");

	// Get list of srcframes from all source layers before anything is replaced
	std::deque<std::deque<CFrame*>> frames;
	for (int l = LayerStart; l <= LayerEnd; l++)
	{
		frames.push_back({});
		for (int f = FrameStart; f <= FrameEnd; f++)
		{
			CFrame* frame = 0;
			if (f < m_layers[l]->Frames().size()) // If no srcfame (out of range), leave null
				frame = m_layers[l]->Frames()[f];
			frames.back().push_back(frame);
		}
	}

	// For each layer from LayerStart - LayerEnd
	for (int l = LayerStart; l <= LayerEnd; l++)
	{
		// Get destlayer using Down
		CLayer* srclayer = m_layers[l], * destlayer = m_layers[l + Down];
		// For each frame in srclayer from FrameEnd - FrameStart
		for (int f = FrameStart; f <= FrameEnd; f++)
		{
			CFrame* srcframe = frames[l - LayerStart][f - FrameStart]; // Same order, elements relative to start
			CFrame* destframe = 0;
			int dst = f + Right;
			if (dst < destlayer->Frames().size()) // Get destframe
				destframe = destlayer->Frames()[dst];

			// Only replace key frames from source with holds

			if (srcframe && srcframe->IsKey()) // If srcframe is key
				srclayer->ReplaceFrame(f, srclayer->NewFrame(f == FrameStart), false); // Replace srcframe in srclayer with hold
			else if (f == FrameStart && srcframe && !srcframe->IsKey()) // Else if srcframe is hold and at FrameStart
				srcframe = srcframe->Parent(); // Set srcframe to key parent
			else if (destframe && !destframe->IsKey()) // If they are both holds
				continue;

			if (!srcframe) // If no srcframe, create hold
				srcframe = destlayer->NewFrame(false);
			destlayer->PlaceFrame(srcframe, dst); // Place srcframe
		}
	}

	m_undo.Compound(false);
}

void CProject::RemoveSelectedFrames()
{
	m_undo.Compound(true, "Delete frames");
	for (auto layer : m_layers)
		layer->RemoveSelected();
	m_undo.Compound(false);
}

void CProject::TimerEvent()
{
	// Change da world… my final message. Goodb ye.
	SetActiveFrame((ActiveFrame() + 1) % (LastFrame()->Index() + 1));
}

void CProject::PutBack(CLayer * Layer, int Index)
{
	m_layers.insert(m_layers.begin() + Index, Layer);
	Layer->LayerEvent(CLayerEvent::Add);
}

void CProject::TakeBack(int Index)
{
	auto pos = m_layers.begin() + Index;
	if (*pos == m_activelayer)
		SetActiveLayer(0);
	CLayer* layer = *pos;
	m_layers.erase(pos);
	CLayer::CreateEvent(CLayerEvent(layer, CLayerEvent::Remove, Index));
}

CProject::LayerList_t::iterator CProject::GetLayerPos(const CLayer * Layer)
{
	for (auto it = m_layers.begin(); it != m_layers.end(); it++)
		if (*it == Layer)
			return it;
	return m_layers.end();
}

CProject::LayerList_t::iterator CProject::ActivePos()
{
	if (m_activelayer)
	{
		auto pos = GetLayerPos(m_activelayer);
		if (pos != m_layers.end())
			return pos;
	}
	return m_layers.begin();
}
