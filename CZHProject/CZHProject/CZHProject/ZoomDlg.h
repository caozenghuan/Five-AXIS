#pragma  once
#include <QWidget>
#include "QDialog.h"
#include "ui_ZoomDlg.h"
class ZoomDlg :public QDialog
{
	Q_OBJECT
public:
	ZoomDlg(QWidget* parent = 0);
	virtual ~ZoomDlg();
	Ui::ZoomDlg *unated1;
public:
	float rate_x, rate_y, rate_z;

///////////²Ûº¯Êý////////
public slots :
    //void Surezoom();
private:

};

