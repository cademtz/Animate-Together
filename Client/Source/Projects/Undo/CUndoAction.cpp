/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/2/2019 9:47:56 PM
 */

#include "CUndoAction.h"
#include <qpainter.h>
#include "CUndoStack.h"
#include "Projects/CProject.h"
#include "Projects/Frame/CFrame.h"

CCompoundAction::CCompoundAction(const std::deque<CUndoAction*>& Actions, const char * Disguise)
	: CUndoAction(CUndoAction::Compound), m_actions(Actions), m_disguise(Disguise)
{
	Q_ASSERT(Actions.size());

	if (!m_disguise) // Pick a name automatically
	{
		int lastcount = 0;
		CUndoAction* last = 0;
		for (auto action : m_actions)
		{
			int count = 0;
			UndoType type = action->Type();
			if (last && type == last->Type())
				continue;

			for (auto other : m_actions)
				if (other->Type() == type)
					count++;

			if (count > lastcount)
				last = action, lastcount = count;
		}

		m_disguise = last->TypeStr(); // A champion emerges alive
	}
}

CCompoundAction::CCompoundAction(const char * Disguise)
	: CUndoAction(UndoType::Compound), m_disguise(Disguise) { }

CCompoundAction::~CCompoundAction()
{
	for (auto action : m_actions)
		delete action;
}

void CCompoundAction::Undo()
{
	for (auto it = m_actions.rbegin(); it != m_actions.rend(); it++)
		(*it)->Undo();
	m_undone = !m_undone;
}

void CCompoundAction::Push(CUndoAction * Action)
{
	if (m_undone)
	{
		for (auto action : m_actions)
			delete action;
		m_actions.clear();
	}

	m_actions.push_back(Action);
	m_undone = false;
}

void CUndoStroke::Undo()
{
	QPixmap m_temp = m_destination.copy(m_x, m_y, m_data.width(), m_data.height()); // Store the current pixmap before our undo/redo
	QPainter paint(&m_destination);
	paint.setCompositionMode(QPainter::CompositionMode_Source); // Ensure alpha is painted over as well
	paint.drawPixmap(m_x, m_y, m_data);
	paint.end();
	m_data = m_temp; // Set our data to the temp pixmap, which now holds data to undo / redo again
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

CUndoErase::CUndoErase(QPixmap & Source, int X, int Y, int W, int H)
	: CUndoAction(UndoType::Erase), m_destination(Source), m_x(X), m_y(Y) {
	m_data = Source.copy(X, Y, W, H);
}

void CUndoErase::Undo()
{
	QPixmap m_temp = m_destination.copy(m_x, m_y, m_data.width(), m_data.height()); // Store the current pixmap before our undo/redo
	QPainter paint(&m_destination);
	paint.setCompositionMode(QPainter::CompositionMode_Source); // Ensure alpha is painted over as well
	paint.drawPixmap(m_x, m_y, m_data);
	paint.end();
	m_data = m_temp; // Set our data to the temp pixmap, which now holds data to undo / redo again
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

void CUndoLayerDel::Undo()
{
	m_undone ? m_proj.TakeBack(m_index) : m_proj.PutBack(m_layer, m_index);
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

void CUndoLayerAdd::Undo()
{
	m_undone ? m_proj.PutBack(m_layer, m_index) : m_proj.TakeBack(m_index);
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

CUndoFill::CUndoFill(QPixmap & Destination, QColor Fill)
	: CUndoAction(UndoType::LayerFill), m_destination(Destination), m_fill(Fill) {
	m_data = m_destination.copy();
}

void CUndoFill::Undo()
{
	if (m_undone)
	{
		m_data = m_destination.copy();
		m_destination.fill(m_fill);
	}
	else
	{
		QPainter paint(&m_destination);
		paint.setCompositionMode(QPainter::CompositionMode_Source);
		paint.drawPixmap(0, 0, m_data);
		m_data = QPixmap(); // No need to store copy anymore
	}
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

CUndoLayerShift::CUndoLayerShift(CProject& Project, CLayer * Layer)
	: CUndoAction(UndoType::LayerShift), m_proj(Project), m_layer(Layer) {
	m_previndex = Project.IndexOf(Layer);
}


CUndoFrame::CUndoFrame(CLayer & Layer, CFrame * Frame, bool Added)
	: CUndoAction(Added ? UndoType::FrameAdd : UndoType::FrameDel), m_layer(Layer), m_frames({ Frame }), m_indexes({ Frame->Index() }) { }

CUndoFrame::CUndoFrame(CLayer & Layer, const std::deque<CFrame*>& Frames, bool Added)
	: CUndoAction(Added ? UndoType::FrameAdd : UndoType::FrameDel), m_layer(Layer), m_frames(Frames)
{
	for (auto frame : m_frames)
		m_indexes.push_back(frame->Index());
}

CUndoFrame::~CUndoFrame()
{
	if (Type() != UndoType::FrameDel || Undone())
		return;
	for (auto frame : m_frames)
		delete frame;
}

void CUndoFrame::Undo()
{
	bool add = Type() == UndoType::FrameDel;
	if (Undone())
		add = !add;

	auto itframe = m_frames.begin();
	auto itindex = m_indexes.begin();
	for (; itframe != m_frames.end(); itframe++, itindex++)
	{
		if (add)
			m_layer.PutBack(*itframe, *itindex);
		else
			m_layer.TakeBack(*itframe);
	}
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}

void CUndoFrameReplace::Undo()
{
	m_old = m_layer.ReplaceFrame(m_index, m_old);
	m_undone = !m_undone;
	CUndoStack::UndoEvent(this);
}
