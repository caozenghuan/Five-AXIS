/********************************************************************************
** Form generated from reading UI file 'MeshSaveDlg.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MESHSAVEDLG_H
#define UI_MESHSAVEDLG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MeshSaveDlg
{
public:
    QWidget *layoutWidget;
    QVBoxLayout *verticalLayout_2;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QRadioButton *RadioBinary;
    QRadioButton *RadioTxt;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *MeshSaveDlg)
    {
        if (MeshSaveDlg->objectName().isEmpty())
            MeshSaveDlg->setObjectName(QStringLiteral("MeshSaveDlg"));
        MeshSaveDlg->resize(202, 148);
        layoutWidget = new QWidget(MeshSaveDlg);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(20, 20, 158, 115));
        verticalLayout_2 = new QVBoxLayout(layoutWidget);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(layoutWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(label);

        RadioBinary = new QRadioButton(layoutWidget);
        RadioBinary->setObjectName(QStringLiteral("RadioBinary"));

        verticalLayout->addWidget(RadioBinary);

        RadioTxt = new QRadioButton(layoutWidget);
        RadioTxt->setObjectName(QStringLiteral("RadioTxt"));

        verticalLayout->addWidget(RadioTxt);


        verticalLayout_2->addLayout(verticalLayout);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        verticalLayout_2->addWidget(buttonBox);


        retranslateUi(MeshSaveDlg);
        QObject::connect(buttonBox, SIGNAL(accepted()), MeshSaveDlg, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), MeshSaveDlg, SLOT(reject()));

        QMetaObject::connectSlotsByName(MeshSaveDlg);
    } // setupUi

    void retranslateUi(QDialog *MeshSaveDlg)
    {
        MeshSaveDlg->setWindowTitle(QApplication::translate("MeshSaveDlg", "Dialog", 0));
        label->setText(QApplication::translate("MeshSaveDlg", "\346\240\274\345\274\217\350\256\276\347\275\256:", 0));
        RadioBinary->setText(QApplication::translate("MeshSaveDlg", "\344\272\214\350\277\233\345\210\266", 0));
        RadioTxt->setText(QApplication::translate("MeshSaveDlg", "\346\226\207\346\234\254\346\240\274\345\274\217", 0));
    } // retranslateUi

};

namespace Ui {
    class MeshSaveDlg: public Ui_MeshSaveDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MESHSAVEDLG_H
