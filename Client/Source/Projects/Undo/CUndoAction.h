/*	Description:
 *		Holds previous data for the program to later restore after a user has performed an action.
 *		CUndoAction is an abstract class, making one simple function for appropriately reverting any type of action.
 *		It should be assumed that any pointers given to undos which can be added and removeed will be destroyed with the undo.
 *
 *	Author: Hold on!
 *	Created: 3/2/2019 5:28:53 PM
 */

#ifndef CUndoAction_H
#define CUndoAction_H
#ifdef _WIN32
#pragma once
#endif

#include <deque>
#include <qpixmap.h>

/*constexpr const char* str_UndoType[] =
{
	"Null",
	"Compound",
	"Stroke",
	"Erase",
	"LayerDel",
	"LayerAdd",
	"LayerFill",
	"LayerShift",
	"FrameDel",
	"FrameAdd"
};*/

class CUndoAction
{
public:
	enum UndoType
	{
		Null = 0,
		Compound,
		Stroke,
		Erase,
		LayerDel,
		LayerAdd,
		LayerFill,
		LayerShift,
		FrameDel,
		FrameAdd,
		FrameReplace,
	};

public:
	// Thanks @dtugend for fixing mem leak after I forgot about virtual destructors
	virtual ~CUndoAction() { }

	// - Restores the data existing before the action was performed
	// - If 'Undone' returns true, this acts as Redo and toggles 'Undone'
	virtual void Undo() = 0;

	// - Returns true if this action is now a Redo
	// - The return value is affected by calling 'Undo'
	inline bool Undone() const { return m_undone; }

	// - Returns the type of action class
	inline UndoType Type() const { return m_type; }

	// - Returns the type of action as a string
	virtual const char* TypeStr() const = 0;

protected:
	bool m_undone = false;
	UndoType m_type;

	CUndoAction(UndoType Type) : m_type(Type) { }
};

class CCompoundAction : public CUndoAction
{
	const char* m_disguise;
	std::deque<CUndoAction*> m_actions;

protected:
	// - Creates a single undo from list 'Actions', which will be undone and redone in order
	// - 'Disguise' sets the name returned from 'TypeStr'. If left default, it will choose the action most listed
	CCompoundAction(const std::deque<CUndoAction*>& Actions, const char* Disguise = 0);
	CCompoundAction(const char* Disguise);
	~CCompoundAction();

	void Undo();
	const char* TypeStr() const { return m_disguise; }

	void Push(CUndoAction* Action);
};

class CUndoStroke : public CUndoAction
{
	QPixmap& m_destination;
	QPixmap m_data;
	int m_x, m_y;

public:
	// - Initializes an undo by storing a section of the pixmap and copying it back when 'Undo' is called
	CUndoStroke(QPixmap& Source, int X, int Y, int W, int H)
		: CUndoAction(CUndoAction::Stroke), m_destination(Source), m_x(X), m_y(Y) {
		m_data = Source.copy(X, Y, W, H);
	}

	void Undo();
	const char* TypeStr() const { return "Stroke"; }

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
	const char* TypeStr() const { return "Erase"; }

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
	CUndoLayerDel(CProject& Project, CLayer* Layer, size_t LayerIndex)
		: CUndoAction(CUndoAction::LayerDel), m_proj(Project), m_layer(Layer), m_index(LayerIndex) { }

	void Undo();
	const char* TypeStr() const { return "Delete layer"; }

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
	CUndoLayerAdd(CProject& Project, CLayer* Layer, size_t LayerIndex)
		: CUndoAction(CUndoAction::LayerAdd), m_proj(Project), m_layer(Layer), m_index(LayerIndex) { }

	void Undo();
	const char* TypeStr() const { return "Add layer"; }

	// - Returns the layer being stored
	inline const CLayer* GetLayer() const { return m_layer; }
};

class CUndoFill : public CUndoAction
{
	QPixmap& m_destination;
	QPixmap m_data;
	QColor m_fill;

public:
	CUndoFill(QPixmap& Destination, QColor Fill);
	
	void Undo();
	const char* TypeStr() const { return "Fill"; }
};

class CUndoLayerShift : public CUndoAction
{
	CProject& m_proj;
	CLayer* m_layer;
	size_t m_previndex;

public:
	CUndoLayerShift(CProject& Project, CLayer* Layer);

	const char* TypeStr() const { return "Move layer"; }
};

class CFrame;

class CUndoFrame : public CUndoAction
{
	CLayer& m_layer;
	std::deque<CFrame*> m_frames;
	std::list<size_t> m_indexes;

public:
	CUndoFrame(CLayer& Layer, CFrame* Frame, bool Added);
	CUndoFrame(CLayer& Layer, const std::deque<CFrame*>& Frames, bool Added);
	~CUndoFrame();

	void Undo();
	const char* TypeStr() const { return Type() == UndoType::FrameAdd ? "Add frame" : "Delete frame"; }
};

class CUndoFrameReplace : public CUndoAction
{
	CLayer& m_layer;
	CFrame* m_old;
	size_t m_index;

public:
	CUndoFrameReplace(CLayer& Layer, CFrame* OldFrame, size_t Index)
		: CUndoAction(UndoType::FrameReplace), m_layer(Layer), m_old(OldFrame), m_index(Index) { }
	~CUndoFrameReplace() { if (!Undone()) delete m_old; }

	void Undo();
	const char* TypeStr() const { return "Replace frame"; }
};

#endif // CUndoAction_H