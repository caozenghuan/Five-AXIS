#pragma once
#include "offset.h"

typedef  struct Offsetclassfiy{

	int classfiy;
	vector<int> underClassfiy;
	Offsetclassfiy()
	{
		classfiy=0;
	}
};

class Cunicursal
{
public:
	Cunicursal(void);
	~Cunicursal(void);
public:	
	static void Getunicursal(vec_PPOLYPOLYGON m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth);
	static PPOLYGON  becomeUnicursal(PPOLYPOLYGON  polypoly,double FillLineWidth); 
	static PPOLYGON  becomeOnePoly(PPOLYPOLYGON  polypoly,double FillLineWidth);
	static vector<int> findPolyMinD(PPOLYGON poly1,PPOLYGON poly2);
	static int findClosestPoint(POINT3D p,PPOLYGON  poly,double FillLineWidth);
	//static void IntoOut(PPOLYPOLYGON polypoly,PPOLYGON poly,double FillLineWidth);
	static void IntoNoOut(PPOLYPOLYGON polypoly, PPOLYGON poly,double FillLineWidth);
	static void LineWidthPop(PPOLYGON poly,double FillLineWidth);///////针对一个多边形首尾删除留出一个偏置距离
	//static void LineWidthPop(PPOLYGON poly,double FillLineWidth,int insert_id);///针对多边形指定位置前后删除留出一个偏置距离

	//static int  findinsertPoint(PPOLYGON  ploy1,PPOLYGON ploy2,double FillLineWidth);///外找里
	static int  findOutPoint(PPOLYGON  ploy1,PPOLYGON ploy2,double FillLineWidth);//里找外
};