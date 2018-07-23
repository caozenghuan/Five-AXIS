#include "BlackgroundDlg.h"


CBlackgroundDlg::CBlackgroundDlg(QWidget* parent) : QDialog(parent)
{
	Adjblack = new Ui::BlackGround;
	Adjblack->setupUi(this);
	BlackMode = 0;
	Adjblack->radioBlueLight->setChecked(true);
}


CBlackgroundDlg::~CBlackgroundDlg()
{
}
void CBlackgroundDlg::Bluelight()
{
	BlackMode = 0;
}
void CBlackgroundDlg::Grepshade()
{
	BlackMode = 1;
}
void CBlackgroundDlg::Bluedark()
{
	BlackMode = 2;
}
void CBlackgroundDlg::WhiteBlack()
{
	BlackMode = 3;
}
