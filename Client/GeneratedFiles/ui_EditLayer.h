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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_EditLayer
{
public:
    QLabel *label;
    QLineEdit *tb_layername;
    QPushButton *btn_ok;
    QPushButton *btn_cancel;
    QWidget *horizontalLayoutWidget;
    QHBoxLayout *horizontalLayout;
    QCheckBox *cb_hidden;
    QCheckBox *cb_private;

    void setupUi(QWidget *EditLayer)
    {
        if (EditLayer->objectName().isEmpty())
            EditLayer->setObjectName(QString::fromUtf8("EditLayer"));
        EditLayer->resize(211, 141);
        EditLayer->setStyleSheet(QString::fromUtf8("QWidget#EditLayer {\n"
"	background-color: rgb(40, 40, 40);\n"
"}\n"
"QLabel {\n"
"	color: rgb(225, 225, 225);\n"
"	font: 16px \"Verdana\";\n"
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
"}"));
        label = new QLabel(EditLayer);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 10, 191, 21));
        label->setAlignment(Qt::AlignCenter);
        tb_layername = new QLineEdit(EditLayer);
        tb_layername->setObjectName(QString::fromUtf8("tb_layername"));
        tb_layername->setGeometry(QRect(10, 34, 191, 31));
        tb_layername->setMinimumSize(QSize(0, 21));
        btn_ok = new QPushButton(EditLayer);
        btn_ok->setObjectName(QString::fromUtf8("btn_ok"));
        btn_ok->setGeometry(QRect(10, 110, 91, 23));
        btn_ok->setMinimumSize(QSize(75, 23));
        btn_ok->setStyleSheet(QString::fromUtf8("background-color: rgb(0, 128, 255);\n"
"color: rgb(225, 225, 225);"));
        btn_cancel = new QPushButton(EditLayer);
        btn_cancel->setObjectName(QString::fromUtf8("btn_cancel"));
        btn_cancel->setGeometry(QRect(110, 110, 91, 23));
        btn_cancel->setMinimumSize(QSize(75, 23));
        horizontalLayoutWidget = new QWidget(EditLayer);
        horizontalLayoutWidget->setObjectName(QString::fromUtf8("horizontalLayoutWidget"));
        horizontalLayoutWidget->setGeometry(QRect(10, 70, 191, 31));
        horizontalLayout = new QHBoxLayout(horizontalLayoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        cb_hidden = new QCheckBox(horizontalLayoutWidget);
        cb_hidden->setObjectName(QString::fromUtf8("cb_hidden"));
        cb_hidden->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(cb_hidden);

        cb_private = new QCheckBox(horizontalLayoutWidget);
        cb_private->setObjectName(QString::fromUtf8("cb_private"));
        cb_private->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout->addWidget(cb_private);


        retranslateUi(EditLayer);

        QMetaObject::connectSlotsByName(EditLayer);
    } // setupUi

    void retranslateUi(QWidget *EditLayer)
    {
        EditLayer->setWindowTitle(QApplication::translate("EditLayer", "Edit layer", nullptr));
        label->setText(QApplication::translate("EditLayer", "Name", nullptr));
        tb_layername->setPlaceholderText(QApplication::translate("EditLayer", "New layer", nullptr));
        btn_ok->setText(QApplication::translate("EditLayer", "OK", nullptr));
        btn_cancel->setText(QApplication::translate("EditLayer", "Cancel", nullptr));
#ifndef QT_NO_TOOLTIP
        cb_hidden->setToolTip(QApplication::translate("EditLayer", "Only you can edit this layer", nullptr));
#endif // QT_NO_TOOLTIP
        cb_hidden->setText(QApplication::translate("EditLayer", "Hidden", nullptr));
#ifndef QT_NO_TOOLTIP
        cb_private->setToolTip(QApplication::translate("EditLayer", "Only you can edit this layer", nullptr));
#endif // QT_NO_TOOLTIP
        cb_private->setText(QApplication::translate("EditLayer", "Private", nullptr));
    } // retranslateUi

};

namespace Ui {
    class EditLayer: public Ui_EditLayer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EDITLAYER_H
