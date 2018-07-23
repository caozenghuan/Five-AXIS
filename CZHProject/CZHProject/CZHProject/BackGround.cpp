
#include "BackGround.h"
using namespace ViewSpace;

void CBackGround::SetBlackGround(int Modeway)
{
	switch (Modeway)
	{
	case 0:
	{
		RenderBackGround(CHANGE_LIGHT_BLUE);
		break;
	}
	case 1:
	{
		RenderBackGround(CHANGE_GRAY);
		break;
	}
	case 2:
	{
		RenderBackGround(CHANGE_DEONGAREE);
		break;
	}
	case 3:
	{
		// 保存矩阵并载入单位矩阵
		glEnable(GL_COLOR_MATERIAL);//启用顶点颜色，没有启用的情况下glColor4f不起作用
		glDisable(GL_LIGHTING);////作为背景色需要暂时关闭光照，避免其他物体散色光反射，从而影响背景矩形
		glDisable(GL_LIGHT0);
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glBegin(GL_QUADS);

		glEnd();
		glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 1);
		glVertex3f(-1, -1, 0.99);
		glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 1);
		glVertex3f(-1, 1, 0.99);
		glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 1);
		glVertex3f(1, 1, 0.99);
		glColor4f(255.0 / 255, 255.0 / 255, 255.0 / 255, 1);
		glVertex3f(1, -1, 0.99);

		// 恢复原来的矩阵
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glDisable(GL_COLOR_MATERIAL);//启用材质自己的颜色，而非顶点颜色
		glEnable(GL_LIGHTING);//打开光照
		glEnable(GL_LIGHT0);
	}
	}
}
void CBackGround::RenderBackGround(BackGroundColor backgroundcolor)
{
	//克隆
	// 保存矩阵并载入单位矩阵
	glEnable(GL_COLOR_MATERIAL);//启用顶点颜色，没有启用的情况下glColor4f不起作用
	glDisable(GL_LIGHTING);////作为背景色需要暂时关闭光照，避免其他物体散色光反射，从而影响背景矩形
	glDisable(GL_LIGHT0);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_QUADS);
	switch(backgroundcolor)
	{
	case BackGroundColor::CHANGE_LIGHT_BLUE:
		{
			glColor4f(255.0/255,255.0/255,255.0/255,1);
			glVertex3f(-1, -1,0.99);	
			glColor4f(191.0/255,239.0/255,255.0/255,1);
			glVertex3f(-1,  1,0.99);	
			glColor4f(191.0/255,239.0/255,255.0/255,1);
			glVertex3f(1,  1,0.99);
			glColor4f(255.0/255,255.0/255,255.0/255,1);	
			glVertex3f( 1, -1,0.99);
			break;
		}
	case BackGroundColor::CHANGE_GRAY:
		{
			glColor4f(227.0/255,225.0/255,213.0/255,1);
			glVertex3f(-1, -1,0.99);	
			glColor4f(158.0/255,155.0/255,145.0/255,1);
			glVertex3f(-1,  1,0.99);	
			glColor4f(158.0/255,155.0/255,145.0/255,1);
			glVertex3f(1,  1,0.99);
			glColor4f(227.0/255,225.0/255,213.0/255,1);	
			glVertex3f( 1, -1,0.99);
			break;
		}
	case BackGroundColor::CHANGE_DEONGAREE:
		{
			glColor4f(0.0/255,93.0/255,155.0/255,1);
			glVertex3f(-1, -1,0.99);	
			glColor4f(0.0/255,17.0/255,50.0/255,1);
			glVertex3f(-1,  1,0.99);	
			glColor4f(0.0/255,17.0/255,50.0/255,1);
			glVertex3f(1,  1,0.99);
			glColor4f(0.0/255,93.0/255,155.0/255,1);
			glVertex3f( 1, -1,0.99);
			break;
		}
	default:
		{
			glColor4f(92.0/255,125.0/255,169.0/255,1);
			glVertex3f(-1, -1,0.99);	
			glColor4f(198.0/255,220.0/255,236.0/255,1);
			glVertex3f(-1,  1,0.99);	
			glColor4f(198.0/255,220.0/255,236.0/255,1);
			glVertex3f(1,  1,0.99);
			glColor4f(92.0/255,125.0/255,169.0/255,1);	
			glVertex3f( 1, -1,0.99);
			break;
		}

	}
	glEnd();

	// 恢复原来的矩阵
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glDisable(GL_COLOR_MATERIAL);//启用材质自己的颜色，而非顶点颜色
	glEnable(GL_LIGHTING);//打开光照
	glEnable(GL_LIGHT0);
}

//绘制坐标及模型顶点三角面片信息
//void CBackGround::RenderInformation(TriMesh*Tmesh)
//{
//	glEnable(GL_COLOR_MATERIAL);//启用顶点颜色，没有启用的情况下glColor4f不起作用
//	glDisable(GL_LIGHTING);////作为背景色需要暂时关闭光照，避免其他物体散色光反射，从而影响背景矩形
//	glMatrixMode(GL_PROJECTION);
//	glPushMatrix();
//	glLoadIdentity();
//	glMatrixMode(GL_MODELVIEW);
//	glPushMatrix();
//	glLoadIdentity();
//	glColor3f(0,0,0);
//	glRasterPos2f(-0.99,-0.97);
//	DrawText("triangle:",12);
//	CString str;
//	str.Format("%d",Tmesh->faces.size());
//	char *pBuff=str.GetBuffer(0);
//	DrawText(pBuff,12);
//	glRasterPos2f(-0.99,-0.90);
//	DrawText("vertice:",12);
//	CString vn;
//	vn.Format("%d",Tmesh->vertices.size());
//	pBuff=vn.GetBuffer(0);
//	DrawText(pBuff,12);
//	// 恢复原来的矩阵
//	glMatrixMode(GL_PROJECTION);
//	glPopMatrix();
//	glMatrixMode(GL_MODELVIEW);
//	glPopMatrix();
//	glDisable(GL_COLOR_MATERIAL);//启用材质自己的颜色，而非顶点颜色
//	glEnable(GL_LIGHTING);//打开光照
//}

