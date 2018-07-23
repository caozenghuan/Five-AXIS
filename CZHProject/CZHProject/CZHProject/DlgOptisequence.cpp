#include "DlgOptisequence.h"


CDlgOptisequence::CDlgOptisequence(QWidget *parent):
ui(new Ui::DlgSeQClass)
{
	ui->setupUi(this);
	index = 0;
}


CDlgOptisequence::~CDlgOptisequence()
{
}
void CDlgOptisequence::getStr()
{
	int sz = m_vec_str.size();
	for (int i = 0; i < sz; i++)
	{
		ui->listWidget->addItem(m_vec_str[i]);
	}
	ui->listWidget->item(0)->setSelected(true);
}
void CDlgOptisequence::whoIsSelect()
{
	index = ui->listWidget->currentRow();
}