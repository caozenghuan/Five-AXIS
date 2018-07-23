#pragma once
#include "polygon.h"
#include "solid.h"

class COpenGLDC;

class CSolidPolygon:public CSolid
{
public:
	vec_PPOLYPOLYGON	m_vecPPolyPolygons;

	//BOX3D		m_BoxSolidPolygon;

	CSolidPolygon(){};
	virtual ~CSolidPolygon(){ };

	//display
	virtual void Draw(COpenGLDC* pDC) = 0;

	virtual bool	IsEmpty() = 0 ;
	virtual void	UpdateBox() = 0;
	//copy model
	virtual CSolid * CopySolid()		= 0;
	//save transform or not
	virtual void	Transform(bool bAddHis = true)		= 0;

	virtual void	Clear() = 0;
	virtual void	InitiatePolyPolygons(int nPolyPolygonNum) = 0;
};

//extern "C" AFX_EXT_API CSolidPolygon*  WINAPI CreateSolidPolygon();