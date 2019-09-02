/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/26/2019 8:31:31 PM
 */

#include "CLayer.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Interface/ColorPicker/ColorPicker.h"
#include "Projects/CProject.h"
#include "Widgets/Canvas/CCanvas.h"

std::list<LayerCallback_t> CLayer::m_listeners;

void CLayer::LayerEvent(CLayerEvent& Event)
{
	for (auto fn : m_listeners)
		fn(&Event);
}

FrameList_t::iterator CLayer::GetFramePos(const CFrame* Frame)
{
	for (auto it = m_frames.begin(); it != m_frames.end(); it++)
		if (*it == Frame)
			return it;
	return m_frames.end();
}

CLayer::CLayer(CProject* Parent, const std::string & Name, QSize Dimensions, bool Private, bool Visible)
	: m_proj(Parent), m_name(Name), m_dimensions(Dimensions), m_private(Private), m_visible(Visible)
{
	m_pixmap = QPixmap(Dimensions);
	m_pixmap.fill(Qt::transparent);
}

CLayer::~CLayer()
{
	for (auto frame : m_frames)
		delete frame;
	m_frames.clear();
}

void CLayer::Fill(QColor Color)
{
	m_proj->Undos().Push(new CUndoLayerFill(*Pixmap(), Color));
	Pixmap()->fill(Color);
}
void CLayer::Fill() {
	ColorPicker::Open(Qt::GlobalColor::white, [this](ColorPicker* picker) { Fill(picker->m_color); });
}
size_t CLayer::Index() const {
	return Project()->IndexOf(this);
}

CRasterFrame * CLayer::ActiveFrame() const
{
	if (Project()->ActiveFrame() > m_frames.size() - 1)
		return 0;
	return m_frames[Project()->ActiveFrame()];
}

size_t CLayer::IndexOf(const CFrame * Frame)
{
	auto pos = GetFramePos(Frame);
	if (pos != m_frames.end())
		return pos - m_frames.begin();
	return UINT_MAX;
}

void CLayer::AddFrame(bool IsKey, size_t Index)
{
	if (Index == UINT_MAX)
		Index = Project()->ActiveFrame();

	if (!m_frames.size())
	{
		m_frames.push_back(new CRasterFrame(this)); // The first frame must always be a key frame
		CFrame::CreateEvent(CFrameEvent(m_frames.back(), CFrameEvent::Add));
		if (!Index)
			return;
	}

	if (Index > m_frames.size() - 1) // If active index past end of list, add holds between, then push a frame back
	{
		// Grab the last frame. If we add a hold, we need to find the previous key

		CFrame* last = m_frames.back();
		for (size_t i = m_frames.size(); i < Index; i++)
		{
			m_frames.push_back(new CRasterFrame(this, last));
			CFrame::CreateEvent(CFrameEvent(m_frames.back(), CFrameEvent::Add));
		}
		m_frames.push_back(new CRasterFrame(this, IsKey ? 0 : last));
		CFrame::CreateEvent(CFrameEvent(m_frames.back(), CFrameEvent::Add));
	}
	else if (!IsKey || m_frames[Index]->State() == CFrame::Hold) // Else, if it's a hold frame
	{
		if (IsKey)	// Then replace with a new key frame
		{
			CRasterFrame** pos = &m_frames[Index], *old = *pos;
			*pos = new CRasterFrame(this);
			CFrame::CreateEvent(CFrameEvent(*pos, CFrameEvent::Replace, old));
		}
		else // Then insert a new hold frame after it
		{
			CRasterFrame* frame = new CRasterFrame(this, m_frames[Index]->Parent<CRasterFrame>());
			m_frames.insert(m_frames.begin() + Index + 1, frame);
			CFrame::CreateEvent(CFrameEvent(frame, CFrameEvent::Insert));
			Project()->SetActiveFrame(Index + 1);
		}
	}
	else // Else, move active index to next frame
	{
		Index++;
		
		if (Index > m_frames.size() - 1) // If we've reached the end, push a key frame back
		{
			m_frames.push_back(new CRasterFrame(this));
			CFrame::CreateEvent(CFrameEvent(m_frames.back(), CFrameEvent::Add));
		}
		else if (m_frames[Index]->State() == CFrame::Hold) // Else if we hit a hold frame, replace it with a key
			AddFrame(IsKey, Index);

		Project()->SetActiveFrame(Index);
	}
}

CFrame * CLayer::LastFrame() const
{
	if (m_frames.size())
		return m_frames.back();
	return 0;
}

CFrame * CLayer::LastKey() const
{
	if (CFrame* frame = LastFrame())
		return frame->State() == CFrame::Hold ? frame->Parent() : frame;
	return nullptr;
}

void CLayer::SetVisible(bool Visible)
{
	m_visible = Visible;
	LayerEvent(CLayerEvent::e_action::Hidden);
}