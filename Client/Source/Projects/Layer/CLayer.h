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

class CProject;
typedef std::function<void(CLayerEvent*)> LayerCallback_t;
typedef std::deque<CRasterFrame*> FrameList_t;

class CLayer
{
	CProject* m_proj;

	std::string m_name;
	FrameList_t m_frames;
	std::list<size_t> m_selectedindexes;

	QSize m_dimensions;
	QPixmap m_pixmap;
	bool m_visible = true, m_private = false;

	static std::list<LayerCallback_t> m_listeners;

protected:
	FrameList_t::iterator GetFramePos(const CFrame* Frame);

public:
	CLayer(CProject* Parent, const std::string& Name, QSize Dimensions, bool Private = false, bool Visible = true);
	~CLayer();


	// ========== Layer properties ========== //


	inline bool IsVisible() const { return m_visible; }
	void SetVisible(bool Visible);

	inline bool IsPrivate() const { return m_private; }
	void SetPrivate(bool Private) { m_private = Private; }

	inline const std::string& GetName() const { return m_name; }
	inline void SetName(const std::string& Name) { m_name = Name; }

	inline QPixmap* Pixmap() { return &m_pixmap; }
	inline CProject* Project() const { return m_proj; }


	// ========== Layer functions ========== //


	// - Fills the layer with color and alpha in 'Color'
	void Fill(QColor Color);

	// - Opens the color picker and fills the layer with the chosen color
	void Fill();

	size_t Index() const;


	// ========== Frame functions ========== //


	inline const FrameList_t& Frames() const { return m_frames; }
	CRasterFrame* ActiveFrame() const;	

	// - Gets the frame's index in the frame list
	// - Returns UINT_MAX if the list does not contain 'Frame'
	size_t IndexOf(const CFrame* Frame);

	// - Adds a new frame to the layer
	// - If 'Index' is left default, the active 
	void AddFrame(bool IsKey, size_t Index = UINT_MAX);

	// - Returns the frame at the end of the frame list
	// - Result will be null if no frames exist
	CFrame* LastFrame() const;

	// - Returns the last key frame
	// - Result will be null if no frames exist
	CFrame* LastKey() const;


	// ========== Event functions ========== //


	inline void LayerEvent(CLayerEvent::e_action Action) { LayerEvent(this, Action); }
	static void LayerEvent(CLayer* Layer, CLayerEvent::e_action Action);

	// - Adds a pointer to your function to the listener list
	// - Listeners are called when a layer is changed or interacted with
	static inline void Listen(LayerCallback_t Func) { m_listeners.push_back(Func); }
};

#endif // CLayer_H