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

enum class e_export
{
	flat,
	layers
};

enum class e_layerevent
{
	null = 0,
	add,
	remove,
	switched, // When the active layer changes
	hidden, // Hidden mode toggled in layer(s)
	edit, // Layer properties changed (name, private, locked, ...)
	moved // Order in the list has changed
};
typedef std::function<void(e_layerevent)> LayerEvent_t;

class CProject
{
	std::string m_name;
	QSize m_dimensions;

	std::deque<CLayer*> m_layers;
	std::deque<CLayer*> m_pSelectedLayers;
	CLayer* m_pActiveLayer = 0;
	CUndoStack m_undo;
	static std::list<LayerEvent_t> m_listeners;

protected:
	friend class CUndoLayerDelete;
	friend class CUndoLayerAdd;

	// - Inserts an exiting layer back into the list
	void PutBack(CLayer* Layer, size_t Index);
	// - Removes an existing layer from the list
	void TakeBack(size_t Index);
	// - Returns a layer's position
	// - On failure, the end position is returned
	std::deque<CLayer*>::iterator GetLayerPos(const CLayer* Layer);

public:
	CProject(const std::string& Name, QSize Dimensions);
	~CProject();

	// - Returns the name of the project file
	inline const std::string& GetName() const { return m_name; }
	// - Sets a new name for the project file
	inline void SetName(const std::string& Name) { m_name = Name; }
	// - Returns the project dimensions in pixels
	inline const QSize& GetDimensions() const { return m_dimensions; }
	// - Sets the project dimension in pixels
	inline void SetDimensions(QSize Dimensions) { m_dimensions = Dimensions; }
	// - Returns an ordered list of the project's layers
	inline const std::deque<CLayer*>& GetLayers() const { return m_layers; }
	// - Returns a pointer to the active layer
	// - Value is nullptr if no layers or active layer exists
	inline CLayer* GetActiveLayer() const { return m_pActiveLayer; }
	// - Sets the actiove layer if it belongs to the project
	void SetActiveLayer(CLayer* Layer);
	// - Returns all selected layers
	// - When nothing is selected, an empty array is returned
	inline const std::deque<CLayer*>& GetSelectedLayers() const { return m_pSelectedLayers; }
	// - Returns the layer's index in the list
	// - UINT_MAX is returned if the list does not contain 'Layer'
	size_t GetLayerIndex(const CLayer* Layer);
	// - Returns the undo stack
	inline CUndoStack& GetUndos() { return m_undo; }

	// - Returns true if the layer exists in the project
	bool HasLayer(const CLayer* Layer);
	// - Returns true if the layer is selected
	bool IsLayerSelected(const CLayer* Layer);
	// - Sets a layer as selected
	void SelectLayer(CLayer* Layer);
	// - Adds a new layer at a max of 256
	// - Returns nullptr on failure, otherwise the new layer is given
	CLayer* AddLayer(const std::string& Name, bool Private = false, bool Visible = true);
	// - Inserts a new layer at a max of 256
	// - Returns nullptr on failure, otherwise the new layer is given
	CLayer* InsertLayer(size_t Index, const std::string& Name, bool Private = false, bool Visible = true);
	// - Removes and destroys the layer if it belongs to the project
	// - Returns false on failure
	const bool RemoveLayer(const CLayer* Layer);
	// - Shifts the layer to specified index anywhere from 0 to the list size
	// - Only works with layers within the project and returns true on success
	bool ShiftLayer(CLayer* Layer, size_t Index);
	// - Adds a pointer to your function to the listener list
	// - Listeners are called when a layer is changed or interacted with
	static inline void Listen(LayerEvent_t Func) { m_listeners.push_back(Func); }
	// - Calls all event listeners
	static void LayerEvent(e_layerevent Event);

	void Export(e_export Type);
};

#endif // CProject_H