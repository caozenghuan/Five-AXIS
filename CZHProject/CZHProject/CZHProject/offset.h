#pragma once
#include"KitTool.h"
#include "datastructure/polygon.h"
#include "STLModell.h"
typedef struct lineIntersect
{
	POINT3D p;
	int   line1Num;
	int   line2Num;
	bool  ifInto;
	bool  beused;

}LineInterS,*pLineInterS;

class Coffset
{
public:
	Coffset(void);
	~Coffset(void);
public:
	
public:
	static void GetOffset(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON & m_vecFillP, double FillLineWidth);
	static void GetOffset(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON & m_vecFillP, double FillLineWidth, int degree
						,vec_PPOLYPOLYGON &vec_lastoffset);///////用于混合填充的偏置
	static void pretreatment(vec_PPOLYPOLYGON m_vecPPolyPolygons);///////预处理，处理分层后的轮廓线之间的关系
	
	static void ifdeleOffset(int &m,PPOLYPOLYGON afteroffset,double FillLineWidth);
	
	static int selfing(PPOLYGON beoffsetpoly,  PPOLYGON poly,PPOLYPOLYGON  pPolyPoly);////去自交
	static int incise (PPOLYPOLYGON afteroffset,PPOLYPOLYGON ploy,int & originalNum);//处理相互干涉
	static bool IfIncise(PPOLYGON ploy1,PPOLYGON ploy2,PPOLYPOLYGON  Newpolypoly,bool ifOutCutOut);//2条曲线之间的相互干涉
	
	static PPOLYGON  offsetpiont(double d,PPOLYGON poly);//求单个曲线的偏置曲线函数

	static bool  ifstop(PPOLYPOLYGON ploy,double d);//判断是否停止函数
	static void offsetSort(PPOLYPOLYGON ployploy);//排序从小到大，并且每个数字相连
	static void offsetClassify(int &m ,int n,POLYGON beoffsetpoly,PPOLYPOLYGON ployploy,double threshold);//对偏置曲线进行分类
	static void FlattenPoly(PPOLYGON poly1,PPOLYGON poly2,vector<LineInterS> vec_lineInter,PPOLYPOLYGON Newpolypoly);//曲线拼接
	static void ifPopBack(POINT3D p1,PPOLYGON poly1);//是否删除最后一点
	static void ExtractPoint(int Num1,int Num2,PPOLYGON poly1,PPOLYGON poly);//提取点
	
	static void OffsetCircle(double degree, VECTOR3D vec_Dir, PPOLYGON ploy, PPOLYPOLYGON vec_poly);//我的环偏置
	static void  OffsetCircle1(double FillLineWidth, VECTOR3D vec_Dir, PPOLYGON ploy, PPOLYPOLYGON &m_vecFillP);//郑红的
	static void GetOffset1(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON & m_vecFillP, double FillLineWidth);
};
