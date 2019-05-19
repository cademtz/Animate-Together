/********************************************************************************
** Form generated from reading UI file 'ColorPicker.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_COLORPICKER_H
#define UI_COLORPICKER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ColorPicker
{
public:
    QFrame *frame;
    QWidget *layoutWidget;
    QVBoxLayout *l_dialogbtns;
    QPushButton *btn_revert;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *l_preview;
    QWidget *verticalLayoutWidget_3;
    QVBoxLayout *l_hueslide;
    QFrame *f_huebox;
    QWidget *formLayoutWidget;
    QFormLayout *rgba_layout;
    QLabel *label_red;
    QLabel *label_blue;
    QLabel *label_green;
    QSpinBox *spin_red;
    QSpinBox *spin_green;
    QSpinBox *spin_blue;
    QSpinBox *spin_alpha;
    QSpinBox *spin_hex;
    QLabel *label_alpha;
    QLabel *label_hex;
    QLabel *label_hue;
    QLabel *label_sat;
    QLabel *label_val;
    QSpinBox *spin_hue;
    QSpinBox *spin_sat;
    QSpinBox *spin_val;

    void setupUi(QWidget *ColorPicker)
    {
        if (ColorPicker->objectName().isEmpty())
            ColorPicker->setObjectName(QString::fromUtf8("ColorPicker"));
        ColorPicker->resize(491, 301);
        ColorPicker->setStyleSheet(QString::fromUtf8("* {\n"
"	font-family: \"Arial\";\n"
"	font-weight: bold;\n"
"}\n"
"\n"
"QWidget#ColorPicker {\n"
"	background-color: rgb(50, 50, 50);\n"
"}\n"
"QLabel, QCheckBox {\n"
"	color: rgb(225, 225, 225);\n"
"	font-size: 10pt;\n"
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
        frame = new QFrame(ColorPicker);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(380, 0, 111, 301));
        frame->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	background-color: rgb(40, 40, 40);\n"
"}"));
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 210, 91, 83));
        l_dialogbtns = new QVBoxLayout(layoutWidget);
        l_dialogbtns->setSpacing(6);
        l_dialogbtns->setContentsMargins(11, 11, 11, 11);
        l_dialogbtns->setObjectName(QString::fromUtf8("l_dialogbtns"));
        l_dialogbtns->setContentsMargins(0, 0, 0, 0);
        btn_revert = new QPushButton(layoutWidget);
        btn_revert->setObjectName(QString::fromUtf8("btn_revert"));
        btn_revert->setMinimumSize(QSize(75, 23));

        l_dialogbtns->addWidget(btn_revert);

        btn_ok = new QPushButton(layoutWidget);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        btn_ok->setMinimumSize(QSize(75, 23));

        l_dialogbtns->addWidget(btn_ok);

        btn_cancel = new QPushButton(layoutWidget);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setMinimumSize(QSize(75, 23));

        l_dialogbtns->addWidget(btn_cancel);

        verticalLayoutWidget = new QWidget(frame);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(10, 10, 91, 191));
        l_preview = new QVBoxLayout(verticalLayoutWidget);
        l_preview->setSpacing(6);
        l_preview->setContentsMargins(11, 11, 11, 11);
        l_preview->setObjectName(QString::fromUtf8("l_preview"));
        l_preview->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_3 = new QWidget(ColorPicker);
        verticalLayoutWidget_3->setObjectName(QString::fromUtf8("verticalLayoutWidget_3"));
        verticalLayoutWidget_3->setGeometry(QRect(10, 270, 261, 21));
        l_hueslide = new QVBoxLayout(verticalLayoutWidget_3);
        l_hueslide->setSpacing(6);
        l_hueslide->setContentsMargins(11, 11, 11, 11);
        l_hueslide->setObjectName(QString::fromUtf8("l_hueslide"));
        l_hueslide->setContentsMargins(0, 0, 0, 0);
        f_huebox = new QFrame(ColorPicker);
        f_huebox->setObjectName(QString::fromUtf8("f_huebox"));
        f_huebox->setGeometry(QRect(10, 10, 257, 257));
        f_huebox->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	border: 1px solid black;\n"
"}"));
        f_huebox->setFrameShape(QFrame::StyledPanel);
        f_huebox->setFrameShadow(QFrame::Raised);
        formLayoutWidget = new QWidget(ColorPicker);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(270, 10, 106, 281));
        rgba_layout = new QFormLayout(formLayoutWidget);
        rgba_layout->setSpacing(6);
        rgba_layout->setContentsMargins(11, 11, 11, 11);
        rgba_layout->setObjectName(QString::fromUtf8("rgba_layout"));
        rgba_layout->setVerticalSpacing(4);
        rgba_layout->setContentsMargins(0, 0, 0, 0);
        label_red = new QLabel(formLayoutWidget);
        label_red->setObjectName(QString::fromUtf8("label_red"));

        rgba_layout->setWidget(0, QFormLayout::LabelRole, label_red);

        label_blue = new QLabel(formLayoutWidget);
        label_blue->setObjectName(QString::fromUtf8("label_blue"));

        rgba_layout->setWidget(2, QFormLayout::LabelRole, label_blue);

        label_green = new QLabel(formLayoutWidget);
        label_green->setObjectName(QString::fromUtf8("label_green"));

        rgba_layout->setWidget(1, QFormLayout::LabelRole, label_green);

        spin_red = new QSpinBox(formLayoutWidget);
        spin_red->setObjectName(QString::fromUtf8("spin_red"));
        spin_red->setMinimumSize(QSize(0, 21));
        spin_red->setFrame(false);
        spin_red->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_red->setMaximum(255);

        rgba_layout->setWidget(0, QFormLayout::FieldRole, spin_red);

        spin_green = new QSpinBox(formLayoutWidget);
        spin_green->setObjectName(QString::fromUtf8("spin_green"));
        spin_green->setMinimumSize(QSize(0, 21));
        spin_green->setFrame(false);
        spin_green->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_green->setMaximum(255);

        rgba_layout->setWidget(1, QFormLayout::FieldRole, spin_green);

        spin_blue = new QSpinBox(formLayoutWidget);
        spin_blue->setObjectName(QString::fromUtf8("spin_blue"));
        spin_blue->setMinimumSize(QSize(0, 21));
        spin_blue->setFrame(false);
        spin_blue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_blue->setMaximum(255);

        rgba_layout->setWidget(2, QFormLayout::FieldRole, spin_blue);

        spin_alpha = new QSpinBox(formLayoutWidget);
        spin_alpha->setObjectName(QString::fromUtf8("spin_alpha"));
        spin_alpha->setMinimumSize(QSize(0, 21));
        spin_alpha->setFrame(false);
        spin_alpha->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_alpha->setMaximum(255);
        spin_alpha->setValue(255);

        rgba_layout->setWidget(6, QFormLayout::FieldRole, spin_alpha);

        spin_hex = new QSpinBox(formLayoutWidget);
        spin_hex->setObjectName(QString::fromUtf8("spin_hex"));
        spin_hex->setMinimumSize(QSize(0, 21));
        spin_hex->setFrame(false);
        spin_hex->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_hex->setMaximum(16777215);
        spin_hex->setValue(0);
        spin_hex->setDisplayIntegerBase(16);

        rgba_layout->setWidget(7, QFormLayout::FieldRole, spin_hex);

        label_alpha = new QLabel(formLayoutWidget);
        label_alpha->setObjectName(QString::fromUtf8("label_alpha"));

        rgba_layout->setWidget(6, QFormLayout::LabelRole, label_alpha);

        label_hex = new QLabel(formLayoutWidget);
        label_hex->setObjectName(QString::fromUtf8("label_hex"));

        rgba_layout->setWidget(7, QFormLayout::LabelRole, label_hex);

        label_hue = new QLabel(formLayoutWidget);
        label_hue->setObjectName(QString::fromUtf8("label_hue"));

        rgba_layout->setWidget(3, QFormLayout::LabelRole, label_hue);

        label_sat = new QLabel(formLayoutWidget);
        label_sat->setObjectName(QString::fromUtf8("label_sat"));

        rgba_layout->setWidget(4, QFormLayout::LabelRole, label_sat);

        label_val = new QLabel(formLayoutWidget);
        label_val->setObjectName(QString::fromUtf8("label_val"));

        rgba_layout->setWidget(5, QFormLayout::LabelRole, label_val);

        spin_hue = new QSpinBox(formLayoutWidget);
        spin_hue->setObjectName(QString::fromUtf8("spin_hue"));
        spin_hue->setMinimumSize(QSize(0, 21));
        spin_hue->setFrame(false);
        spin_hue->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_hue->setMaximum(255);

        rgba_layout->setWidget(3, QFormLayout::FieldRole, spin_hue);

        spin_sat = new QSpinBox(formLayoutWidget);
        spin_sat->setObjectName(QString::fromUtf8("spin_sat"));
        spin_sat->setMinimumSize(QSize(0, 21));
        spin_sat->setFrame(false);
        spin_sat->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_sat->setMaximum(255);

        rgba_layout->setWidget(4, QFormLayout::FieldRole, spin_sat);

        spin_val = new QSpinBox(formLayoutWidget);
        spin_val->setObjectName(QString::fromUtf8("spin_val"));
        spin_val->setMinimumSize(QSize(0, 21));
        spin_val->setFrame(false);
        spin_val->setButtonSymbols(QAbstractSpinBox::NoButtons);
        spin_val->setMaximum(255);

        rgba_layout->setWidget(5, QFormLayout::FieldRole, spin_val);


        retranslateUi(ColorPicker);

        QMetaObject::connectSlotsByName(ColorPicker);
    } // setupUi

    void retranslateUi(QWidget *ColorPicker)
    {
        ColorPicker->setWindowTitle(QApplication::translate("ColorPicker", "ColorPicker", nullptr));
        btn_revert->setText(QApplication::translate("ColorPicker", "Revert", nullptr));
        btn_ok->setText(QApplication::translate("ColorPicker", "OK", nullptr));
        btn_cancel->setText(QApplication::translate("ColorPicker", "Cancel", nullptr));
        label_red->setText(QApplication::translate("ColorPicker", "Red", nullptr));
        label_blue->setText(QApplication::translate("ColorPicker", "Blue", nullptr));
        label_green->setText(QApplication::translate("ColorPicker", "Green", nullptr));
        spin_hex->setPrefix(QApplication::translate("ColorPicker", "#", nullptr));
        label_alpha->setText(QApplication::translate("ColorPicker", "Alpha", nullptr));
        label_hex->setText(QApplication::translate("ColorPicker", "Hex", nullptr));
        label_hue->setText(QApplication::translate("ColorPicker", "Hue", nullptr));
        label_sat->setText(QApplication::translate("ColorPicker", "Sat", nullptr));
        label_val->setText(QApplication::translate("ColorPicker", "Val", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ColorPicker: public Ui_ColorPicker {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_COLORPICKER_H
