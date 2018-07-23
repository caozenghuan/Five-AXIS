#include "TanshexianDlg.h"


TanshexianDlg::TanshexianDlg(QWidget* parent) : QDialog(parent)
{
	ui_T = new Ui::TancexianDlg;
	ui_T->setupUi(this);
	angel = 60;
}


TanshexianDlg::~TanshexianDlg()
{
}
void TanshexianDlg::Linput()
{
	QString text = ui_T->EditAngel->text();
	angel = text.toDouble();
	if (angel < 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您输入的数据有误,重新输入!"));
		return;
	}
}