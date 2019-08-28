/*	Description:
 *		Holds the user's colors 'n stuff.
 *
 *	Author: Hold on!
 *	Created: 8/15/2019 5:31:02 PM
 */

#ifndef CPalette_H
#define CPalette_H
#ifdef _WIN32
#pragma once
#endif

#include <qcolor.h>
#include <list>
#include <functional>
#include "Projects/Events/Events.h"

class CProject;
typedef std::function<void(CPaletteEvent*)> PaletteCallback_t;

class CPalette
{
	CProject* m_parent;
	QColor m_first, m_second;

	static std::list<PaletteCallback_t> m_listeners;
	void PaletteEvent();

public:
	CPalette(CProject* Parent) : m_parent(Parent) { SetDefaultPalette(); }

	// - Returns the project that holds the palette
	inline CProject* Project() const { return m_parent; }

	// - Returns the first palette color
	inline QColor First() const { return m_first; }

	// - Sets the first palette color
	void SetFirst(QColor First);

	// - Returns the second palette color
	inline QColor Second() const { return m_second; }

	// - Sets the second palette color
	void SetSecond(QColor Second);

	// - Sets the palette colors to default
	void SetDefaultPalette();

	// - Swaps the first and second palette colors;
	void Swap();

	// - Adds a pointer to your function to the listener list
	// - Listeners are called when the palette is updated
	static inline void Listen(PaletteCallback_t Callback) { m_listeners.push_back(Callback); }
};

#endif // CPalette_H