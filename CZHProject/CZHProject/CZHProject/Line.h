
#pragma once
#include "offset.h"
typedef struct SortX
{
	POINT3D a;
	bool operator < (SortX const & _A)const
	{
		if((a.y<_A.a.y)||((a.y==_A.a.y)&&(a.x<_A.a.x)))return true;
		return false;
	}
}SORTX , *PSORTX;

typedef struct PntAndNum
{
	POINT3D p;
	int    num;
};


class CLine 
{
public:
	CLine(void);
	~CLine(void);
public:
	//vector<set<SORTX>> m_vecpaddN;
	//vec_PPOLYPOLYGON   m_vecALLPaddingP;
public:
	static void GetLine(vec_PPOLYPOLYGON m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth);
	static void GetContourLine(vec_PPOLYPOLYGON vec_contour,VECTOR3D m_VecDir,vec_PPOLYPOLYGON &m_vecFillP,double FillLineWidth);///重载适用于混合填充
	//void DrawPadding(COpenGLDC* pDC);
	static bool CrossY(POINT3D p3,POINT3D p4, double y, POINT3D  &p);
	static void SortXmin(PPOLYGON singleLine);
	static void becomePadLine(PPOLYPOLYGON  SameCLine,PPOLYPOLYGON TempLine,double FillLineWidth);
	static void OptiLine(PPOLYPOLYGON  SameCLineTemp,PPOLYPOLYGON SameCLine,double FillLineWidth);
	static PPOLYPOLYGON tailor(PPOLYGON contour,PPOLYGON curve);
};


