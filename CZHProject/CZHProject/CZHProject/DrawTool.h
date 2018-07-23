#pragma  once
#include "datastructure/GeomBase.h"
#include <glaux.h>
#include "QtOpenGL"
#include "glut.h"
#include<iostream>
#include<string>
#include<sstream>
using namespace std;
class drawTool 
{
public:
	drawTool();
	virtual ~drawTool();
	COLORREF m_clrMaterial; //材料的颜色
	COLORREF m_clr;         //非光照模式下的模型颜色

	COLORREF	m_clrBkMaterial;  //for normal rendering
	void SetMaterialColor(COLORREF clr);            //设置与获取材料颜色
	void GetMaterialColor(COLORREF& clr);
	void SetBkMaterialColor(COLORREF clr);
	void SetColor(COLORREF rgb);                  //设置与获取非光照模式下的绘制颜色
	void GetColor(COLORREF& rgb);

	static void	DrawText(char* string, int flag = 12);
	string itos(int i);//int转string类型
public:
	void DrawBox(BOX3D bot);
	void DrawHeadBox(vector<vec_POINT3D> vec_face, COLORREF clr);//绘制打印头
	void DrawSphere(const POINT3D& cen, double r);
	void DrawPickPoint(const vector<POINT3D> &modelPOINT);//绘制交互拾取点
	void DrawPoint(const POINT3D&);            //绘制一个空间点
	void DrawLine(const POINT3D& sp, const POINT3D& ep);//绘制一条直线
	void Drawcone(const POINT3D &cen, double r, VECTOR3D &h);
	virtual void DrawTriChip(double n0, double n1, double n2, double v00, double v01, double v02,       //绘制一个三角面片
		double v10, double v11, double v12, double v20, double v21, double v22);
};