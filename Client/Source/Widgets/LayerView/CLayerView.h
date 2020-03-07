/*	Description:
 *		Provides a widget for user interaction with layers in a layer model
 *
 *	Author: Hold on!
 *	Created: 3/1/2020 6:29:14 PM
 */

#ifndef CLayerView_H
#define CLayerView_H
#ifdef _WIN32
#pragma once
#endif

#include <qtreeview.h>

class QMenu;
class CBaseMsg;
class CLayerModel;

class CLayerView : public QTreeView
{
public:
	CLayerView(QWidget* Parent = 0);

private:
	void CustomMenu(const QPoint& Pos);
	void LayerMenu(const QPoint& Pos, const QModelIndex& Index);
	void FrameMenu(const QPoint& Pos, const QModelIndex& Index);
	void ColumsInserted();

	CLayerModel* m_model;
	QMenu* m_layermenu, *m_framemenu;
	QAction* m_del, *m_moveup, *m_movedown, *m_edit;
};

#endif // CLayerView_H