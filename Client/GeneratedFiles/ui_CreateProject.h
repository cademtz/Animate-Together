/********************************************************************************
** Form generated from reading UI file 'CreateProject.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATEPROJECT_H
#define UI_CREATEPROJECT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateProject
{
public:
    QPushButton *create_btn;
    QPushButton *cancel_btn;
    QLabel *label_3;
    QLineEdit *line_name;
    QLabel *label_4;
    QSpinBox *spin_width;
    QSpinBox *spin_height;

    void setupUi(QWidget *CreateProject)
    {
        if (CreateProject->objectName().isEmpty())
            CreateProject->setObjectName(QString::fromUtf8("CreateProject"));
        CreateProject->resize(211, 231);
        CreateProject->setStyleSheet(QString::fromUtf8("QWidget#CreateProject {\n"
"	background-color: rgb(40, 40, 40);\n"
"}\n"
"* {\n"
"	font-family: \"Arial\";\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QLabel {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 16px \"Verdana\"\n"
"}\n"
"\n"
"QCheckBox {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 500 8pt \"Arial\";\n"
"}\n"
"QPushButton, QLineEdit, QSpinBox {\n"
"	border: none;\n"
"	background-color: rgb(30, 30, 30);\n"
"	color: rgb(200, 200, 200);\n"
"}\n"
"QPushButton:hover, QLineEdit:focus, QSpinBox:focus {\n"
"	border-color: rgb(0, 128, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover:pressed {\n"
"	background-color: rgb(25, 25, 25);\n"
"}"));
        create_btn = new QPushButton(CreateProject);
        create_btn->setObjectName(QString::fromUtf8("create_btn"));
        create_btn->setGeometry(QRect(10, 200, 91, 23));
        create_btn->setMinimumSize(QSize(0, 23));
        create_btn->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 128, 255);\n"
"color: rgb(225, 225, 225);"));
        cancel_btn = new QPushButton(CreateProject);
        cancel_btn->setObjectName(QString::fromUtf8("cancel_btn"));
        cancel_btn->setGeometry(QRect(110, 200, 91, 23));
        cancel_btn->setMinimumSize(QSize(0, 23));
        label_3 = new QLabel(CreateProject);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 10, 191, 21));
        label_3->setAlignment(Qt::AlignCenter);
        line_name = new QLineEdit(CreateProject);
        line_name->setObjectName(QString::fromUtf8("line_name"));
        line_name->setGeometry(QRect(10, 34, 191, 31));
        line_name->setMinimumSize(QSize(0, 25));
        line_name->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"color: white;\n"
"font-family: \"Arial\";"));
        label_4 = new QLabel(CreateProject);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 80, 191, 22));
        label_4->setAlignment(Qt::AlignCenter);
        spin_width = new QSpinBox(CreateProject);
        spin_width->setObjectName(QString::fromUtf8("spin_width"));
        spin_width->setGeometry(QRect(10, 104, 191, 31));
        spin_width->setMinimumSize(QSize(0, 25));
        spin_width->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_width->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        spin_width->setMinimum(1);
        spin_width->setMaximum(999999999);
        spin_width->setValue(1920);
        spin_height = new QSpinBox(CreateProject);
        spin_height->setObjectName(QString::fromUtf8("spin_height"));
        spin_height->setGeometry(QRect(10, 140, 191, 31));
        spin_height->setMinimumSize(QSize(0, 25));
        spin_height->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_height->setCorrectionMode(QAbstractSpinBox::CorrectToNearestValue);
        spin_height->setMinimum(1);
        spin_height->setMaximum(999999999);
        spin_height->setValue(1080);

        retranslateUi(CreateProject);

        QMetaObject::connectSlotsByName(CreateProject);
    } // setupUi

    void retranslateUi(QWidget *CreateProject)
    {
        CreateProject->setWindowTitle(QApplication::translate("CreateProject", "New project", nullptr));
        create_btn->setText(QApplication::translate("CreateProject", "Create", nullptr));
        cancel_btn->setText(QApplication::translate("CreateProject", "Cancel", nullptr));
        label_3->setText(QApplication::translate("CreateProject", "Name", nullptr));
        line_name->setPlaceholderText(QApplication::translate("CreateProject", "New project", nullptr));
        label_4->setText(QApplication::translate("CreateProject", "Dimensions", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateProject: public Ui_CreateProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEPROJECT_H
