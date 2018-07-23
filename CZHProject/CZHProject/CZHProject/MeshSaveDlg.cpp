#include "MeshSaveDlg.h"


MeshSaveDlg::MeshSaveDlg(QWidget* parent) : QDialog(parent)
{
	unsavemesh = new Ui::MeshSaveDlg;
	unsavemesh->setupUi(this);
	saveways = 0;
}


MeshSaveDlg::~MeshSaveDlg()
{
}
void MeshSaveDlg::binarysimp()
{
	saveways = 1;
}
void MeshSaveDlg::Txtsimp()
{
	saveways = 0;
}
