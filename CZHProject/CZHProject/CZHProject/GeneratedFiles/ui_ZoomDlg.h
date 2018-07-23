/********************************************************************************
** Form generated from reading UI file 'ZoomDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ZOOMDLG_H
#define UI_ZOOMDLG_H

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
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ZoomDlg
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_3;
    QLabel *label_zoom;
    QHBoxLayout *horizontalLayout_4;
    QSpacerItem *horizontalSpacer_7;
    QLabel *label_x;
    QLineEdit *m_rateEdit_X;
    QSpacerItem *horizontalSpacer_4;
    QHBoxLayout *horizontalLayout_5;
    QSpacerItem *horizontalSpacer_8;
    QLabel *label_Y;
    QLineEdit *m_rateEdit_Y;
    QSpacerItem *horizontalSpacer_5;
    QHBoxLayout *horizontalLayout_9;
    QSpacerItem *horizontalSpacer_9;
    QLabel *label_Z;
    QLineEdit *m_rateEdit_Z;
    QSpacerItem *horizontalSpacer_6;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *ZoomDlg)
    {
        if (ZoomDlg->objectName().isEmpty())
            ZoomDlg->setObjectName(QStringLiteral("ZoomDlg"));
        ZoomDlg->resize(196, 152);
        layoutWidget = new QWidget(ZoomDlg);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 10, 161, 127));
        verticalLayout_3 = new QVBoxLayout(layoutWidget);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        label_zoom = new QLabel(layoutWidget);
        label_zoom->setObjectName(QStringLiteral("label_zoom"));
        label_zoom->setAlignment(Qt::AlignCenter);

        verticalLayout_3->addWidget(label_zoom);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalSpacer_7 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_7);

        label_x = new QLabel(layoutWidget);
        label_x->setObjectName(QStringLiteral("label_x"));
        label_x->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(label_x);

        m_rateEdit_X = new QLineEdit(layoutWidget);
        m_rateEdit_X->setObjectName(QStringLiteral("m_rateEdit_X"));
        m_rateEdit_X->setStyleSheet(QLatin1String("border: 2px solid grey;\n"
"border-radius: 5px;\n"
" font-weight: bold;  "));
        m_rateEdit_X->setAlignment(Qt::AlignCenter);

        horizontalLayout_4->addWidget(m_rateEdit_X);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_4->addItem(horizontalSpacer_4);


        verticalLayout_3->addLayout(horizontalLayout_4);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalSpacer_8 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_8);

        label_Y = new QLabel(layoutWidget);
        label_Y->setObjectName(QStringLiteral("label_Y"));
        label_Y->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(label_Y);

        m_rateEdit_Y = new QLineEdit(layoutWidget);
        m_rateEdit_Y->setObjectName(QStringLiteral("m_rateEdit_Y"));
        m_rateEdit_Y->setStyleSheet(QLatin1String("border: 2px solid grey;\n"
"border-radius: 5px;\n"
" font-weight: bold;  "));
        m_rateEdit_Y->setAlignment(Qt::AlignCenter);

        horizontalLayout_5->addWidget(m_rateEdit_Y);

        horizontalSpacer_5 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer_5);


        verticalLayout_3->addLayout(horizontalLayout_5);

        horizontalLayout_9 = new QHBoxLayout();
        horizontalLayout_9->setObjectName(QStringLiteral("horizontalLayout_9"));
        horizontalSpacer_9 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_9);

        label_Z = new QLabel(layoutWidget);
        label_Z->setObjectName(QStringLiteral("label_Z"));
        label_Z->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(label_Z);

        m_rateEdit_Z = new QLineEdit(layoutWidget);
        m_rateEdit_Z->setObjectName(QStringLiteral("m_rateEdit_Z"));
        m_rateEdit_Z->setStyleSheet(QLatin1String("border: 2px solid grey;\n"
"border-radius: 5px;\n"
" font-weight: bold;  "));
        m_rateEdit_Z->setAlignment(Qt::AlignCenter);

        horizontalLayout_9->addWidget(m_rateEdit_Z);

        horizontalSpacer_6 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_9->addItem(horizontalSpacer_6);


        verticalLayout_3->addLayout(horizontalLayout_9);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setLayoutDirection(Qt::LeftToRight);
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_3->addWidget(buttonBox);


        retranslateUi(ZoomDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), ZoomDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), ZoomDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(ZoomDlg);
    } // setupUi

    void retranslateUi(QDialog *ZoomDlg)
    {
        ZoomDlg->setWindowTitle(QApplication::translate("ZoomDlg", "Dialog", 0));
        label_zoom->setText(QApplication::translate("ZoomDlg", "\347\274\251\346\224\276\345\217\202\346\225\260\350\256\276\347\275\256", 0));
        label_x->setText(QApplication::translate("ZoomDlg", " X:", 0));
        m_rateEdit_X->setText(QApplication::translate("ZoomDlg", "1000", 0));
        label_Y->setText(QApplication::translate("ZoomDlg", " Y:", 0));
        m_rateEdit_Y->setText(QApplication::translate("ZoomDlg", "1000", 0));
        label_Z->setText(QApplication::translate("ZoomDlg", " Z:", 0));
        m_rateEdit_Z->setText(QApplication::translate("ZoomDlg", "1000", 0));
    } // retranslateUi

};

namespace Ui {
    class ZoomDlg: public Ui_ZoomDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ZOOMDLG_H
