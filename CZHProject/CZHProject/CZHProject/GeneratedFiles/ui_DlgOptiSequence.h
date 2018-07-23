/********************************************************************************
** Form generated from reading UI file 'DlgOptiSequence.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGOPTISEQUENCE_H
#define UI_DLGOPTISEQUENCE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgSeQClass
{
public:
    QLabel *label;
    QListWidget *listWidget;
    QWidget *layoutWidget;
    QHBoxLayout *hboxLayout;
    QSpacerItem *spacerItem;
    QPushButton *okButton;
    QPushButton *cancelButton;

    void setupUi(QDialog *DlgSeQClass)
    {
        if (DlgSeQClass->objectName().isEmpty())
            DlgSeQClass->setObjectName(QStringLiteral("DlgSeQClass"));
        DlgSeQClass->resize(583, 300);
        label = new QLabel(DlgSeQClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(10, 10, 206, 16));
        listWidget = new QListWidget(DlgSeQClass);
        listWidget->setObjectName(QStringLiteral("listWidget"));
        listWidget->setGeometry(QRect(5, 25, 566, 225));
        layoutWidget = new QWidget(DlgSeQClass);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(5, 255, 566, 33));
        hboxLayout = new QHBoxLayout(layoutWidget);
        hboxLayout->setSpacing(6);
        hboxLayout->setObjectName(QStringLiteral("hboxLayout"));
        hboxLayout->setContentsMargins(0, 0, 0, 0);
        spacerItem = new QSpacerItem(131, 31, QSizePolicy::Expanding, QSizePolicy::Minimum);

        hboxLayout->addItem(spacerItem);

        okButton = new QPushButton(layoutWidget);
        okButton->setObjectName(QStringLiteral("okButton"));

        hboxLayout->addWidget(okButton);

        cancelButton = new QPushButton(layoutWidget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        hboxLayout->addWidget(cancelButton);


        retranslateUi(DlgSeQClass);
        QObject::connect(okButton, SIGNAL(clicked()), DlgSeQClass, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), DlgSeQClass, SLOT(reject()));
        QObject::connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), DlgSeQClass, SLOT(whoIsSelect()));

        QMetaObject::connectSlotsByName(DlgSeQClass);
    } // setupUi

    void retranslateUi(QDialog *DlgSeQClass)
    {
        DlgSeQClass->setWindowTitle(QApplication::translate("DlgSeQClass", "Dialog", 0));
        label->setText(QApplication::translate("DlgSeQClass", "Choose one result and click sure!", 0));
        okButton->setText(QApplication::translate("DlgSeQClass", "Certain", 0));
        cancelButton->setText(QApplication::translate("DlgSeQClass", "Close", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgSeQClass: public Ui_DlgSeQClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGOPTISEQUENCE_H
