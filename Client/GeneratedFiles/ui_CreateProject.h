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
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateProject
{
public:
    QWidget *w_dlg;
    QHBoxLayout *horizontalLayout;
    QPushButton *create_btn;
    QPushButton *cancel_btn;
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QSpinBox *spin_width;
    QLabel *label_2;
    QSpinBox *spin_height;
    QLineEdit *line_name;
    QLabel *label_3;

    void setupUi(QWidget *CreateProject)
    {
        if (CreateProject->objectName().isEmpty())
            CreateProject->setObjectName(QString::fromUtf8("CreateProject"));
        CreateProject->resize(211, 161);
        CreateProject->setStyleSheet(QString::fromUtf8("QWidget#CreateProject {\n"
"	background-color: rgb(50, 50, 50);\n"
"}\n"
"* {\n"
"	font-family: \"Arial\";\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QLabel {\n"
"	font-weight: bold;\n"
"	color: rgb(200, 200, 200);\n"
"}\n"
"\n"
"QCheckBox {\n"
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
        w_dlg = new QWidget(CreateProject);
        w_dlg->setObjectName(QString::fromUtf8("w_dlg"));
        w_dlg->setGeometry(QRect(0, 110, 211, 51));
        w_dlg->setStyleSheet(QString::fromUtf8(".QWidget {\n"
"	background-color: rgb(40, 40, 40);\n"
"}"));
        horizontalLayout = new QHBoxLayout(w_dlg);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        create_btn = new QPushButton(w_dlg);
        create_btn->setObjectName(QString::fromUtf8("create_btn"));
        create_btn->setMinimumSize(QSize(0, 23));

        horizontalLayout->addWidget(create_btn);

        cancel_btn = new QPushButton(w_dlg);
        cancel_btn->setObjectName(QString::fromUtf8("cancel_btn"));
        cancel_btn->setMinimumSize(QSize(0, 23));

        horizontalLayout->addWidget(cancel_btn);

        formLayoutWidget = new QWidget(CreateProject);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 191, 101));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(1, QFormLayout::LabelRole, label);

        spin_width = new QSpinBox(formLayoutWidget);
        spin_width->setObjectName(QString::fromUtf8("spin_width"));
        spin_width->setMinimumSize(QSize(0, 25));
        spin_width->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_width->setMaximum(999999999);
        spin_width->setValue(1920);

        formLayout->setWidget(1, QFormLayout::FieldRole, spin_width);

        label_2 = new QLabel(formLayoutWidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));

        formLayout->setWidget(2, QFormLayout::LabelRole, label_2);

        spin_height = new QSpinBox(formLayoutWidget);
        spin_height->setObjectName(QString::fromUtf8("spin_height"));
        spin_height->setMinimumSize(QSize(0, 25));
        spin_height->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_height->setMaximum(999999999);
        spin_height->setValue(1080);

        formLayout->setWidget(2, QFormLayout::FieldRole, spin_height);

        line_name = new QLineEdit(formLayoutWidget);
        line_name->setObjectName(QString::fromUtf8("line_name"));
        line_name->setMinimumSize(QSize(0, 25));
        line_name->setStyleSheet(QString::fromUtf8("font-weight: bold;\n"
"color: white;\n"
"font-family: \"Arial\";"));

        formLayout->setWidget(0, QFormLayout::FieldRole, line_name);

        label_3 = new QLabel(formLayoutWidget);
        label_3->setObjectName(QString::fromUtf8("label_3"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label_3);


        retranslateUi(CreateProject);

        QMetaObject::connectSlotsByName(CreateProject);
    } // setupUi

    void retranslateUi(QWidget *CreateProject)
    {
        CreateProject->setWindowTitle(QApplication::translate("CreateProject", "CreateProject", nullptr));
        create_btn->setText(QApplication::translate("CreateProject", "Create", nullptr));
        cancel_btn->setText(QApplication::translate("CreateProject", "Cancel", nullptr));
        label->setText(QApplication::translate("CreateProject", "Width", nullptr));
        label_2->setText(QApplication::translate("CreateProject", "Height", nullptr));
        line_name->setPlaceholderText(QApplication::translate("CreateProject", "New project", nullptr));
        label_3->setText(QApplication::translate("CreateProject", "Name", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CreateProject: public Ui_CreateProject {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATEPROJECT_H
