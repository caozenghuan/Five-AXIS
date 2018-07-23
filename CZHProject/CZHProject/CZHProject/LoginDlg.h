#ifndef LOGINDLG_H
#define LOGINDLG_H

#include "ui_LoginDlg.h"
#include "qtimer.h"


class LoginDlg :public QDialog,Ui::LoginDlg
{
	Q_OBJECT
public:
	LoginDlg(QWidget *parent = 0);
	~LoginDlg();
	void start();
	void paintEvent(QPaintEvent *  event ) ;
private:
	Ui::LoginDlg    *ui;
	QTimer *timer; 
	int backLevel;
};

#endif 