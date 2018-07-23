#pragma once
#include "qdialog.h"
#include <QWidget>
#include "QDialog.h"
#include "ui_MeshSaveDlg.h"
class MeshSaveDlg :public QDialog
{
	Q_OBJECT
public:
	MeshSaveDlg(QWidget* parent = 0);
	virtual ~MeshSaveDlg();
	Ui::MeshSaveDlg *unsavemesh;
public: 
	int saveways;
	///////////²Ûº¯Êý////////
public slots :
   void binarysimp();
   void Txtsimp();

};

