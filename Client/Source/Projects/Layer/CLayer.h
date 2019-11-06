/*	Description:
 *		Contains all info and data held in a layer.
 *		Can be manipulated through Canvas widget and other code
 *
 *	Author: Hold on!
 *	Created: 2/26/2019 8:31:20 PM
 */

#ifndef CLayer_H
#define CLayer_H
#ifdef _WIN32
#pragma once
#endif

#include <functional>
#include <deque>
#include <qpixmap.h>
#include "Projects/Events/Events.h"
#include "Projects/Frame/CRasterFrame.h"
#include <Shared/CEventHandler.h>

class CProject;

// To do: Make an abstract layer class that uses abstract frame class

class CLayer : public CEventHandler<CLayerEvent>
{
protected:
	typedef std::deque<CRasterFrame*> FrameList_t;

private:
	CProject* m_proj;

	std::string m_name;
	FrameList_t m_frames;
	std::deque<int> m_selectedframes;

	QSize m_dimensions;
	bool m_visible = true, m_private = false;

public:
	CLayer(CProject* Parent, const std::string& Name, int Frames = -1, bool Private = false, bool Visible = true);
	CLayer(const CLayer& Layer, CProject* Project);
	~CLayer();


	// ========== Layer properties ========== //


	inline bool IsVisible() const { return m_visible; }
	void SetVisible(bool Visible);

	inline bool IsPrivate() const { return m_private; }
	void SetPrivate(bool Private) { m_private = Private; }

	inline const std::string& Name() const { return m_name; }
	inline void SetName(const std::string& Name) { m_name = Name; }

	inline CProject* Project() { return m_proj; }


	// ========== Layer functions ========== //


	// - Fills the layer with color and alpha in 'Color'
	void Fill(QColor Color);

	// - Opens the color picker and fills the layer with the chosen color
	void Fill();

	int Index();


	// ========== Frame functions ========== //

	inline const FrameList_t& Frames() const { return m_frames; }
	inline const std::deque<int>& SelectedFrames() { return m_selectedframes; }

	bool IsFrameSelected(CFrame* Frame);
	bool IsFrameSelected(int Index);
	bool HasFrame(CFrame* Frame);

	// - Adds or removes a frame from the selected list
	// - Returns the frame's previous selection state
	bool SelectFrame(CFrame* Frame, bool Selected = true);
	bool SelectFrame(int Index, bool Selected = true);
	void ClearSelected();

	// - Gets the active frame
	// - Returns null if no frame is active
	CRasterFrame* ActiveFrame();	

	// - Gets the specified frame's pixmap. Defaults to active frame.
	// - Returns null if no frame is active
	QPixmap* Pixmap(int Frame = -1);

	// - Gets the frame's index in the frame list
	// - Returns -1 if the list does not contain 'Frame'
	int IndexOf(const CFrame* Frame);

	// - Adds a new frame to the layer
	// - If 'Index' is left default, the active index is used
	void AddFrame(bool IsKey, int Index = -1);

	// - Constructs a new frame with same type as the layer
	inline CFrame* NewFrame(bool IsKey) { return new CRasterFrame(0, !IsKey); }

	// - Places a key frame at Index and parents it to the layer
	void PlaceFrame(CFrame* Frame, int Index);

	// - Returns the old frame on success, else null pointer on failure.
	CFrame* ReplaceFrame(int Index, CFrame* New, bool Delete = true);
	inline void ReplaceFrame(CFrame* Old, CFrame* New, bool Delete = true) { ReplaceFrame(IndexOf(Old), New, Delete); }
	inline void RemoveFrame(CFrame* Frame) { RemoveFrame(IndexOf(Frame)); }
	void RemoveFrame(int Index);
	void RemoveSelected();

	// - Returns the frame at the end of the frame list
	// - Result will be null if no frames exist
	CFrame* LastFrame();

	// - Returns the last key frame starting at Index
	// - Result will be null if no frames exist
	CFrame* LastKey(int Index = -1);


	// ========== Event functions ========== //


	inline void LayerEvent(CLayerEvent::e_action Action) { CreateEvent(CLayerEvent(this, Action)); }

protected:
	friend class CUndoFrameReplace;


	// ========== Internal functions ========== //


	FrameList_t::iterator FramePos(const CFrame* Frame);
	inline FrameList_t::iterator FramePos(int Index) { return m_frames.begin() + Index; }
	inline int IndexOf(FrameList_t::iterator Pos) const { return Pos - m_frames.begin(); }

	friend class CUndoFrame;
	void PutBack(CFrame* Frame, int Index);
	void TakeBack(CFrame* Frame);

	// - Removes a frame directly without creating events or undos
	void _RemoveFrame(int Index);
	inline void _RemoveFrame(CFrame* Frame) { _RemoveFrame(IndexOf(Frame)); }

	// - Replaces frame at 'Index' with 'Frame' without creating undos
	// - Returns the old frame
	CFrame* _ReplaceFrame(int Index, CFrame* New);
};

#endif // CLayer_H