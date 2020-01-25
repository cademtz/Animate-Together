#ifndef MainWindow_H
#define MainWindow_H
#ifdef _WIN32
#pragma once
#endif

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "Widgets/LayerPanel/CLayerPanel.h"
#include "Widgets/HistoryPanel/CHistoryPanel.h"

class CTitleBar;
class CLayer;
class CCanvas;
class CToolBar;
class CUndoAction;
class CTimeline;
class CBaseMsg;

enum class e_layerevent;
enum class e_undoevent;
enum class e_tool;

class MainWindow : public QMainWindow
{
	Q_OBJECT

	CHistoryPanel* m_history;
	CLayerPanel* m_layerpanel;
	CTitleBar* m_titlebar;
	CCanvas* m_canvas;
	CToolBar* m_toolbar;
	CTimeline* m_timeline;
	QWidget* m_client;

	QLayout* l_mainlayout;
	QVBoxLayout* l_panels, * l_client;

	static QString m_globalstyle;

	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

public:
	static MainWindow& Get()
	{
		static MainWindow inst;
		return inst;
	}

	CLayer* ActiveLayer() const;
	inline CHistoryPanel* GetHistory() const { return m_history; }
	inline CLayerPanel* GetLayerPanel() const { return m_layerpanel; }
	inline CCanvas* GetCanvas() const { return m_canvas; }
	inline CToolBar* GetToolBar() const { return m_toolbar; }
	inline static QString GetGlobalStyle() { return m_globalstyle; }
	static void TogglePanel(QWidget* Panel);
	inline static void ToggleHistory() { TogglePanel(MainWindow::Get().GetHistory()); }
	inline static void ToggleLayers() { TogglePanel(MainWindow::Get().GetLayerPanel()); }

	void UndoStackEvent(const CUndoAction* Undo);
	void ClientEvent(const CBaseMsg* Msg);

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