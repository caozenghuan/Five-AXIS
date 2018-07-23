#ifndef MYQT_H
#define MYQT_H
//#include "LoginDlg.h"
#include "ZoomDlg.h"
#include "RoughdetectDlg.h"
#include "MeshSaveDlg.h"
#include "BoxDLg.h"
#include "TanshexianDlg.h"
#include "BlackgroundDlg.h"
#include <QtWidgets/QMainWindow>
#include "ui_myqt.h"
#include "QGLWidget"
#include "WGLWidge.h"

#include "viewspace/ShowShader.h"
#include "time.h" 
#include "DlgOptisequence.h"


#define ROTATE_ANGLE   -0.08726646260
#define ROTATE_ANGLE_NEG   0.08726646260
class MyQT : public QMainWindow
{
	Q_OBJECT

public:
	MyQT(QWidget *parent = 0);
	~MyQT();
public:
	WGLWidge  *opgl;
private:   
	Ui::MyQTClass *ui;
public:
	QPoint pOld;
	bool MouseDown;
	bool m_bifCtrl;
	QPoint mousepickpoint;

	int m_laplacian;
	double simpletime;
	VECTOR3D    m_VecDir;
	float m_equdept,m_mindept,Diamer,rate,ratepercent;

	
	
public:
	/////主窗口绘制函数/////////
	void keyPressEvent(QKeyEvent *e);
	void paintEvent(QPaintEvent *); 
	void wheelEvent(QWheelEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void  mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void setprogressbar();
	void Displayparameter(int choe);
	///////////槽函数////////
public slots :

/////////////////////视图函数/////////////////////////////
	void ViewBack();
	void ViewBottom();
	void ViewFront();
	void ViewLeft();
	void ViewRight();
	void ViewTop();
	void ViewIsometric();
	void ViewZoomall();
	void ViewZoomin();
	void ViewShade();
	void Viewshow();
	void ShowBoard();
	void ShowBox();
	void ShowBlock();

	void ActionMenu();
	void ActionWelcome();
	void ActionBlock();
	void ActionPathdesign();
	/////////////////////功能函数/////////////////////////////
	void FileOpen();
	void Linput(QString);

	void MeshZoom();
	void MeshSave();

	void SimplifyGrid();
	void Calmeshdegree();


	void Detectvetex();
	void Detectedge();
	void Skeletonizing();

	void Laplacian();
	void Laplacianback();
	void LaplacianFeature();

	POINT3D ScreenToPoint(QPoint P);
	void CutLineshow();
	void CutLineTrace();
	void CutLineDir();

	void MeshBlock();//分块
	void MeshBlockOpti();
	void DetectBlock();//修补孔洞
	void BlockSave();
	void BlockSwitch();//分块结果切换
	
	void DirTackBack();//与优化方向反向
	void ShowOpti();//显示块分层方向
	void ChangeDir();//交互输入分层方向
	void OptiDir();//子块最优成型方向优化

	void BlockSQ();//优化子块加工顺序
	void ShowSeQ();//切换显示加工顺序
	void ShowHeadbox();//显示打印头包围盒
	void Ceshi();

	void Slicing();//切片
	void Shadeslice();//隐藏切片

	void OnFill();//偏置填充
	void SeqLayerShow();//按照块加工顺序分层显示加工轨迹
	void saveFillResult();//以记事本方式保存路径结果

	void SimlateResult();
	void SimlastBlock();
	void SimNextBlock();
};

#endif // MYQT_H
