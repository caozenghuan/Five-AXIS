#pragma once

#include "datastructure/GeomBase.h"

#define VIEW_FRONT         0
#define VIEW_BACK          1
#define VIEW_TOP           2
#define VIEW_BOTTOM        3
#define VIEW_RIGHT         4
#define VIEW_LEFT          5
#define VIEW_SW_ISOMETRIC  6
#define VIEW_SE_ISOMETRIC  7
#define VIEW_NE_ISOMETRIC  8
#define VIEW_NW_ISOMETRIC  9

//#define ZOOM_ALL           10
//#define ZOOM_IN            11
//#define ZOOM_OUT           12

class GCamera
{
protected:
	//视点位置和观察方向
	POINT3D      m_eye;
	POINT3D      m_ref;
	VECTOR3D     m_vecUp;

	//视景体参数
	double       m_far,m_near;
	double       m_width,m_height;

	//视口参数
	double       m_screen[2];
public:
	GCamera(void);
	~GCamera(void);
	//初始化函数
	void init();
	POINT3D selection(int xPos,int yPos);//选取模式
	//取景函数,设置绘图模式下的投影变换
	void projection();
	void BeginGetMatrix();
	GLdouble ModelMatrix[16]; 
	GLdouble ProjMatrix[16];
	GLdouble			CurrentModelMatrix[16];		//当前模型矩阵
	GLdouble			CurrentProjMatrix[16];			//当前投影矩阵
	GLint				CurrentViewport[4];				//当前视口
	//选择定义，设置选择模式下的投影变换
	//void selection(int xPos,int yPos);

	//景物缩放
	void zoom(double scale);
	void zoom_all(double x0,double y0,double z0,double x1,double y1,double z1);
	GLdouble ox, oy, oz;

	//景物平移
	void move_view(double dpx,double dpy);

	//选择典型视图
	void set_view_type(int type);

	//设置视口尺寸
	void set_screen(int x,int y);

	//设置视点位置和方向
	void set_eye(double eye_x,double eye_y,double eye_z);
	void set_ref(double ref_x,double ref_y,double ref_z);
	void set_vecUp(double up_dx,double up_dy,double up_dz);

	//设置视景体
	void set_view_ret(double width,double height);
	void get_view_rect(double& width,double& height);
	void rotate_view(double dRot, short snXyz);
protected:
	//计算视线上方向的矢量
	void update_upVec();
};

