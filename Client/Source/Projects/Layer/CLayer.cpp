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

CLayer::Listeners_t CLayer::m_listeners;

CLayer::CLayer(CProject* Parent, const std::string & Name, int Frames, bool Private, bool Visible)
	: m_proj(Parent), m_name(Name), m_dimensions(Parent->Dimensions()), m_private(Private), m_visible(Visible)
{
	int end = Frames == -1 ? m_proj->ActiveFrame() : Frames;
	for (size_t i = 0; i <= end; i++)
		m_frames.push_back(new CRasterFrame(this, m_frames.size()));
}

CLayer::CLayer(const CLayer & Layer, CProject * Project) : CLayer(Layer)
{
	m_proj = Project;
	m_selectedframes.clear();
	for (auto it = m_frames.begin(); it != m_frames.end(); it++)
		*it = new CRasterFrame(**it, this);
}

CLayer::~CLayer()
{
	for (auto frame : m_frames)
		delete frame;
	m_frames.clear();
}

void CLayer::Fill(QColor Color)
{
	if (!Pixmap())
		return;

	m_proj->Undos().Push(new CUndoFill(*Pixmap(), Color));
	Pixmap()->fill(Color);
}
void CLayer::Fill()
{
	if (!Pixmap())
		return;
	ColorPicker::Open(Qt::GlobalColor::white, [this](ColorPicker* picker) { Fill(picker->m_color); });
}
size_t CLayer::Index() {
	return Project()->IndexOf(this);
}

bool CLayer::IsFrameSelected(CFrame * Frame)
{
	for (auto index : m_selectedframes)
		if (index < m_frames.size() && m_frames[index] == Frame)
			return true;
	return false;
}

bool CLayer::IsFrameSelected(int Index)
{
	for (auto index : m_selectedframes)
		if (index == Index)
			return true;
	return false;
}

bool CLayer::HasFrame(CFrame * Frame)
{
	for (auto frame : m_frames)
		if (frame == Frame)
			return true;
	return false;
}

bool CLayer::SelectFrame(CFrame * Frame, bool Selected)
{
	if (!HasFrame(Frame) && Selected)
		return false; // Only prevent selection of unowned frames

	bool result = IsFrameSelected(Frame);

	int index = IndexOf(Frame);
	if (Selected && !result)
		m_selectedframes.push_back(index);
	else
	{
		for (auto it = m_selectedframes.begin(); it != m_selectedframes.end(); it++)
		{
			if (*it == index)
			{
				m_selectedframes.erase(it);
				break;
			}
		}
	}
	LayerEvent(CLayerEvent::Selection);
	return result;
}

bool CLayer::SelectFrame(int Index, bool Selected)
{
	bool result = IsFrameSelected(Index);

	if (Selected && !result)
		m_selectedframes.push_back(Index);
	else
	{
		for (auto it = m_selectedframes.begin(); it != m_selectedframes.end(); it++)
		{
			if (*it == Index)
			{
				m_selectedframes.erase(it);
				break;
			}
		}
	}
	LayerEvent(CLayerEvent::Selection);
	return result;
}

void CLayer::ClearSelected()
{
	if (!m_selectedframes.size())
		return;

	m_selectedframes.clear();
	LayerEvent(CLayerEvent::Selection);
}

CRasterFrame * CLayer::ActiveFrame()
{
	if (m_frames.empty() || Project()->ActiveFrame() >= m_frames.size())
		return 0;
	return m_frames[Project()->ActiveFrame()];
}

QPixmap * CLayer::Pixmap(size_t Frame)
{
	CRasterFrame* frame = 0;
	if (Frame == UINT_MAX)
		frame = ActiveFrame();
	else if (Frame < Frames().size())
		frame = Frames()[Frame];

	if (!frame)
		return 0;

	if (frame->Pixmap()->isNull())
	{
		*frame->Pixmap() = QPixmap(m_dimensions);
		frame->Pixmap()->fill(Qt::transparent);
	}
	return frame->Pixmap();
}

size_t CLayer::IndexOf(const CFrame * Frame)
{
	auto pos = FramePos(Frame);
	if (pos != m_frames.end())
		return pos - m_frames.begin();
	return UINT_MAX;
}

void CLayer::AddFrame(bool IsKey, size_t Index)
{
	if (Index == UINT_MAX) // Leave it to default action
	{
		if (m_selectedframes.size())
		{
			if (IsKey)
			{
				m_proj->Undos().Compound(true, "Keyframe fill");
				while (true)
				{
					int index = -1;
					for (auto i : m_selectedframes)
					{
						if (i >= m_frames.size() || !m_frames[i]->IsKey())
						{
							index = i;
							break;
						}
					}
					if (index == -1)
						break; // No more non-key frames left

					AddFrame(true, index);
				}
				m_proj->Undos().Compound(false);
			}
			else
			{
				m_proj->Undos().Compound(true, "Add frame");
				for (auto index : m_selectedframes)
					AddFrame(false, index);
				m_proj->Undos().Compound(false);
			}
			return;
		}
		Index = Project()->ActiveFrame();
	}

	std::deque<CFrame*> added;
	if (!m_frames.size())
	{
		m_frames.push_back(new CRasterFrame(this)); // The first frame must always be a key frame
		if (!Index)
		{
			m_proj->Undos().Push(new CUndoFrame(*this, m_frames.front(), true));
			return CFrame::CreateEvent(CFrameEvent(m_frames.front(), CFrameEvent::Add));
		}
		added.push_back(m_frames.front());
	}

	if (Index > m_frames.size() - 1) // If active index past end of list, add holds between, then push a frame back
	{
		// Grab the last frame. If we add a hold, we need to find the previous key
		for (size_t i = m_frames.size(); i < Index; i++)
		{
			m_frames.push_back(new CRasterFrame(this, true));
			added.push_back(m_frames.back());
		}
		m_frames.push_back(new CRasterFrame(this, !IsKey));
		added.push_back(m_frames.back());
	}
	else if (!IsKey || !m_frames[Index]->IsKey()) // Else, if it's a hold frame
	{
		if (IsKey)	// Then replace with a new key frame
			ReplaceFrame(m_frames[Index], new CRasterFrame(this));
		else // Then insert a new hold frame after it
		{
			CRasterFrame* frame = new CRasterFrame(this, true);
			m_frames.insert(m_frames.begin() + Index + 1, frame);
			added.push_back(frame);
			Project()->SetActiveFrame(Index + 1);
		}
	}
	else // Else, move active index to next frame
	{
		Index++;
		
		if (Index > m_frames.size() - 1) // If we've reached the end, push a key frame back
		{
			m_frames.push_back(new CRasterFrame(this));
			added.push_back(m_frames.back());
		}
		else if (!m_frames[Index]->IsKey()) // Else if we hit a hold frame, replace it with a key
			AddFrame(IsKey, Index);

		Project()->SetActiveFrame(Index);
	}

	if (added.size())
	{
		Project()->Undos().Push(new CUndoFrame(*this, added, true));
		for (auto frame : added)
			CFrame::CreateEvent(CFrameEvent(frame, CFrameEvent::Add));
	}
}

