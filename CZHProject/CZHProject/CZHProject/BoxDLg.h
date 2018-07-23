#pragma once
#include <QWidget>
#include "QDialog.h"
#include "ui_BoxDlg.h"
#include "qdialog.h"
class BoxDLg :public QDialog
{
	Q_OBJECT
public:
	BoxDLg(QWidget* parent = 0);
	virtual ~BoxDLg();
	Ui::BoxDlg *unbox;

public:
	void showboxsize(double x, double y, double z);
};

