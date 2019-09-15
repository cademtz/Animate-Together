/*	Description:
 *		Holds previous data for the program to later restore after a user has performed an action.
 *		CUndoAction is an abstract class, making one simple function for appropriatly reverting any type of action
 *
 *	Author: Hold on!
 *	Created: 3/2/2019 5:28:53 PM
 */

#ifndef CUndoAction_H
#define CUndoAction_H
#ifdef _WIN32
#pragma once
#endif

enum class e_UndoType
{
	Null = 0,
	Stroke,
	Erase,
	LayerDel,
	LayerAdd,
	LayerFill,
	LayerShift,
	FrameDel,
	FrameAdd
};

constexpr const char* str_UndoType[] =
{
	"Null",
	"Stroke",
	"Erase",
	"LayerDel",
	"LayerAdd",
	"LayerFill",
	"LayerShift",
	"FrameDel",
	"FrameAdd"
};

class CUndoAction
{
protected:
	CUndoAction() { }
	bool m_wasUndone = false;
	e_UndoType m_type = e_UndoType::Null;

public:
	// - Credits to @dtugend for fixing mem leak after I forgot about virtual destructors
	virtual ~CUndoAction() { }

	// - Restores the data existing before the action was performed
	// - If 'Undone' returns true, this acts as Redo and toggles 'Undone'
	virtual void Undo() = 0;

	// - Returns true if this action is now a Redo
	// - The return value is affected by calling 'Undo'
	inline bool Undone() const { return m_wasUndone; }

	// - Returns the type of action class
	inline e_UndoType Type() const { return m_type; }

	// - Returns the type of action class as a string
	inline const char* TypeSTr() const { return str_UndoType[(int)m_type]; }
};

#include <qpixmap.h>
class CUndoStroke : public CUndoAction
{
	QPixmap& m_destination;
	QPixmap m_data;
	int m_x, m_y;

public:
	// - Initializes an undo by storing a section of the pixmap and copying it back when 'Undo' is called
	CUndoStroke(QPixmap& Source, int X, int Y, int W, int H) : m_destination(Source), m_x(X), m_y(Y) {
		m_type = e_UndoType::Stroke, m_data = Source.copy(X, Y, W, H);
	}

	void Undo();

	// - Returns the pixmap that will have the data restored to
	inline QPixmap GetDestination() const { return m_destination; }
	// - Sets the pixmap that will have the data restored to
	inline void SetDestination(QPixmap& Destination) { m_destination = Destination; }
};

// Same as UndoStroke for now. Behavior will change later when it must be treated like an individual stroke

class CUndoErase : public CUndoAction
{
	QPixmap& m_destination;
	QPixmap m_data;
	int m_x, m_y;

public:
	// - Initializes an undo by storing a section of the pixmap and copying it back when 'Undo' is called
	CUndoErase(QPixmap& Source, int X, int Y, int W, int H);

	void Undo();

	// - Returns the pixmap that will have the data restored to
	inline QPixmap GetDestination() const { return m_destination; }

	// - Sets the pixmap that will have the data restored to
	inline void SetDestination(QPixmap& Destination) { m_destination = Destination; }
};

class CProject;
class CLayer;

class CUndoLayerDel : public CUndoAction
{
	CProject& m_proj;
	CLayer* m_layer;
	size_t m_index;
	
public:
	~CUndoLayerDel() { if (!Undone()) delete m_layer; }
	CUndoLayerDel(CProject& Project, CLayer* Layer, size_t LayerIndex) : m_proj(Project), m_layer(Layer), m_index(LayerIndex) {
		m_type = e_UndoType::LayerDel;
	}

	void Undo();

	// - Returns the layer being stored
	inline const CLayer* GetLayer() const { return m_layer; }
};

class CUndoLayerAdd : public CUndoAction
{
	CProject& m_proj;
	CLayer* m_layer;
	size_t m_index;

public:
	~CUndoLayerAdd() { if (Undone()) delete m_layer; }
	CUndoLayerAdd(CProject& Project, CLayer* Layer, size_t LayerIndex) : m_proj(Project), m_layer(Layer), m_index(LayerIndex) {
		m_type = e_UndoType::LayerAdd;
	}

	void Undo();

	// - Returns the layer being stored
	inline const CLayer* GetLayer() const { return m_layer; }
};

class CUndoLayerFill : public CUndoAction
{
	QPixmap& m_destination;
	QPixmap m_data;
	QColor m_fill;

public:
	CUndoLayerFill(QPixmap& Destination, QColor Fill);
	
	void Undo();
};

class CUndoLayerShift : public CUndoAction
{
	CProject& m_proj;
	CLayer* m_layer;
	size_t m_previndex;

public:
	CUndoLayerShift(CProject& Project, CLayer* Layer);
};

class CFrame;

#include <deque>
class CUndoFrame : public CUndoAction
{
	CLayer& m_layer;
	std::deque<CFrame*> m_frames;
	std::list<size_t> m_indexes;

public:
	~CUndoFrame();
	CUndoFrame(CLayer& Layer, CFrame* Frame, bool Added);
	CUndoFrame(CLayer& Layer, const std::deque<CFrame*>& Frames, bool Added);

	void Undo();
};

#endif // CUndoAction_H