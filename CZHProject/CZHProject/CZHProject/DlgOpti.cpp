#include "DlgOpti.h"

DlgOpti::DlgOpti(QWidget *parent ):
	ui(new Ui::DlgOptiClass)
{
	ui->setupUi(this);
	index=0;
	NBlock = 0;
	m_bOptiDir = false;
	Yresult = false;
	m_VecDir = VECTOR3D(0, 0, 1);
}


DlgOpti::~DlgOpti()
{

}


void DlgOpti::getStr()
{
	int sz=m_vec_str.size();
	for (int i=0;i<sz;i++)
	{
		ui->listWidget->addItem(m_vec_str[i]);
	}
	ui->listWidget->item(0)->setSelected(true);
}


void DlgOpti::whoIsSelect()
{
	int index=ui->listWidget->currentRow();
}
void DlgOpti::onShowOpti()
{
	m_bOptiDir = !m_bOptiDir;
}
void DlgOpti::onOptiDir()
{
	m_bOptiDir = true;
	return;
}
void DlgOpti::PreBlock()
{
	if (NBlock)
	{
		NBlock--;
	}
}
void DlgOpti::NextBlock()
{
	NBlock++;
}
void DlgOpti::GetResult()
{
	Yresult= true;
}
