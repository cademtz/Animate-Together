/********************************************************************************
** Form generated from reading UI file 'EditBrush.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITBRUSH_H
#define UI_EDITBRUSH_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditBrush
{
public:
    QSlider *slide_min;
    QDoubleSpinBox *spin_min;
    QDoubleSpinBox *spin_max;
    QSlider *slide_max;
    QSpinBox *spin_hard;
    QSlider *slide_hard;

    void setupUi(QWidget *EditBrush)
    {
        if (EditBrush->objectName().isEmpty())
            EditBrush->setObjectName(QString::fromUtf8("EditBrush"));
        EditBrush->resize(201, 151);
        EditBrush->setStyleSheet(QString::fromUtf8("#EditBrush {\n"
"	background-color: rgb(40, 40, 40);\n"
"}\n"
"* {\n"
"	font-family: \"Arial\";\n"
"	font-weight: bold;\n"
"}\n"
"QCheckBox {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 500 8pt \"Arial\";\n"
"}\n"
"QPushButton, QLineEdit, QSpinBox, QDoubleSpinBox {\n"
"	border: none;\n"
"	background-color: rgb(30, 30, 30);\n"
"	color: rgb(200, 200, 200);\n"
"}\n"
"QPushButton:hover, QLineEdit:focus, QSpinBox:focus, QDoubleSpinBox:focus {\n"
"	border-color: rgb(0, 128, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover:pressed {\n"
"	background-color: rgb(25, 25, 25);\n"
"}\n"
"\n"
"QSlider:disabled {\n"
"  background: #252525;\n"
"}\n"
"\n"
"QSlider:focus {\n"
"  border: none;\n"
"}\n"
"\n"
"QSlider::groove:horizontal {\n"
"  background: #505050;\n"
"  border: none;\n"
"  height: 2px;\n"
"  margin: 0px;\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::groove:vertical {\n"
"  background: #505050;\n"
"  border: none;\n"
"  width: 2px;\n"
"  margin: 0px;\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"QSl"
                        "ider::add-page:vertical {\n"
"  background: #808080;\n"
"  border: 1px solid #32414B;\n"
"  width: 4px;\n"
"  margin: 0px;\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"QSlider::add-page:vertical :disabled {\n"
"  background: #14506E;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal {\n"
"  background: #808080;\n"
"  height: 4px;\n"
"  margin: 0px;\n"
"}\n"
"\n"
"QSlider::sub-page:horizontal:disabled {\n"
"  background: #14506E;\n"
"}\n"
"\n"
"QSlider::handle:horizontal {\n"
"  background: #323232;\n"
"  border: 2px solid #909090;\n"
"  width: 11px;\n"
"  height: 9px;\n"
"  margin: -7px 0px;\n"
"  border-radius: 7px;\n"
"}\n"
"\n"
"QSlider::handle:horizontal:hover {\n"
"  border: 2px solid #cccccc;\n"
"}\n"
"\n"
"QSlider::handle:vertical {\n"
"  background: #323232;\n"
"  border: 2px solid #909090;\n"
"  width: 9px;\n"
"  height: 11px;\n"
"  margin: 0px -7px;\n"
"  border-radius: 7px;\n"
"}\n"
"\n"
"QSlider::handle:vertical:hover {\n"
"  border: 2px solid #cccccc;\n"
"}\n"
"\n"
"QScrollBar:horizontal {\n"
"  height: 16p"
                        "x;\n"
"  margin: 2px 0px 2px 0px;\n"
"  background: #282828;\n"
"  border: 1px solid #323232;\n"
"}\n"
"\n"
"QScrollBar:vertical {\n"
"  background: #282828;\n"
"  border: 1px solid #323232;\n"
"  width: 16px;\n"
"  margin: 0px 2px 0px 2px;\n"
"}\n"
"\n"
"QScrollBar::handle:horizontal {\n"
"  margin: 1px;\n"
"  background-color: #404040;\n"
"  min-width: 8px;\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::handle:vertical {\n"
"  margin: 1px;\n"
"  background-color: #404040;\n"
"  min-height: 8px;\n"
"  border-radius: 4px;\n"
"}\n"
"\n"
"QScrollBar::add-line:horizontal {\n"
"  border-image: url(\":/qss_icons/rc/right_arrow_disabled.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: right;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::add-line:horizontal:hover, QScrollBar::add-line:horizontal:on {\n"
"  border-image: url(\":/qss_icons/rc/right_arrow.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: right;\n"
"  subcontrol-origin: margin;\n"
"}\n"
""
                        "\n"
"QScrollBar::add-line:vertical {\n"
"  border-image: url(\":/qss_icons/rc/down_arrow_disabled.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: bottom;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::add-line:vertical:hover, QScrollBar::add-line:vertical:on {\n"
"  border-image: url(\":/qss_icons/rc/down_arrow.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: bottom;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:horizontal {\n"
"  border-image: url(\":/qss_icons/rc/left_arrow_disabled.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: left;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:horizontal:hover, QScrollBar::sub-line:horizontal:on {\n"
"  border-image: url(\":/qss_icons/rc/left_arrow.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: left;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:vertical {\n"
"  border-image: url(\":/qss_"
                        "icons/rc/up_arrow_disabled.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: top;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::sub-line:vertical:hover, QScrollBar::sub-line:vertical:on {\n"
" border-image: url(\":/qss_icons/rc/up_arrow.png\");\n"
"  height: 10px;\n"
"  width: 10px;\n"
"  subcontrol-position: top;\n"
"  subcontrol-origin: margin;\n"
"}\n"
"\n"
"QScrollBar::up-arrow:horizontal, QScrollBar::down-arrow:horizontal {\n"
"  background: none;\n"
"}\n"
"\n"
"QScrollBar::up-arrow:vertical, QScrollBar::down-arrow:vertical {\n"
"  background: none;\n"
"}\n"
"\n"
"QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {\n"
"  background: none;\n"
"}\n"
"\n"
"QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {\n"
"  background: none;\n"
"}"));
        slide_min = new QSlider(EditBrush);
        slide_min->setObjectName(QString::fromUtf8("slide_min"));
        slide_min->setGeometry(QRect(80, 80, 111, 22));
        slide_min->setMaximum(100);
        slide_min->setOrientation(Qt::Horizontal);
        spin_min = new QDoubleSpinBox(EditBrush);
        spin_min->setObjectName(QString::fromUtf8("spin_min"));
        spin_min->setGeometry(QRect(10, 80, 61, 26));
        spin_min->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_min->setMinimum(0.010000000000000);
        spin_min->setMaximum(1000.000000000000000);
        spin_max = new QDoubleSpinBox(EditBrush);
        spin_max->setObjectName(QString::fromUtf8("spin_max"));
        spin_max->setGeometry(QRect(10, 110, 61, 26));
        spin_max->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_max->setMinimum(0.010000000000000);
        spin_max->setMaximum(1000.000000000000000);
        slide_max = new QSlider(EditBrush);
        slide_max->setObjectName(QString::fromUtf8("slide_max"));
        slide_max->setGeometry(QRect(80, 110, 111, 22));
        slide_max->setMaximum(100);
        slide_max->setOrientation(Qt::Horizontal);
        spin_hard = new QSpinBox(EditBrush);
        spin_hard->setObjectName(QString::fromUtf8("spin_hard"));
        spin_hard->setGeometry(QRect(10, 10, 181, 26));
        spin_hard->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_hard->setMaximum(100);
        slide_hard = new QSlider(EditBrush);
        slide_hard->setObjectName(QString::fromUtf8("slide_hard"));
        slide_hard->setEnabled(false);
        slide_hard->setGeometry(QRect(10, 40, 181, 22));
        slide_hard->setMaximum(100);
        slide_hard->setOrientation(Qt::Horizontal);

        retranslateUi(EditBrush);

        QMetaObject::connectSlotsByName(EditBrush);
    } // setupUi

    void retranslateUi(QWidget *EditBrush)
    {
        EditBrush->setWindowTitle(QApplication::translate("EditBrush", "EditBrush", nullptr));
        spin_min->setPrefix(QApplication::translate("EditBrush", "Min: ", nullptr));
        spin_max->setPrefix(QApplication::translate("EditBrush", "Max: ", nullptr));
        spin_hard->setPrefix(QApplication::translate("EditBrush", "Hardness: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditBrush: public Ui_EditBrush {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITBRUSH_H
