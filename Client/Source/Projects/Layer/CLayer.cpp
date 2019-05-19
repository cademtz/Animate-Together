/*	Description:
 *		(See header)
 *
 *	Author: Hold on!
 *	Created: 2/26/2019 8:31:31 PM
 */

#include "CLayer.h"
#include "Interface/MainWindow/MainWindow.h"
#include "Interface/ColorPicker/ColorPicker.h"
#include "Projects/CProject.h"
#include "Widgets/Canvas/CCanvas.h"
#include "Widgets/LayerList/CLayerList.h"

CLayer::CLayer(CProject* Parent, const std::string & Name, QSize Dimensions, bool Private, bool Visible)
	: m_proj(Parent), m_name(Name), m_dimensions(Dimensions), m_private(Private), m_visible(Visible)
{
	m_pixmap = QPixmap(Dimensions);
	m_pixmap.fill(Qt::transparent);
}

void CLayer::Fill(QColor Color)
{
	m_proj->GetUndos().Push(new CUndoLayerFill(*GetPixmap(), Color));
	GetPixmap()->fill(Color);
}
void CLayer::Fill() {
	ColorPicker::Open(Qt::GlobalColor::white, [this](ColorPicker* picker) { Fill(picker->m_color); });
}

void CLayer::SetVisible(bool Visible)
{
	m_visible = Visible;
	CProject::LayerEvent(e_layerevent::hidden);
}