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

void CUndoStroke::Undo()
{
	QPixmap m_temp = m_destination.copy(m_x, m_y, m_data.width(), m_data.height()); // Store the current pixmap before our undo/redo
	QPainter paint(&m_destination);
	paint.setCompositionMode(QPainter::CompositionMode_Source); // Ensure alpha is painted over as well
	paint.drawPixmap(m_x, m_y, m_data);
	paint.end();
	m_data = m_temp; // Set our data to the temp pixmap, which now holds data to undo / redo again
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}

CUndoErase::CUndoErase(QPixmap & Source, int X, int Y, int W, int H) : m_destination(Source), m_x(X), m_y(Y) {
	m_type = e_UndoType::Erase, m_data = Source.copy(X, Y, W, H);
}

void CUndoErase::Undo()
{
	QPixmap m_temp = m_destination.copy(m_x, m_y, m_data.width(), m_data.height()); // Store the current pixmap before our undo/redo
	QPainter paint(&m_destination);
	paint.setCompositionMode(QPainter::CompositionMode_Source); // Ensure alpha is painted over as well
	paint.drawPixmap(m_x, m_y, m_data);
	paint.end();
	m_data = m_temp; // Set our data to the temp pixmap, which now holds data to undo / redo again
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}

void CUndoLayerDel::Undo()
{
	m_wasUndone ? m_proj.TakeBack(m_index) : m_proj.PutBack(m_layer, m_index);
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}

void CUndoLayerAdd::Undo()
{
	m_wasUndone ? m_proj.PutBack(m_layer, m_index) : m_proj.TakeBack(m_index);
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}

CUndoLayerFill::CUndoLayerFill(QPixmap & Destination, QColor Fill) : m_destination(Destination), m_fill(Fill) {
	m_type = e_UndoType::LayerFill, m_data = m_destination.copy();
}

void CUndoLayerFill::Undo()
{
	if (m_wasUndone)
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
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}

CUndoLayerShift::CUndoLayerShift(CProject& Project, CLayer * Layer) : m_proj(Project), m_layer(Layer) {
	m_type = e_UndoType::LayerShift, m_previndex = Project.IndexOf(Layer);
}

CUndoFrame::~CUndoFrame()
{
	if (Type() != e_UndoType::FrameDel && Undone())
		return;
	for (auto frame : m_frames)
		delete frame;
}

CUndoFrame::CUndoFrame(CLayer & Layer, CFrame * Frame, bool Added)
	: m_layer(Layer), m_frames({ Frame }), m_indexes({ Frame->Index() }) {
	m_type = Added ? e_UndoType::FrameAdd : e_UndoType::FrameDel;
}

CUndoFrame::CUndoFrame(CLayer & Layer, const std::deque<CFrame*>& Frames, bool Added)
	: m_layer(Layer), m_frames(Frames)
{
	m_type = Added ? e_UndoType::FrameAdd : e_UndoType::FrameDel;
	for (auto frame : m_frames)
		m_indexes.push_back(frame->Index());
}

void CUndoFrame::Undo()
{
	bool add = Type() == e_UndoType::FrameDel;
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
	m_wasUndone = !m_wasUndone;
	CUndoStack::UndoEvent(this);
}
