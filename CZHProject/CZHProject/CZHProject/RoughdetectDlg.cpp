#include "RoughdetectDlg.h"


RoughdetectDlg::RoughdetectDlg(QWidget* parent) : QDialog(parent)
{
	unated1 = new Ui::Roughdetect;
	unated1->setupUi(this);
	Limitvalue = 2.6;
}


RoughdetectDlg::~RoughdetectDlg()
{
}

void RoughdetectDlg::Linput()
{
	QString text = unated1->Threvalue->text();
	Limitvalue = text.toDouble();
	if (Limitvalue < 0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您输入的数据有误,重新输入!"));
		return;
	}
}