#pragma once
#include "qdialog.h"
#include <QWidget>
#include "QDialog.h"
#include "ui_BlackGround.h"
class CBlackgroundDlg :public QDialog
{
	Q_OBJECT
public:
	CBlackgroundDlg(QWidget* parent = 0);
	virtual ~CBlackgroundDlg();
	Ui::BlackGround *Adjblack;

public:
	int BlackMode;
	///////////²Ûº¯Êý////////
	public slots :
	void Bluelight();
	void Grepshade();
	void Bluedark();
	void WhiteBlack();
	
};

