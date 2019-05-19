#ifndef MainWindow_H
#define MainWindow_H
#ifdef _WIN32
#pragma once
#endif

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"

class CHistory;
class CLayerList;
class CTitleBar;
class CProject;
class CLayer;
class CCanvas;
class CToolBar;
class CUndoAction;
class CTimeline;

enum class e_layerevent;
enum class e_undoevent;
enum class e_tool;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	CHistory* m_history;
	CLayerList* m_layerlist;
	CTitleBar* m_titlebar;
	CProject* m_proj;
	CCanvas* m_canvas;
	CToolBar* m_toolbar;
	CTimeline* m_timeline;

	QLayout* l_panels, * l_mainlayout;

	static QString m_globalstyle;

	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

public:
	static MainWindow& Get()
	{
		static MainWindow inst;
		return inst;
	}

	void SetActiveProj(CProject* Project);
	inline CProject* GetActiveProj() const { return m_proj; }
	CLayer* GetActiveLayer() const;
	inline CHistory* GetHistory() const { return m_history; }
	inline CLayerList* GetLayerList() const { return m_layerlist; }
	inline CCanvas* GetCanvas() const { return m_canvas; }
	inline CToolBar* GetToolBar() const { return m_toolbar; }
	inline static QString GetGlobalStyle() { return m_globalstyle; }

	void LayerEvent(e_layerevent Event);
	void UndoStackEvent(const CUndoAction* Undo);

	static void Undo();
	static void Redo();
	void FillLayer();

protected:
	void closeEvent(QCloseEvent* Event);
	void paintEvent(QPaintEvent* Event);
private:
	Ui::MainWindowClass ui;
};

#endif // MainWindow_H