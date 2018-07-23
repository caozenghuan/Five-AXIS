#ifndef DLGOPTI_H
#define DLGOPTI_H

#include "ui_Opti.h"
#include <vector>
#include "Optimization.h"
using namespace std;

class DlgOpti :public QDialog, public Ui::DlgOptiClass
{
	Q_OBJECT
public:
	DlgOpti(QWidget *parent = 0);
	~DlgOpti();
private:
	Ui::DlgOptiClass *ui;
public:
	vector<QString>  m_vec_str;
	void getStr();
	int index;
private slots: 
	void  whoIsSelect();

public slots:
	void PreBlock();
	void NextBlock();
	void GetResult();
	void onOptiDir();
	void onShowOpti();//显示分层方向
public:
	VECTOR3D    m_VecDir;
	bool       m_bOptiDir;
	bool       Yresult;        
	int NBlock;
};

#endif 