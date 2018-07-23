#include "BoxDLg.h"
BoxDLg::BoxDLg(QWidget* parent) : QDialog(parent)
{
	unbox = new Ui::BoxDlg;
	unbox->setupUi(this);
}


BoxDLg::~BoxDLg()
{
}
void BoxDLg::showboxsize(double x, double y, double z)
{
	unbox->labelbox->setText(QString::fromLocal8Bit("ģ�Ͱ�Χ�гߴ磺"));
	QString box_x = QString::number(x);
	unbox->label_x1->setText(box_x);
	QString box_y = QString::number(y);
	unbox->label_y1->setText(box_y);
	QString box_z = QString::number(z);
	unbox->label_z1->setText(box_z);
}