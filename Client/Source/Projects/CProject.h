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
#include <functional>
#include "Layer/CLayer.h"
#include "Undo/CUndoStack.h"
#include "Palette/CPalette.h"

enum class e_export
{
	flat,
	layers
};

class CFrame;

typedef std::function<void(CProjectEvent*)> ProjectCallback_t;

class CProject
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

	size_t m_framerate, m_activeframe = 0;

	static CProject* m_activeproj;

	static std::list<ProjectCallback_t> m_listeners;
	void ProjectEvent(CProjectEvent::e_action Action);

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
	inline void SetFramerate(size_t Framerate) { m_framerate = Framerate; }

	inline size_t ActiveFrame() const { return m_activeframe; }
	void SetActiveFrame(size_t Frame);

	// - Returns an ordered list of the project's layers
	inline LayerList_t& Layers() { return m_layers; }

	// - Returns a pointer to the active layer
	// - Return is nullptr if no layers or active layer exists
	inline CLayer* ActiveLayer() const { return m_activelayer; }

	// - Sets the active layer if it belongs to the project
	void SetActiveLayer(CLayer* Layer);

	// - Returns all selected layers
	// - Returns an empty array When nothing is selected
	inline const LayerList_t& SelectedLayers() const { return m_selectedLayers; }

	// - Gets the layer's index in the list
	// - Returns UINT_MAX if the list does not contain 'Layer'
	size_t IndexOf(const CLayer* Layer);

	// - Returns the undo stack
	inline CUndoStack& Undos() { return m_undo; }


	// ========== Layer functions ========== //


	// - Returns true if the layer exists in the project
	bool HasLayer(const CLayer* Layer) const;

	// - Returns true if the layer is selected
	bool IsLayerSelected(const CLayer* Layer);

	// - Adds or removes a layer from the selected list
	// - Skips and returns false if the layer is invalid or isn't owned
	bool SelectLayer(CLayer* Layer, bool Selected = true);

	// - Adds a new layer at a max of 256
	// - Returns nullptr on failure, otherwise the new layer is given
	CLayer* AddLayer(const std::string& Name, bool Private = false, bool Visible = true);

	// - Inserts a new layer at a max of 256
	// - Returns nullptr on failure, otherwise the new layer is given
	CLayer* InsertLayer(size_t Index, const std::string& Name, bool Private = false, bool Visible = true);

	// - Adds a copy of any layer to the project
	void Duplicate(CLayer* Layer);

	// - Removes and destroys the layer if it belongs to the project
	// - Returns false on failure
	const bool RemoveLayer(CLayer* Layer);

	// - Shifts the layer to specified index anywhere from 0 to the list size
	// - Only works with layers within the project and returns true on success
	bool ShiftLayer(CLayer* Layer, size_t Index);

	// - Exports the project as the type specified
	void Export(e_export Type);


	// - Returns the foremost frame from the end of the animation
	// - Result will be null if no frames exist
	CFrame* LastFrame();


	// ========== Event functions ========== //


	// - Adds a pointer to your function to the listener list
	// - Listeners are called when the active project is changed
	static inline void Listen(ProjectCallback_t Func) { m_listeners.push_back(Func); }

protected:

	// ========== Internal functions ========== //


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