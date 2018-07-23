#pragma once
#include "mesh.h"
#include "solid.h"


class CSolidMeshTri:public CSolid
{
public:		
	vec_PHEDGE	m_vecPHEdge;
	vec_PEDGE	m_vecPEdge;
	vec_PVERT	m_vecPVert;
	vec_PFACETTRI	m_vecPFacetTri;
	vec_PVECTOR3D m_vecFacetNorm;

public:	
	CSolidMeshTri(){SetSolidType(SOLID_MESH_TRI);};

	~CSolidMeshTri(){
		int nNum, nCnt;
		nNum = m_vecPHEdge.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPHEdge[nCnt];
		}
		m_vecPHEdge.clear();

		nNum = m_vecPEdge.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPEdge[nCnt];
		}
		m_vecPEdge.clear();

		nNum = m_vecPVert.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPVert[nCnt];
		}
		m_vecPVert.clear();

		nNum = m_vecPFacetTri.size();
		for (nCnt=0;nCnt < nNum; nCnt++)
		{
			delete m_vecPFacetTri[nCnt];
		}
		m_vecPFacetTri.clear();

		m_vecFacetNorm.clear();
	};
public:
/*	//vertex
	virtual int   GetVertNum()      = 0;
	virtual PVERT GetVertex(int ID) = 0;
	virtual PVERT GetVertex(PHEDGE pHEdge) = 0;

	//edge
	virtual PHEDGE GetHEdge(int ID) = 0;
	virtual PHEDGE GetHEdgeOut(PVERT pVert) = 0;
	virtual PHEDGE GetHEdgePair(PHEDGE pHEdge)  = 0;
	virtual PHEDGE GetHEdgeNext(PHEDGE pHEdge)  = 0;
	virtual PHEDGE GetHEdgePre(PHEDGE pHEdge)   = 0;

	//facet
	virtual int    GetFeactNum()    = 0;
	virtual PFACET GetFeact(int ID) = 0;
	virtual PFACET GetFeactAdjHE(PHEDGE pHEdge)  = 0;
	virtual PFACET GetFeactAdjF(PFACET pFacet)   = 0;
*/
};