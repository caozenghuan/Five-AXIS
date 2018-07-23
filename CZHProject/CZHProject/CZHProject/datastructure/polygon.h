#pragma once

#include "GeomBase.h"

#if 0
typedef struct tagPolygon 
{
	vec_PPOINT3D	m_vecPPnts;
	bool			m_bIsClosed;

	tagPolygon()
	{
		m_bIsClosed = false;
	}

	~tagPolygon()
	{
		int nNum, nCnt;
		nNum = m_vecPPnts.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPPnts[nCnt];
		}
		m_vecPPnts.clear();
	}
} POLYGON, *PPOLYGON;
#endif

typedef struct tagPolygon 
{
	vec_POINT3D	m_vecPnts;
	bool		m_bIsClosed;
	BOX3D		m_BoxPolygon;
	int         m_Iclassify;
	int         m_IoriClass;
	void UpdateBox()
	{
		int nNum = m_vecPnts.size();
		if (nNum < 1)
		{
			return;
		}
		int nCnt;
		POINT3D p3d;
		p3d = m_vecPnts[0];
		m_BoxPolygon = BOX3D(p3d,p3d);
		for (nCnt = 1; nCnt < nNum; nCnt++)
		{
			p3d = m_vecPnts[nCnt];
			if (m_BoxPolygon.x0 > p3d.x)
			{
				m_BoxPolygon.x0 = p3d.x;
			} 
			else
			{
				if (m_BoxPolygon.x1 < p3d.x)
				{
					m_BoxPolygon.x1 = p3d.x;
				}
			}

			if (m_BoxPolygon.y0 > p3d.y)
			{
				m_BoxPolygon.y0 = p3d.y;
			} 
			else
			{
				if (m_BoxPolygon.y1 < p3d.y)
				{
					m_BoxPolygon.y1 = p3d.y;
				}
			}

			if (m_BoxPolygon.z0 > p3d.z)
			{
				m_BoxPolygon.z0 = p3d.z;
			} 
			else
			{
				if (m_BoxPolygon.z1 < p3d.z)
				{
					m_BoxPolygon.z1 = p3d.z;
				}
			}
		}
	}

	tagPolygon()
	{
		m_bIsClosed = false;
	}

	~tagPolygon()
	{
		m_vecPnts.clear();
	}
} POLYGON, *PPOLYGON;

typedef	vector<PPOLYGON> vec_PPOLYGON;
typedef vec_PPOLYGON::iterator it_vec_vec_PPOLYGON;
typedef	vector<POLYGON> vec_POLYGON;
typedef vec_POLYGON::iterator it_vec_POLYGON;


#if 0
typedef struct tagPolyPolygon
{
	vec_PPOLYGON	m_vecPPolygons;

	~tagPolyPolygon()
	{
		int nNum, nCnt;
		nNum = m_vecPPolygons.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPPolygons[nCnt];
		}
		m_vecPPolygons.clear();
	}
} POLYPOLYGON, *PPLOYPOLYGON;
#endif

typedef struct tagPolyPolygon
{
	vec_PPOLYGON	m_vecPPolygons;

	BOX3D		m_BoxPolyPolygon;
	int         maxclassfiy;
	void UpdateBox()
	{
		int nNum = m_vecPPolygons.size();
		if (nNum < 1)
		{
			return;
		}
		int nCnt;

		m_vecPPolygons[0]->UpdateBox();
		m_BoxPolyPolygon = m_vecPPolygons[0]->m_BoxPolygon;
		for (nCnt = 1; nCnt < nNum; nCnt++)
		{
			m_vecPPolygons[nCnt]->UpdateBox();
			m_BoxPolyPolygon += m_vecPPolygons[nCnt]->m_BoxPolygon;
		}
	}

	~tagPolyPolygon()
	{
		int nNum, nCnt;
		nNum = m_vecPPolygons.size();
		for (nCnt = 0; nCnt < nNum; nCnt++)
		{
			delete (m_vecPPolygons[nCnt]);
		}
		m_vecPPolygons.clear();
	}
} POLYPOLYGON, *PPOLYPOLYGON;


typedef	vector<PPOLYPOLYGON> vec_PPOLYPOLYGON;
typedef vec_PPOLYPOLYGON::iterator it_vec_vec_PPOLYPOLYGON;
typedef	vector<POLYPOLYGON> vec_POLYPOLYGON;
typedef vec_POLYPOLYGON::iterator it_vec_POLYPOLYGON;