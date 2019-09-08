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
#include "Projects/Events/CEventHandler.h"

class CProject;
typedef std::function<void(CLayerEvent*)> LayerCallback_t;
typedef std::deque<CRasterFrame*> FrameList_t;

// To do: Make an abstract layer class that uses abstract frame class

class CLayer : public CEventHandler<CLayerEvent>
{
	CProject* m_proj;

	std::string m_name;
	FrameList_t m_frames;
	std::deque<CFrame*> m_selectedframes;

	QSize m_dimensions;
	bool m_visible = true, m_private = false;

	//static std::list<LayerCallback_t> m_listeners;

protected:
	FrameList_t::iterator FramePos(const CFrame* Frame);

public:
	CLayer(CProject* Parent, const std::string& Name, QSize Dimensions, bool Private = false, bool Visible = true);
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

	size_t Index();


	// ========== Frame functions ========== //

	inline const FrameList_t& Frames() const { return m_frames; }
	inline std::deque<CFrame*>& SelectedFrames() { return m_selectedframes; }

	bool IsFrameSelected(CFrame* Frame);
	bool HasFrame(CFrame* Frame);

	// - Adds or removes a frame from the selected list
	// - Skips and returns false if the frame is invalid or isn't owned
	bool SelectFrame(CFrame* Frame, bool Selected = true);
	inline void ClearSelected() { m_selectedframes.clear(); }

	// - Gets the active frame
	// - Returns null if no frame is active
	CRasterFrame* ActiveFrame();	

	// - Gets the active frame's pixmap
	// - Returns null if no frame is active
	QPixmap* Pixmap();

	// - Gets the frame's index in the frame list
	// - Returns UINT_MAX if the list does not contain 'Frame'
	size_t IndexOf(const CFrame* Frame);

	// - Adds a new frame to the layer
	// - If 'Index' is left default, the active index is used
	void AddFrame(bool IsKey, size_t Index = UINT_MAX);

	void RemoveFrame(CFrame* Frame);

	// - Returns the frame at the end of the frame list
	// - Result will be null if no frames exist
	CFrame* LastFrame();

	// - Returns the last key frame starting at Index
	// - Result will be null if no frames exist
	CFrame* LastKey(size_t Index = UINT_MAX);


	// ========== Event functions ========== //


	inline void LayerEvent(CLayerEvent::e_action Action) { CreateEvent(CLayerEvent(this, Action)); }
};

#endif // CLayer_H