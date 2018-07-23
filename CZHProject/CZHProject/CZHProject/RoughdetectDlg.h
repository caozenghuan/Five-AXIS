#pragma once
#include <QWidget>
#include "QDialog.h"
#include "qmessagebox.h"
#include "ui_RoughdetectDlg.h"
class RoughdetectDlg :public QDialog
{
	Q_OBJECT
public:
	RoughdetectDlg(QWidget* parent = 0);
	virtual ~RoughdetectDlg();
	Ui::Roughdetect *unated1;

public:
	 double Limitvalue;

///////////²Ûº¯Êý////////
public slots :
            void Linput();

};

