#pragma once

#include "ui_DlgOptiSequence.h"
#include <vector>
#include "Optisequence.h"
using namespace std;

class CDlgOptisequence :public QDialog, public Ui::DlgSeQClass
{
	Q_OBJECT
public:
	CDlgOptisequence(QWidget *parent = 0);
	virtual ~CDlgOptisequence();
private:
	Ui::DlgSeQClass *ui;

public:
	vector<QString>  m_vec_str;
	void getStr();
	int index;
private slots:
	void  whoIsSelect();

public slots:

};

