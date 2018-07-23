#include <glut.h>
#include <glaux.h>

#include "GCamera.h"


GCamera::GCamera(void)
{
}
GCamera::~GCamera(void)
{
}
void GCamera::init()
{
	m_eye = POINT3D(0,0,1000.0);      //视点位置（0，0,1000）
	m_ref = POINT3D(0,0,0);           //参考点位置（0，0，0）
	m_vecUp = VECTOR3D(0,1,0);         //视点向上方向（0，1，0） 

	m_far = 10000;
	m_near = 1;
	m_width = 2400.0;
	m_height = 2400.0;

	m_screen[0] = 400;
	m_screen[1] = 400;
	
}

void GCamera::projection()
{
	glMatrixMode(GL_PROJECTION);       //切换到投影变换矩阵设置
	glLoadIdentity();                  //初始化投影矩阵
	glRenderMode(GL_RENDER);           //渲染模式

	//创建投影矩阵
	double left    =  -m_width/2.0;
	double right   =  m_width/2.0;
	double bottom  =  -m_height/2.0;
	double top     =  m_height/2.0;

	glOrtho(left,right,bottom,top,m_near,m_far);    //直角投影方式

	glMatrixMode(GL_MODELVIEW);                     //切换到视图变换矩阵设置
	glLoadIdentity();                               //初始化视图变换矩阵

	//设置视点位置和观察方向
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z,m_vecUp.dx,m_vecUp.dy,m_vecUp.dz);
	

}

void GCamera::set_screen(int x,int y)
{
	glViewport(0,0,x,y);
	if(y==0)y=1;
	double ratio = (double)x/(double)y;
	m_width *= (double)x/m_screen[0];
	m_height *= (double)y/m_screen[1];
	m_width = m_height*ratio;

	m_screen[0] = x;
	m_screen[1] = y;
}

void GCamera::set_eye(double eye_x,double eye_y,double eye_z)
{
	m_eye.x = eye_x;
	m_eye.y = eye_y;
	m_eye.z = eye_z;
}

void GCamera::set_ref(double ref_x,double ref_y,double ref_z)
{
	m_ref.x = ref_x;
	m_ref.y = ref_y;
	m_ref.z = ref_z;
}

void GCamera::set_vecUp(double up_dx,double up_dy,double up_dz)
{
	m_vecUp.dx = up_dx;
	m_vecUp.dy = up_dy;
	m_vecUp.dz = up_dz;
}

void GCamera::set_view_ret(double width,double height)
{
	m_width = width;
	m_height = height;
	double aspect = m_screen[0]/m_screen[1];
	m_width = m_height*aspect;
}

void GCamera::get_view_rect(double& width,double& height)
{
	width = m_width;
	height = m_height;
}

void GCamera::zoom(double scale)
{
	Q_ASSERT(scale > 0.0);   //缩放尺寸必须大于0
	m_height *= scale;     //缩放视景体的高
	m_width  *= scale;     //缩放视景体的宽
}

void GCamera::zoom_all(double x0,double y0,double z0,double x1,double y1,double z1)
{
	double width,height;
	double xx,yy,zz;

	//模型包容盒的长宽高
	xx = x1-x0;
	yy = y1-y0;
	zz = z1-z0;

	//计算能够包含模型包容盒的视景体的宽和高
	width = max(max(xx,yy),zz);
	height = max(max(xx,yy),zz);

	//重新设置视景体的宽和高
	set_view_ret(width,height);

	//移动视点和参考点
	VECTOR3D vec = m_eye - m_ref;
	m_ref.x = (x0+x1)/2;
	m_ref.y = (y0+y1)/2;
	m_ref.z = (z0+z1)/2;
	m_eye = m_ref + vec;
}

void GCamera::set_view_type(int type)   //典型观察视图选择
{
	double r;
	VECTOR3D vec;

	vec = m_ref - m_eye;    //矢量vec表示视线方向
	r = vec.GetLength();    //视点与参照点的距离

	if(IS_ZERO(r)) r = 50.0;//防止视点与参照点重合
	if(r > 10000)  r = 10000;//防止视点距离参照点太远

	switch(type){
	case VIEW_FRONT:                       //前视图
		m_eye = m_ref+VECTOR3D(0,-r,0);    //移动视点位置
		m_vecUp = VECTOR3D(0,0,1);
		break;
	case VIEW_BACK:                       //后视图
		m_eye = m_ref+VECTOR3D(0,r,0);    //移动视点位置
		m_vecUp = VECTOR3D(0,0,1);
		break;
	case VIEW_TOP:                        //俯视图
		m_eye = m_ref+VECTOR3D(0,0,r);    //移动视点位置
		m_vecUp = VECTOR3D(0,1,0);
		break;
	case VIEW_BOTTOM:                      //顶视图
		m_eye = m_ref+VECTOR3D(0,0,-r);    //移动视点位置
		m_vecUp = VECTOR3D(0,1,0);
		break;
	case VIEW_RIGHT:                       //右视图
		m_eye = m_ref+VECTOR3D(r,0,0);    //移动视点位置
		m_vecUp = VECTOR3D(0,0,1);
		break;
	case VIEW_LEFT:                         //左视图
		m_eye = m_ref+VECTOR3D(-r,0,0);    //移动视点位置
		m_vecUp = VECTOR3D(0,0,1);
		break;
	case VIEW_SW_ISOMETRIC:                //SW 轴侧图
		m_eye = m_ref+VECTOR3D(-1,-1,1).GetNormal()*r;
		update_upVec();
		break;
	case VIEW_SE_ISOMETRIC:                //SE 轴侧图
		m_eye = m_ref+VECTOR3D(1,-1,1).GetNormal()*r;
		update_upVec();
		break;
	case VIEW_NE_ISOMETRIC:                //NE 轴侧图
		m_eye = m_ref+VECTOR3D(1,1,1).GetNormal()*r;
		update_upVec();
		break;
	case VIEW_NW_ISOMETRIC:                //NW 轴侧图
		m_eye = m_ref+VECTOR3D(-1,1,1).GetNormal()*r;
		update_upVec();
		break;
	}
}

