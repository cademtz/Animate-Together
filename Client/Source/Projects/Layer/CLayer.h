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

#include <qpixmap.h>

class CProject;

class CLayer
{
	CProject* m_proj;

	std::string m_name;
	QSize m_dimensions;
	QPixmap m_pixmap;
	bool m_visible = true, m_private = false;

public:
	CLayer(CProject* Parent, const std::string& Name, QSize Dimensions, bool Private = false, bool Visible = true);

	// - Fills the layer with color and alpha in 'Color'
	void Fill(QColor Color);
	// - Opens the color picker and fills the layer with the chosen color
	void Fill();

	inline QPixmap* GetPixmap() { return &m_pixmap; }
	inline bool IsVisible() const { return m_visible; }
	void SetVisible(bool Visible);
	inline bool IsPrivate() const { return m_private; }
	void SetPrivate(bool Private) { m_private = Private; }
	inline const std::string& GetName() const { return m_name; }
	inline void SetName(const std::string& Name) { m_name = Name; }
	inline CProject* GetProject() const { return m_proj; }
};

#endif // CLayer_H