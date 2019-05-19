#ifndef EditLayer_H
#define EditLayer_H
#ifdef _WIN32
#pragma once
#endif

#include <QWidget>
#include "ui_EditLayer.h"

class CProject;
class CLayer;

class EditLayer : public QWidget
{
	Q_OBJECT

public:
	// - Opens the EditLayer interface
	// - Returns false and does not show if an existing EditLayer window exists
	// @ Layer:	When set, EditLayer will modify the layer properties. Otherwise, a new layer is created
	static bool Open(CProject* Project, CLayer* Layer = 0);
	~EditLayer() { bExists = false; }

private:
	Ui::EditLayer ui;
	CProject* m_proj;
	CLayer* m_layer;
	bool m_ok = false;
	static bool bExists;

	EditLayer(CProject* Project, CLayer* Layer);

protected:
	void Confirm();
	void closeEvent(QCloseEvent* Event);
};

#endif // EditLayer_H