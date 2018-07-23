#include "DlgInterveResult.h"


DlgInterveResult::DlgInterveResult(QWidget *parent) :
ui(new Ui::InterveResultDialog)
{
	ui->setupUi(this);
	index = 0;
}


DlgInterveResult::~DlgInterveResult()
{
}
void DlgInterveResult::getStr()
{
	int sz = m_vec_str.size();
	for (int i = 0; i < sz; i++)
	{
		ui->listWidget->addItem(m_vec_str[i]);
	}
	ui->listWidget->item(0)->setSelected(true);
}
void DlgInterveResult::whoIsSelect()
{
	index = ui->listWidget->currentRow();
}