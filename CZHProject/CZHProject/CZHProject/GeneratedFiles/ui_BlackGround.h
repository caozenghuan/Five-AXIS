/********************************************************************************
** Form generated from reading UI file 'BlackGround.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BLACKGROUND_H
#define UI_BLACKGROUND_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QRadioButton>

QT_BEGIN_NAMESPACE

class Ui_BlackGround
{
public:
    QGroupBox *groupBox;
    QRadioButton *radioGray;
    QRadioButton *radioBlueDeep;
    QRadioButton *radioBlueLight;
    QDialogButtonBox *buttonBox;
    QRadioButton *radioWhite;

    void setupUi(QDialog *BlackGround)
    {
        if (BlackGround->objectName().isEmpty())
            BlackGround->setObjectName(QStringLiteral("BlackGround"));
        BlackGround->resize(179, 212);
        groupBox = new QGroupBox(BlackGround);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        groupBox->setGeometry(QRect(25, 15, 131, 176));
        radioGray = new QRadioButton(groupBox);
        radioGray->setObjectName(QStringLiteral("radioGray"));
        radioGray->setGeometry(QRect(25, 55, 89, 16));
        radioBlueDeep = new QRadioButton(groupBox);
        radioBlueDeep->setObjectName(QStringLiteral("radioBlueDeep"));
        radioBlueDeep->setGeometry(QRect(25, 85, 89, 16));
        radioBlueLight = new QRadioButton(groupBox);
        radioBlueLight->setObjectName(QStringLiteral("radioBlueLight"));
        radioBlueLight->setGeometry(QRect(25, 25, 89, 16));
        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(5, 130, 126, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        radioWhite = new QRadioButton(groupBox);
        radioWhite->setObjectName(QStringLiteral("radioWhite"));
        radioWhite->setGeometry(QRect(25, 110, 89, 16));

        retranslateUi(BlackGround);
        QObject::connect(buttonBox, SIGNAL(accepted()), BlackGround, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BlackGround, SLOT(reject()));
        QObject::connect(radioBlueLight, SIGNAL(clicked()), BlackGround, SLOT(Bluelight()));
        QObject::connect(radioBlueDeep, SIGNAL(clicked()), BlackGround, SLOT(Bluedark()));
        QObject::connect(radioGray, SIGNAL(clicked()), BlackGround, SLOT(Grepshade()));
        QObject::connect(radioWhite, SIGNAL(clicked()), BlackGround, SLOT(WhiteBlack()));

        QMetaObject::connectSlotsByName(BlackGround);
    } // setupUi

    void retranslateUi(QDialog *BlackGround)
    {
        BlackGround->setWindowTitle(QApplication::translate("BlackGround", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("BlackGround", "\350\203\214\346\231\257\350\256\276\347\275\256", 0));
        radioGray->setText(QApplication::translate("BlackGround", "\347\201\260\350\211\262\346\270\220\345\217\230", 0));
        radioBlueDeep->setText(QApplication::translate("BlackGround", "\346\267\261\350\223\235\346\270\220\345\217\230", 0));
        radioBlueLight->setText(QApplication::translate("BlackGround", "\346\265\205\350\223\235\346\270\220\345\217\230", 0));
        radioWhite->setText(QApplication::translate("BlackGround", "\347\272\257\347\231\275\350\211\262", 0));
    } // retranslateUi

};

namespace Ui {
    class BlackGround: public Ui_BlackGround {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BLACKGROUND_H
