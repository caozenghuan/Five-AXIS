#pragma once
#include "ui_DligInterveResult.h"

using namespace std;
class DlgInterveResult :public QDialog, public Ui::InterveResultDialog
{
	Q_OBJECT
public:
	DlgInterveResult(QWidget *parent = 0);
	virtual ~DlgInterveResult();
	int index;
public:
	Ui::InterveResultDialog *ui;
	vector<QString>  m_vec_str;
	void getStr();
private slots :
	void  whoIsSelect();
};