void GCamera::update_upVec()
{
	VECTOR3D vec = m_ref - m_eye;  //视线方向矢量
	VECTOR3D zVec(0,0,1);
	VECTOR3D vec0;

	vec.Normalize();       //矢量单位化
	vec0 = vec*zVec;
	m_vecUp = vec0*vec;    //矢量m_vecUp与视线方向垂直
}

void GCamera::move_view(double dpx,double dpy)
{
	VECTOR3D vec;
	VECTOR3D xUp,yUp;

	vec = m_ref - m_eye;        //视线方向矢量
	vec.Normalize();            //单位化
	xUp = vec*m_vecUp;          //xUp：景物窗口的x轴对应于OpenGL用户坐标系的矢量
	yUp = xUp*vec;              //yUp：景物窗口的y轴对应于OpenGL用户坐标系的矢量

	m_eye -= xUp*m_width*dpx + yUp*m_height*dpy;//移动视点位置
	m_ref -= xUp*m_width*dpx + yUp*m_height*dpy;//移动参照点位置
}
POINT3D GCamera::selection(int xPos,int yPos)
{
	POINT3D P;
	GLint vp[4];
	
	glGetIntegerv(GL_VIEWPORT,vp);//获取当前视口信息
	//设置投影变换
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//切换到选择模式
	glRenderMode(GL_SELECT);
	//根据鼠标位置，定义选择矩阵。
	gluPickMatrix(xPos,vp[3]-yPos, 1, 1, vp );
	//乘以投影矩阵
	double left		=  - m_width/2.0;
	double right	=  m_width/2.0;
	double bottom	=  - m_height/2.0;
	double top		=  m_height/2.0;
	 
	ox=oy=oz=0;
	glOrtho(left,right,bottom,top,m_near,m_far);
	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);		//获得模型变换矩阵
	glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);		//获得投影变换矩阵
	float fZValue = 0;
	glReadPixels(xPos,Viewport[3]-yPos,1,1,	GL_DEPTH_COMPONENT,GL_FLOAT,&fZValue );	
	
	gluUnProject(xPos, Viewport[3]-yPos,fZValue, ModelMatrix,ProjMatrix, Viewport,	&ox, &oy, &oz);
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	gluLookAt(m_eye.x,m_eye.y,m_eye.z,m_ref.x,m_ref.y,m_ref.z, m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);
	P.x=ox;
	P.y=oy;
	P.z=oz;
	return P;
}
void GCamera::BeginGetMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glRenderMode(GL_SELECT);

	//apply projective matrix
	double left = -m_width / 2.0;
	double right = m_width / 2.0;
	double bottom = -m_height / 2.0;
	double top = m_height / 2.0;

	glOrtho(left, right, bottom, top, m_near, m_far);

	GLint Viewport[4];
	glGetIntegerv(GL_VIEWPORT, Viewport);

	glGetDoublev(GL_MODELVIEW_MATRIX, ModelMatrix);		//获得模型变换矩阵
	glGetDoublev(GL_PROJECTION_MATRIX, ProjMatrix);		//获得投影变换矩阵

	//--------------------------------------------------------------------------
	for (int i = 0; i < 16; i++)
	{
		CurrentModelMatrix[i] = ModelMatrix[i];
		CurrentProjMatrix[i] = ProjMatrix[i];
	}

	for (int i = 0; i < 4; i++)
	{
		CurrentViewport[i] = Viewport[i];
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_eye.x, m_eye.y, m_eye.z, m_ref.x, m_ref.y, m_ref.z, m_vecUp.dx, m_vecUp.dy, m_vecUp.dz);

}
//绕屏幕给定轴旋转
void GCamera::rotate_view(double dRot, short snXyz)
{
	VECTOR3D rotVec;
	VECTOR3D origVec;
	MATRIX3D rotMtrx;

	VECTOR3D	mVec;

	/*mVec = m_ref - m_refCenter;

	m_ref = m_refCenter;
	m_eye = m_eye - mVec;*/

	origVec = m_eye - m_ref;

	switch (snXyz)
	{
	case 0:			//绕屏幕X轴方向旋转
		rotVec = m_vecUp * origVec;			//旋转轴
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot, rotVec);

		//修正m_vecUp
		m_vecUp *= rotMtrx;
		//修正视点位置
		origVec *= rotMtrx;
		m_eye = m_ref + origVec;
		break;
	case 1:			//绕屏幕Y轴方向旋转
		//旋转轴为m_vecUp
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot, m_vecUp);
		//不需要修正m_vecUp
		//修正视点位置
		origVec *= rotMtrx;
		m_eye = m_ref + origVec;

		break;
	case 2:			//绕屏幕Z轴方向旋转
		//旋转轴为origVec
		rotMtrx = rotMtrx.CreateRotateMatrix(dRot, origVec);
		//修正m_vecUp
		m_vecUp *= rotMtrx;
		//不需修正视点位置
		break;
	default:
		break;
	}

}