CFrame * CLayer::ReplaceFrame(size_t Index, CFrame * New)
{
	CFrame* old = m_frames[Index];

	New->SetLayer(this);
	if (IsFrameSelected(old))
	{
		SelectFrame(New, true);
		SelectFrame(old, false);
	}

	m_frames[Index] = (CRasterFrame*)New;
	m_proj->Undos().Push(new CUndoFrameReplace(*this, old, Index));
	CFrame::CreateEvent(CFrameEvent(New, CFrameEvent::Replace, old, Index));
	return old;
}

void CLayer::RemoveFrame(size_t Index)
{
	CFrame* frame = m_frames[Index];
	if (frame->IsKey() && Index < m_frames.size() - 1 && !m_frames[Index + 1]->IsKey())
		++Index; // Delete the key's hold frame instead so all other holds keep the same key

	Project()->Undos().Push(new CUndoFrame(*this, frame, false));
	_RemoveFrame(Index);
	CFrame::CreateEvent(CFrameEvent(frame, CFrameEvent::Remove, 0, Index));
}

void CLayer::RemoveSelected()
{
	std::deque<CFrame*> frames;
	while (m_selectedframes.size())
	{
		CFrame* next = 0;
		int index = -1;
		for (auto i : m_selectedframes)
		{
			if (i < m_frames.size())
			{
				next = m_frames[i], index = i;
				break;
			}
		}
		if (!next)
			break; // No valid frames selected

		if (next->IsKey())
		{
			for (auto pos = ++FramePos(next); pos != m_frames.end() && !(*pos)->IsKey(); pos++)
			{
				CFrame* frame = *pos;
				
				bool selected = SelectFrame(frame, false);
				frames.push_back(frame);

				if (!selected)
				{
					SelectFrame(next, false);
					break;
				}
			}
			if (!IsFrameSelected(next))
				continue;
		}
		SelectFrame(next, false);
		frames.push_back(next);
	}

	if (!frames.size())
		return;

	Project()->Undos().Push(new CUndoFrame(*this, frames, false));
	for (auto frame : frames)
	{
		int index = IndexOf(frame);
		_RemoveFrame(frame);
		CFrame::CreateEvent(CFrameEvent(frame, CFrameEvent::Remove, 0, index));
	}
}

CFrame * CLayer::LastFrame()
{
	if (m_frames.size())
		return m_frames.back();
	return 0;
}

CFrame * CLayer::LastKey(size_t Index)
{
	if (!m_frames.size())
		return 0;

	for (auto it = m_frames.begin() + Index; it != m_frames.begin(); it--)
		if ((*it)->IsKey())
			return *it;
	return m_frames.front();
}

void CLayer::SetVisible(bool Visible)
{
	m_visible = Visible;
	LayerEvent(CLayerEvent::e_action::Hidden);
}

CLayer::FrameList_t::iterator CLayer::FramePos(const CFrame* Frame)
{
	for (auto it = m_frames.begin(); it != m_frames.end(); it++)
		if (*it == Frame)
			return it;
	return m_frames.end();
}

void CLayer::PutBack(CFrame * Frame, size_t Index)
{
	if (Index >= m_frames.size())
		m_frames.push_back((CRasterFrame*)Frame);
	else
		m_frames.insert(m_frames.begin() + Index, (CRasterFrame*)Frame);
	CFrame::CreateEvent(CFrameEvent(Frame, CFrameEvent::Add));
}

void CLayer::TakeBack(CFrame* Frame)
{
	auto pos = FramePos(Frame);
	CFrame* frame = *pos;
	size_t index = IndexOf(pos);
	m_frames.erase(pos);
	CFrame::CreateEvent(CFrameEvent(frame, CFrameEvent::Remove, 0, index));
}

void CLayer::_RemoveFrame(size_t Index)
{
	CFrame* frame = m_frames[Index];
	SelectFrame(frame, false);
	m_frames.erase(FramePos(Index));
}

CFrame* CLayer::_ReplaceFrame(int Index, CFrame * New)
{
	CFrame* old = m_frames[Index];

	New->SetLayer(this);
	if (IsFrameSelected(old))
	{
		SelectFrame(New, true);
		SelectFrame(old, false);
	}

	m_frames[Index] = (CRasterFrame*)New;
	CFrame::CreateEvent(CFrameEvent(New, CFrameEvent::Replace, old, Index));
	return old;
}
