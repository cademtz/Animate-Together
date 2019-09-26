/********************************************************************************
** Form generated from reading UI file 'CExport.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CEXPORT_H
#define UI_CEXPORT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CExport
{
public:

    void setupUi(QWidget *CExport)
    {
        if (CExport->objectName().isEmpty())
            CExport->setObjectName(QString::fromUtf8("CExport"));
        CExport->resize(400, 300);

        retranslateUi(CExport);

        QMetaObject::connectSlotsByName(CExport);
    } // setupUi

    void retranslateUi(QWidget *CExport)
    {
        CExport->setWindowTitle(QApplication::translate("CExport", "CExport", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CExport: public Ui_CExport {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CEXPORT_H
