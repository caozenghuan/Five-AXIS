/********************************************************************************
** Form generated from reading UI file 'TanshexianDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TANSHEXIANDLG_H
#define UI_TANSHEXIANDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_TancexianDlg
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QLineEdit *EditAngel;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *TancexianDlg)
    {
        if (TancexianDlg->objectName().isEmpty())
            TancexianDlg->setObjectName(QStringLiteral("TancexianDlg"));
        TancexianDlg->resize(198, 99);
        layoutWidget = new QWidget(TancexianDlg);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 30, 161, 53));
        verticalLayout = new QVBoxLayout(layoutWidget);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        EditAngel = new QLineEdit(layoutWidget);
        EditAngel->setObjectName(QStringLiteral("EditAngel"));
        EditAngel->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(EditAngel);


        verticalLayout->addLayout(horizontalLayout);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout->addWidget(buttonBox);


        retranslateUi(TancexianDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), TancexianDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), TancexianDlg, SLOT(reject()));
        QObject::connect(EditAngel, SIGNAL(textChanged(QString)), TancexianDlg, SLOT(Linput()));

        QMetaObject::connectSlotsByName(TancexianDlg);
    } // setupUi

    void retranslateUi(QDialog *TancexianDlg)
    {
        TancexianDlg->setWindowTitle(QApplication::translate("TancexianDlg", "Dialog", 0));
        label->setText(QApplication::translate("TancexianDlg", "\346\216\242\345\260\204\347\272\277\350\247\222\345\272\246\357\274\232", 0));
        EditAngel->setText(QApplication::translate("TancexianDlg", "60", 0));
    } // retranslateUi

};

namespace Ui {
    class TancexianDlg: public Ui_TancexianDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TANSHEXIANDLG_H
