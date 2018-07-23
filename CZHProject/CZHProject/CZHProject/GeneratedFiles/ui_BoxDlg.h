/********************************************************************************
** Form generated from reading UI file 'BoxDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BOXDLG_H
#define UI_BOXDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_BoxDlg
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *labelbox;
    QHBoxLayout *horizontalLayout;
    QLabel *label_x;
    QLabel *label_x1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_y;
    QLabel *label_y1;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label_z;
    QLabel *label_z1;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *BoxDlg)
    {
        if (BoxDlg->objectName().isEmpty())
            BoxDlg->setObjectName(QStringLiteral("BoxDlg"));
        BoxDlg->resize(230, 165);
        layoutWidget = new QWidget(BoxDlg);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(50, 20, 158, 113));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        labelbox = new QLabel(layoutWidget);
        labelbox->setObjectName(QStringLiteral("labelbox"));
        labelbox->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        verticalLayout->addWidget(labelbox);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label_x = new QLabel(layoutWidget);
        label_x->setObjectName(QStringLiteral("label_x"));
        label_x->setAlignment(Qt::AlignCenter);

        horizontalLayout->addWidget(label_x);

        label_x1 = new QLabel(layoutWidget);
        label_x1->setObjectName(QStringLiteral("label_x1"));

        horizontalLayout->addWidget(label_x1);

        horizontalLayout->setStretch(0, 3);
        horizontalLayout->setStretch(1, 5);

        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_y = new QLabel(layoutWidget);
        label_y->setObjectName(QStringLiteral("label_y"));
        label_y->setAlignment(Qt::AlignCenter);

        horizontalLayout_2->addWidget(label_y);

        label_y1 = new QLabel(layoutWidget);
        label_y1->setObjectName(QStringLiteral("label_y1"));

        horizontalLayout_2->addWidget(label_y1);

        horizontalLayout_2->setStretch(0, 3);
        horizontalLayout_2->setStretch(1, 5);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        label_z = new QLabel(layoutWidget);
        label_z->setObjectName(QStringLiteral("label_z"));
        label_z->setAlignment(Qt::AlignCenter);

        horizontalLayout_3->addWidget(label_z);

        label_z1 = new QLabel(layoutWidget);
        label_z1->setObjectName(QStringLiteral("label_z1"));

        horizontalLayout_3->addWidget(label_z1);

        horizontalLayout_3->setStretch(0, 3);
        horizontalLayout_3->setStretch(1, 5);

        verticalLayout->addLayout(horizontalLayout_3);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(BoxDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), BoxDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), BoxDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(BoxDlg);
    } // setupUi

    void retranslateUi(QDialog *BoxDlg)
    {
        BoxDlg->setWindowTitle(QApplication::translate("BoxDlg", "Dialog", 0));
        labelbox->setText(QString());
        label_x->setText(QApplication::translate("BoxDlg", "X:", 0));
        label_x1->setText(QString());
        label_y->setText(QApplication::translate("BoxDlg", "Y:", 0));
        label_y1->setText(QString());
        label_z->setText(QApplication::translate("BoxDlg", "Z:", 0));
        label_z1->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class BoxDlg: public Ui_BoxDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BOXDLG_H
