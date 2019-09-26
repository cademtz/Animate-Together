/********************************************************************************
** Form generated from reading UI file 'Export.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EXPORT_H
#define UI_EXPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Export
{
public:
    QPushButton *btn_export;
    QPushButton *btn_cancel;
    QWidget *widget;
    QGridLayout *gridLayout;
    QLabel *label_2;
    QComboBox *cmb_export;
    QComboBox *cmb_format;
    QLabel *label;

    void setupUi(QWidget *Export)
    {
        if (Export->objectName().isEmpty())
            Export->setObjectName(QString::fromUtf8("Export"));
        Export->resize(311, 121);
        Export->setStyleSheet(QString::fromUtf8("QWidget#Export {\n"
"	background-color: rgb(40, 40, 40);\n"
"}\n"
"QLabel {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 12px \"Verdana\";\n"
"}\n"
"QCheckBox {\n"
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
        btn_export = new QPushButton(Export);
        btn_export->setObjectName(QString::fromUtf8("btn_export"));
        btn_export->setGeometry(QRect(110, 90, 91, 23));
        btn_export->setMinimumSize(QSize(75, 23));
        btn_export->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 128, 255);\n"
"color: rgb(225, 225, 225);"));
        btn_cancel = new QPushButton(Export);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setGeometry(QRect(210, 90, 91, 23));
        btn_cancel->setMinimumSize(QSize(75, 23));
        widget = new QWidget(Export);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setGeometry(QRect(10, 10, 291, 71));
        gridLayout = new QGridLayout(widget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(label_2->sizePolicy().hasHeightForWidth());
        label_2->setSizePolicy(sizePolicy);

        gridLayout->addWidget(label_2, 1, 0, 1, 1);

        cmb_export = new QComboBox(widget);
        cmb_export->setObjectName(QString::fromUtf8("cmb_export"));

        gridLayout->addWidget(cmb_export, 0, 1, 1, 1);

        cmb_format = new QComboBox(widget);
        cmb_format->setObjectName(QString::fromUtf8("cmb_format"));

        gridLayout->addWidget(cmb_format, 1, 1, 1, 1);

        label = new QLabel(widget);
        label->setObjectName(QString::fromUtf8("label"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(label->sizePolicy().hasHeightForWidth());
        label->setSizePolicy(sizePolicy1);

        gridLayout->addWidget(label, 0, 0, 1, 1);


        retranslateUi(Export);

        QMetaObject::connectSlotsByName(Export);
    } // setupUi

    void retranslateUi(QWidget *Export)
    {
        Export->setWindowTitle(QApplication::translate("Export", "Export project", nullptr));
        btn_export->setText(QApplication::translate("Export", "OK", nullptr));
        btn_cancel->setText(QApplication::translate("Export", "Cancel", nullptr));
        label_2->setText(QApplication::translate("Export", "Format", nullptr));
        label->setText(QApplication::translate("Export", "Export", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Export: public Ui_Export {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EXPORT_H
