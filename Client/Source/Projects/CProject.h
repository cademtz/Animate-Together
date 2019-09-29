/*	Description:
 *		Stores all variables and information held in a project file.
 *		Pretty much the meat of the entire program and what the interface is based around controlling.
 *
 *	Author: Hold on!
 *	Created: 2/25/2019 7:11:53 PM
 */

#ifndef CProject_H
#define CProject_H
#ifdef _WIN32
#pragma once
#endif

#define MAX_LAYERS 256

#include <deque>
#include <list>
#include <qtimer.h>
#include <functional>
#include "Layer/CLayer.h"
#include "Undo/CUndoStack.h"
#include "Palette/CPalette.h"
#include "Events/CEventHandler.h"

enum class e_export
{
	flat,
	layers,
	sequence
};

class CFrame;

class CProject : public CEventHandler<CProjectEvent>
{
protected:
	typedef std::deque<CLayer*> LayerList_t;

private:
	std::string m_name;
	QSize m_dimensions;
	CPalette m_palette;

	LayerList_t m_layers, m_selectedLayers;
	CLayer* m_activelayer = 0;
	CUndoStack m_undo;

	size_t m_framerate = 24, m_activeframe = 0;
	QTimer* m_timer;

	static CProject* m_activeproj;

public:
	CProject(const std::string& Name, QSize Dimensions);
	~CProject();


	// ========== Global properties ========== //


	// - Gets a pointer to the active project
	// - Returns nullptr if no project is active
	static inline CProject* ActiveProject() { return m_activeproj; }
	static void SetActiveProject(CProject* Project);


	// ========== Project properties ========== //


	inline const std::string& Name() const { return m_name; }
	inline void SetName(const std::string& Name) { m_name = Name; }

	inline const QSize& Dimensions() const { return m_dimensions; }
	inline void SetDimensions(QSize Dimensions) { m_dimensions = Dimensions; }

	inline CPalette& Palette() { return m_palette; }
	inline void SetPalette(const CPalette& Palette) { m_palette = Palette; }

	inline size_t Framerate() const { return m_framerate; }
	void SetFramerate(size_t Framerate);

	inline size_t ActiveFrame() const { return m_activeframe; }
	void SetActiveFrame(size_t Frame);

	inline bool IsPlaying() const { return m_timer->isActive(); }

	inline LayerList_t& Layers() { return m_layers; }

	// - Returns a pointer to the active layer, or null if none are active
	inline CLayer* ActiveLayer() const { return m_activelayer; }

	// - Sets the active layer if it belongs to the project
	void SetActiveLayer(CLayer* Layer);

	inline const LayerList_t& SelectedLayers() const { return m_selectedLayers; }

	// - Gets the layer's index in the list
	// - Returns UINT_MAX if the list does not contain 'Layer'
	size_t IndexOf(const CLayer* Layer);

	inline CUndoStack& Undos() { return m_undo; }


	// ========== Project functions ========== //


	void Play();
	void Pause();

	void Export(bool SingleFrame, bool Flatten);

	// - Returns an image of all the layers in the specified frame combined
	// - 'Frame' defaults to active
	QImage Preview(size_t Frame = UINT_MAX);


	// ========== Layer functions ========== //


	// - Returns true if the layer exists in the project
	bool HasLayer(const CLayer* Layer) const;

	bool IsLayerSelected(const CLayer* Layer);

	// - Adds or removes a layer from the selected list
	// - Skips and returns false if the layer is invalid or isn't owned
	bool SelectLayer(CLayer* Layer, bool Selected = true);

	// - Returns null on failure, otherwise a new layer is added and returned
	CLayer* AddLayer(const std::string& Name, bool Private = false, bool Visible = true);

	// - Returns null on failure, otherwise a new layer is inerted and returned
	CLayer* InsertLayer(size_t Index, const std::string& Name, bool Private = false, bool Visible = true);

	// - Adds a copy of any layer to the project
	void Duplicate(CLayer* Layer);

	// - Removes and destroys the layer if it belongs to the project
	// - Returns false on failure
	const bool RemoveLayer(CLayer* Layer);

	// - Shifts the layer to specified index anywhere from 0 to the list size
	// - Only works with layers within the project and returns true on success
	bool ShiftLayer(CLayer* Layer, size_t Index);

	// - Returns the foremost frame from the end of the animation
	// - Result will be null if no frames exist
	CFrame* LastFrame();


	// ========== Frame functions ========== //


	inline void DeselectFrames() {
		for (auto layer : Layers())
			layer->ClearSelected();
	}
	inline void RemoveSelectedFrames() {
		for (auto layer : Layers())
			layer->RemoveSelected();
	}

protected:

	// ========== Internal functions ========== //


	void TimerEvent();

	friend class CUndoLayerDel;
	friend class CUndoLayerAdd;

	// - Inserts an existing layer back into the list
	void PutBack(CLayer* Layer, size_t Index);

	// - Removes an existing layer from the list
	void TakeBack(size_t Index);

	// - Returns a layer's position
	// - On failure, the end position is returned
	LayerList_t::iterator GetLayerPos(const CLayer* Layer);

	LayerList_t::iterator ActivePos();
};

#endif // CProject_H