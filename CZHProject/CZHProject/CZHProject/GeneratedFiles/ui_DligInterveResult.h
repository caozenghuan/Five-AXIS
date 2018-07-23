/********************************************************************************
** Form generated from reading UI file 'DligInterveResult.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLIGINTERVERESULT_H
#define UI_DLIGINTERVERESULT_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_InterveResultDialog
{
public:
    QLabel *labelResult;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *InterveResultDialog)
    {
        if (InterveResultDialog->objectName().isEmpty())
            InterveResultDialog->setObjectName(QStringLiteral("InterveResultDialog"));
        InterveResultDialog->resize(573, 251);
        labelResult = new QLabel(InterveResultDialog);
        labelResult->setObjectName(QStringLiteral("labelResult"));
        labelResult->setGeometry(QRect(205, 10, 86, 16));
        widget = new QWidget(InterveResultDialog);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 30, 541, 206));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(InterveResultDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), InterveResultDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), InterveResultDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(InterveResultDialog);
    } // setupUi

    void retranslateUi(QDialog *InterveResultDialog)
    {
        InterveResultDialog->setWindowTitle(QApplication::translate("InterveResultDialog", "Dialog", 0));
        labelResult->setText(QApplication::translate("InterveResultDialog", "\345\271\262\346\266\211\346\243\200\346\237\245\347\273\223\346\236\234", 0));
    } // retranslateUi

};

namespace Ui {
    class InterveResultDialog: public Ui_InterveResultDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLIGINTERVERESULT_H
