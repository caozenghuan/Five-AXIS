#include "offset.h"

class ChelicalPath
{
public:
	ChelicalPath(void);
	~ChelicalPath(void);
public:
	//vec_PPOLYPOLYGON m_vecALLhelical;

public:
	static void GetheilcalPath(vec_PPOLYPOLYGON   &m_vecFillP);
	//static void DrawheilcalPath(COpenGLDC* pDC);
	static void becomeheilcalPath(vec_PPOLYGON vecTempPPolygons,PPOLYPOLYGON vecNewPPolygons);
	static double GetChordLength(PPOLYGON poly,int StopI);
	static POINT3D  MinDofPoint2piont(POINT3D p,PPOLYGON ploy);
};