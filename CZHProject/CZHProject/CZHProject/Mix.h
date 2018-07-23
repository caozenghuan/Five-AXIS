#pragma  once

#include "offset.h"
#include "Line.h"

class CMix
{
public:
	CMix(void);
	~CMix(void);
public:
	static void GetMixFill(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth);
	//static int   degree;

};

