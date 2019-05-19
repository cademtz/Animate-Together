/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindowClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *tool_stack;
    QFrame *mainframe;
    QWidget *w_panels;

    void setupUi(QMainWindow *MainWindowClass)
    {
        if (MainWindowClass->objectName().isEmpty())
            MainWindowClass->setObjectName(QString::fromUtf8("MainWindowClass"));
        MainWindowClass->resize(901, 440);
        MainWindowClass->setStyleSheet(QString::fromUtf8("QMainWindow, .QFrame {\n"
"	background-color: rgb(50, 50, 50);\n"
"}"));
        centralWidget = new QWidget(MainWindowClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        tool_stack = new QVBoxLayout(centralWidget);
        tool_stack->setSpacing(0);
        tool_stack->setContentsMargins(11, 11, 11, 11);
        tool_stack->setObjectName(QString::fromUtf8("tool_stack"));
        tool_stack->setContentsMargins(1, 1, 1, 1);
        mainframe = new QFrame(centralWidget);
        mainframe->setObjectName(QString::fromUtf8("mainframe"));
        mainframe->setStyleSheet(QString::fromUtf8("QCheckBox {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 500 8pt \"Arial\";\n"
"}\n"
"QPushButton, QLineEdit, QSpinBox {\n"
"	border: 1px solid rgb(75, 75, 75);\n"
"	background-color: rgb(35, 35, 35);\n"
"	color: rgb(200, 200, 200);\n"
"}\n"
"QPushButton:hover, QLineEdit:focus, QSpinBox:focus {\n"
"	border-color: rgb(0, 128, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover:pressed {\n"
"	background-color: rgb(25, 25, 25);\n"
"}"));
        mainframe->setFrameShape(QFrame::StyledPanel);
        mainframe->setFrameShadow(QFrame::Raised);
        w_panels = new QWidget(mainframe);
        w_panels->setObjectName(QString::fromUtf8("w_panels"));
        w_panels->setGeometry(QRect(780, 0, 100, 439));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(w_panels->sizePolicy().hasHeightForWidth());
        w_panels->setSizePolicy(sizePolicy);
        w_panels->setMinimumSize(QSize(100, 0));

        tool_stack->addWidget(mainframe);

        MainWindowClass->setCentralWidget(centralWidget);

        retranslateUi(MainWindowClass);

        QMetaObject::connectSlotsByName(MainWindowClass);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindowClass)
    {
        MainWindowClass->setWindowTitle(QApplication::translate("MainWindowClass", "Animate Together", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindowClass: public Ui_MainWindowClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
