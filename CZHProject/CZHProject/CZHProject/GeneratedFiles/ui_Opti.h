/********************************************************************************
** Form generated from reading UI file 'Opti.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OPTI_H
#define UI_OPTI_H

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
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DlgOptiClass
{
public:
    QLabel *label;
    QWidget *widget;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer;
    QPushButton *okButton;
    QSpacerItem *horizontalSpacer_14;
    QPushButton *cancelButton;
    QSpacerItem *horizontalSpacer_13;

    void setupUi(QDialog *DlgOptiClass)
    {
        if (DlgOptiClass->objectName().isEmpty())
            DlgOptiClass->setObjectName(QStringLiteral("DlgOptiClass"));
        DlgOptiClass->resize(330, 267);
        label = new QLabel(DlgOptiClass);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 221, 16));
        widget = new QWidget(DlgOptiClass);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(21, 31, 298, 225));
        verticalLayout = new QVBoxLayout(widget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        listWidget = new QListWidget(widget);
        listWidget->setObjectName(QStringLiteral("listWidget"));

        verticalLayout->addWidget(listWidget);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        okButton = new QPushButton(widget);
        okButton->setObjectName(QStringLiteral("okButton"));

        horizontalLayout->addWidget(okButton);

        horizontalSpacer_14 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_14);

        cancelButton = new QPushButton(widget);
        cancelButton->setObjectName(QStringLiteral("cancelButton"));

        horizontalLayout->addWidget(cancelButton);

        horizontalSpacer_13 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_13);


        verticalLayout->addLayout(horizontalLayout);


        retranslateUi(DlgOptiClass);
        QObject::connect(listWidget, SIGNAL(itemClicked(QListWidgetItem*)), DlgOptiClass, SLOT(whoIsSelect()));
        QObject::connect(okButton, SIGNAL(clicked()), DlgOptiClass, SLOT(accept()));
        QObject::connect(cancelButton, SIGNAL(clicked()), DlgOptiClass, SLOT(reject()));

        QMetaObject::connectSlotsByName(DlgOptiClass);
    } // setupUi

    void retranslateUi(QDialog *DlgOptiClass)
    {
        DlgOptiClass->setWindowTitle(QApplication::translate("DlgOptiClass", "\345\220\204\345\255\220\345\235\227\345\210\206\345\261\202\346\226\271\345\220\221\344\274\230\345\214\226", 0));
        label->setText(QApplication::translate("DlgOptiClass", "\350\257\267\345\234\250\344\270\213\351\235\242\347\232\204\347\273\223\346\236\234\344\270\255\351\200\211\346\213\251\344\270\200\344\270\252\357\274\214\347\202\271\345\207\273\347\241\256\345\256\232", 0));
        okButton->setText(QApplication::translate("DlgOptiClass", "\347\241\256\345\256\232", 0));
        cancelButton->setText(QApplication::translate("DlgOptiClass", "\345\217\226\346\266\210", 0));
    } // retranslateUi

};

namespace Ui {
    class DlgOptiClass: public Ui_DlgOptiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OPTI_H
