/********************************************************************************
** Form generated from reading UI file 'DlgBlockshow.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DLGBLOCKSHOW_H
#define UI_DLGBLOCKSHOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BlockSeqshow
{
public:
    QGridLayout *gridLayout;
    QGroupBox *groupBox;
    QDialogButtonBox *buttonBox;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QPushButton *Blocklast;
    QPushButton *BolckNext;

    void setupUi(QDialog *BlockSeqshow)
    {
        if (BlockSeqshow->objectName().isEmpty())
            BlockSeqshow->setObjectName(QStringLiteral("BlockSeqshow"));
        BlockSeqshow->resize(224, 131);
        gridLayout = new QGridLayout(BlockSeqshow);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        groupBox = new QGroupBox(BlockSeqshow);
        groupBox->setObjectName(QStringLiteral("groupBox"));
        buttonBox = new QDialogButtonBox(groupBox);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(5, 70, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        layoutWidget = new QWidget(groupBox);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 25, 158, 25));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        Blocklast = new QPushButton(layoutWidget);
        Blocklast->setObjectName(QStringLiteral("Blocklast"));

        horizontalLayout->addWidget(Blocklast);

        BolckNext = new QPushButton(layoutWidget);
        BolckNext->setObjectName(QStringLiteral("BolckNext"));

        horizontalLayout->addWidget(BolckNext);


        gridLayout->addWidget(groupBox, 0, 0, 1, 1);


        retranslateUi(BlockSeqshow);
        QObject::connect(buttonBox, SIGNAL(accepted()), BlockSeqshow, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BlockSeqshow, SLOT(reject()));
        QObject::connect(Blocklast, SIGNAL(clicked()), BlockSeqshow, SLOT(LastBlock()));
        QObject::connect(BolckNext, SIGNAL(clicked()), BlockSeqshow, SLOT(NextBlock()));

        QMetaObject::connectSlotsByName(BlockSeqshow);
    } // setupUi

    void retranslateUi(QDialog *BlockSeqshow)
    {
        BlockSeqshow->setWindowTitle(QApplication::translate("BlockSeqshow", "Dialog", 0));
        groupBox->setTitle(QApplication::translate("BlockSeqshow", "\350\247\206\345\233\276\346\230\276\347\244\272", 0));
        Blocklast->setText(QApplication::translate("BlockSeqshow", "\344\270\212\344\270\200\345\235\227", 0));
        BolckNext->setText(QApplication::translate("BlockSeqshow", "\344\270\213\344\270\200\345\235\227", 0));
    } // retranslateUi

};

namespace Ui {
    class BlockSeqshow: public Ui_BlockSeqshow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DLGBLOCKSHOW_H
