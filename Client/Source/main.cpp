#include <QtWidgets/QApplication>
#include "Interface/MainWindow/MainWindow.h"
#include "Interface/CreateProject/CreateProject.h"
#include "Widgets/Canvas/CCanvas.h"
#include "Brushes/CBaseBrush.h"
#include "Projects/CProject.h"
#include <qfile.h>
#include <qtextstream.h>
int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	a.setStyle("Fusion");
	BrushesPrivate::CBaseBrush::InitBrushes();
	MainWindow& w = MainWindow::Get();
	w.show();
	CreateProject::Open();
	return a.exec();
}