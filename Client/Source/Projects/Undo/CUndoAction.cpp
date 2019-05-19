/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 3/2/2019 9:47:56 PM
 */

#include "CUndoAction.h"
#include <qpainter.h>
#include "Projects/CProject.h"
#include "CUndoStack.h"

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

void CUndoLayerDelete::Undo()
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

CUndoLayerShift::CUndoLayerShift(CProject& Project, CLayer * Layer) : m_proj(Project), m_layer(Layer)
{
	m_type = e_UndoType::LayerShift, m_previndex = Project.GetLayerIndex(Layer);
}