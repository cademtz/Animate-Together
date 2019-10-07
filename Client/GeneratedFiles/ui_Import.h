/********************************************************************************
** Form generated from reading UI file 'Import.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_IMPORT_H
#define UI_IMPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Import
{
public:
    QLabel *label;
    QSpinBox *spin_frame;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
    QLabel *label_2;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QRadioButton *r_start;
    QRadioButton *r_current;
    QRadioButton *r_end;
    QRadioButton *r_custom;

    void setupUi(QWidget *Import)
    {
        if (Import->objectName().isEmpty())
            Import->setObjectName(QString::fromUtf8("Import"));
        Import->resize(291, 141);
        Import->setStyleSheet(QString::fromUtf8("QWidget#Import {\n"
"	background-color: rgb(40, 40, 40);\n"
"}\n"
"QLabel {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 12px \"Verdana\";\n"
"}\n"
"QCheckBox, QRadioButton {\n"
"	color: rgb(225, 225, 225);\n"
"	background-color: rgb(40, 40, 40);\n"
"	font: bold 11px \"Tahoma\";\n"
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
"}\n"
"QComboBox {\n"
"	border: 1px solid #4B4B4B;\n"
"	background-color: #2C2C2C;\n"
"	color: #C8C8C8;\n"
"}\n"
"QComboBox:item {\n"
"	background-color: #1B1B1B;\n"
"}\n"
"QComboBox:item:selected {\n"
"	background-color: #0080FF;\n"
"}"));
        label = new QLabel(Import);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 71, 18));
        QSizePolicy sizePolicy(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy);
        spin_frame = new QSpinBox(Import);
        spin_frame->setObjectName(QString::fromUtf8("spin_frame"));
        spin_frame->setGeometry(QRect(220, 80, 61, 22));
        spin_frame->setMinimum(1);
        spin_frame->setMaximum(999999999);
        btn_ok = new QPushButton(Import);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        btn_ok->setGeometry(QRect(90, 110, 91, 23));
        btn_ok->setMinimumSize(QSize(75, 23));
        btn_ok->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 128, 255);\n"
"color: rgb(225, 225, 225);"));
        btn_cancel = new QPushButton(Import);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setGeometry(QRect(190, 110, 91, 23));
        btn_cancel->setMinimumSize(QSize(75, 23));
        label_2 = new QLabel(Import);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(170, 80, 51, 18));
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);
        widget = new QWidget(Import);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(90, 10, 77, 91));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        r_start = new QRadioButton(widget);
        r_start->setObjectName(QString::fromUtf8("r_start"));

        verticalLayout->addWidget(r_start);

        r_current = new QRadioButton(widget);
        r_current->setObjectName(QString::fromUtf8("r_current"));

        verticalLayout->addWidget(r_current);

        r_end = new QRadioButton(widget);
        r_end->setObjectName(QString::fromUtf8("r_end"));
        r_end->setChecked(true);

        verticalLayout->addWidget(r_end);

        r_custom = new QRadioButton(widget);
        r_custom->setObjectName(QString::fromUtf8("r_custom"));

        verticalLayout->addWidget(r_custom);


        retranslateUi(Import);

        QMetaObject::connectSlotsByName(Import);
    } // setupUi

    void retranslateUi(QWidget *Import)
    {
        Import->setWindowTitle(QApplication::translate("Import", "Import frames", nullptr));
        label->setText(QApplication::translate("Import", "Import at:", nullptr));
        btn_ok->setText(QApplication::translate("Import", "OK", nullptr));
        btn_cancel->setText(QApplication::translate("Import", "Cancel", nullptr));
#ifndef QT_NO_SHORTCUT
        btn_cancel->setShortcut(QApplication::translate("Import", "Esc", nullptr));
#endif // QT_NO_SHORTCUT
        label_2->setText(QApplication::translate("Import", "Frame", nullptr));
        r_start->setText(QApplication::translate("Import", "Start", nullptr));
        r_current->setText(QApplication::translate("Import", "Current", nullptr));
        r_end->setText(QApplication::translate("Import", "End", nullptr));
        r_custom->setText(QApplication::translate("Import", "Custom...", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Import: public Ui_Import {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_IMPORT_H
