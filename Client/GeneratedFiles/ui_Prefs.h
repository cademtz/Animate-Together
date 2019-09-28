/********************************************************************************
** Form generated from reading UI file 'Prefs.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_PREFS_H
#define UI_PREFS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Prefs
{
public:

    void setupUi(QWidget *Prefs)
    {
        if (Prefs->objectName().isEmpty())
            Prefs->setObjectName(QString::fromUtf8("Prefs"));
        Prefs->resize(400, 300);

        retranslateUi(Prefs);

        QMetaObject::connectSlotsByName(Prefs);
    } // setupUi

    void retranslateUi(QWidget *Prefs)
    {
        Prefs->setWindowTitle(QApplication::translate("Prefs", "Preferences", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Prefs: public Ui_Prefs {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_PREFS_H
