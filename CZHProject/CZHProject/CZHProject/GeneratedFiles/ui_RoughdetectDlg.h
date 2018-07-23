/********************************************************************************
** Form generated from reading UI file 'RoughdetectDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ROUGHDETECTDLG_H
#define UI_ROUGHDETECTDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Roughdetect
{
public:
    QGroupBox *groupBox;
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QLineEdit *Threvalue;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *Roughdetect)
    {
        if (Roughdetect->objectName().isEmpty())
            Roughdetect->setObjectName(QStringLiteral("Roughdetect"));
        Roughdetect->resize(187, 141);
        groupBox = new QGroupBox(Roughdetect);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 161, 121));
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(0, 60, 158, 51));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        Threvalue = new QLineEdit(layoutWidget);
        Threvalue->setObjectName(QStringLiteral("Threvalue"));
        Threvalue->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(Threvalue);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(Roughdetect);
        QObject::connect(buttonBox, SIGNAL(accepted()), Roughdetect, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), Roughdetect, SLOT(reject()));
        QObject::connect(Threvalue, SIGNAL(editingFinished()), Roughdetect, SLOT(Linput()));

        QMetaObject::connectSlotsByName(Roughdetect);
    } // setupUi

    void retranslateUi(QDialog *Roughdetect)
    {
        Roughdetect->setWindowTitle(QApplication::translate("Roughdetect", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("Roughdetect", "\346\243\200\346\265\213\345\207\271\347\202\271\347\232\204\351\230\200\345\200\274", 0));
        Threvalue->setText(QApplication::translate("Roughdetect", "2.6", 0));
    } // retranslateUi

};

namespace Ui {
    class Roughdetect: public Ui_Roughdetect {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ROUGHDETECTDLG_H
