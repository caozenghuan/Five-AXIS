
#include "ZoomDlg.h"

ZoomDlg::ZoomDlg(QWidget* parent) : QDialog(parent)
{
	unated1 = new Ui::ZoomDlg;
	unated1->setupUi(this);
	rate_x = rate_y = rate_z = 1000;
}
ZoomDlg::~ZoomDlg()
{
}
//void Dialog::Surezoom()
//{
//	QString  strx = unated1->m_rateEdit_X->text();
//	rate_x = strx.toFloat();
//	QString  stry = unated1->m_rateEdit_Y->text();
//	rate_y = stry.toFloat();
//	QString  strz = unated1->m_rateEdit_Z->text();
//	rate_z = strz.toFloat();
//}