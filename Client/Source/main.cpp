#include <QtWidgets/QApplication>
#include "Interface/MainWindow/MainWindow.h"
#include "Interface/CreateProject/CreateProject.h"
#include "Brushes/CBaseBrush.h"

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