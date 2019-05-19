/********************************************************************************
** Form generated from reading UI file 'EditLayer.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EDITLAYER_H
#define UI_EDITLAYER_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QFrame>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditLayer
{
public:
    QWidget *formLayoutWidget;
    QFormLayout *formLayout;
    QLabel *label;
    QLineEdit *tb_layername;
    QCheckBox *cb_private;
    QFrame *frame;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;

    void setupUi(QWidget *EditLayer)
    {
        if (EditLayer->objectName().isEmpty())
            EditLayer->setObjectName(QString::fromUtf8("EditLayer"));
        EditLayer->resize(361, 71);
        EditLayer->setStyleSheet(QString::fromUtf8("QWidget#EditLayer {\n"
"	background-color: rgb(50, 50, 50);\n"
"}\n"
"QLabel, QCheckBox {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 500 8pt \"Arial\";\n"
"}\n"
"QPushButton, QLineEdit {\n"
"	border: 1px solid rgb(75, 75, 75);\n"
"	background-color: rgb(35, 35, 35);\n"
"	color: rgb(200, 200, 200);\n"
"}\n"
"QPushButton:hover, QLineEdit:focus {\n"
"	border-color: rgb(0, 128, 255);\n"
"	color: rgb(255, 255, 255);\n"
"}\n"
"QPushButton:hover:pressed {\n"
"	background-color: rgb(25, 25, 25);\n"
"}"));
        formLayoutWidget = new QWidget(EditLayer);
        formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));
        formLayoutWidget->setGeometry(QRect(10, 10, 241, 52));
        formLayout = new QFormLayout(formLayoutWidget);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QString::fromUtf8("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(formLayoutWidget);
        label->setObjectName(QString::fromUtf8("label"));

        formLayout->setWidget(0, QFormLayout::LabelRole, label);

        tb_layername = new QLineEdit(formLayoutWidget);
        tb_layername->setObjectName(QString::fromUtf8("tb_layername"));
        tb_layername->setMinimumSize(QSize(0, 21));

        formLayout->setWidget(0, QFormLayout::FieldRole, tb_layername);

        cb_private = new QCheckBox(formLayoutWidget);
        cb_private->setObjectName(QString::fromUtf8("cb_private"));
        cb_private->setStyleSheet(QString::fromUtf8("background-color: rgb(50, 50, 50);"));

        formLayout->setWidget(1, QFormLayout::LabelRole, cb_private);

        frame = new QFrame(EditLayer);
        frame->setObjectName(QString::fromUtf8("frame"));
        frame->setGeometry(QRect(260, 0, 101, 71));
        frame->setStyleSheet(QString::fromUtf8(".QFrame {\n"
"	background-color: rgb(40, 40, 40);\n"
"}"));
        layoutWidget = new QWidget(frame);
        layoutWidget->setObjectName(QString::fromUtf8("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 81, 54));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        btn_ok = new QPushButton(layoutWidget);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        btn_ok->setMinimumSize(QSize(75, 23));

        verticalLayout->addWidget(btn_ok);

        btn_cancel = new QPushButton(layoutWidget);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setMinimumSize(QSize(75, 23));

        verticalLayout->addWidget(btn_cancel);


        retranslateUi(EditLayer);

        QMetaObject::connectSlotsByName(EditLayer);
    } // setupUi

    void retranslateUi(QWidget *EditLayer)
    {
        EditLayer->setWindowTitle(QApplication::translate("EditLayer", "Edit layer", nullptr));
        label->setText(QApplication::translate("EditLayer", "Name", nullptr));
        tb_layername->setPlaceholderText(QApplication::translate("EditLayer", "New layer", nullptr));
#ifndef QT_NO_TOOLTIP
        cb_private->setToolTip(QApplication::translate("EditLayer", "Only you can edit this layer", nullptr));
#endif // QT_NO_TOOLTIP
        cb_private->setText(QApplication::translate("EditLayer", "Private", nullptr));
        btn_ok->setText(QApplication::translate("EditLayer", "OK", nullptr));
        btn_cancel->setText(QApplication::translate("EditLayer", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditLayer: public Ui_EditLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITLAYER_H
