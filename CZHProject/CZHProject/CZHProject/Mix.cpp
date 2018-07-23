#include "Mix.h"

CMix::CMix(void)
{
}

CMix::~CMix(void)
{

}
void CMix::GetMixFill(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth)
{
	vec_PPOLYPOLYGON  vec_lastoffset;
	int degree=4;
	Coffset::GetOffset(m_vecPPolyPolygons, m_VecDir, m_vecFillP, FillLineWidth, degree, vec_lastoffset);
	Coffset::pretreatment(vec_lastoffset);
	for (int z=0;z<vec_lastoffset.size();z++)
	{
		vec_PPOLYPOLYGON  vec_polypoly;
		int sameCN = vec_lastoffset[z]->m_vecPPolygons.size();
		int maxClassfiy=vec_lastoffset[z]->maxclassfiy;
		for (int i=0;i<maxClassfiy;i++)
		{
			vec_polypoly.push_back(new POLYPOLYGON);
		}
		for (int classfiy=1;classfiy<maxClassfiy+1;classfiy++)
		{
			for (int i=0;i<sameCN;i++)
			{
				if (vec_lastoffset[z]->m_vecPPolygons[i]->m_IoriClass==classfiy)
				{
					vec_polypoly[classfiy-1]->m_vecPPolygons.push_back(vec_lastoffset[z]->m_vecPPolygons[i]);
				}
			}
		}
		vec_PPOLYPOLYGON  m_vecFillPTemp;
		CLine::GetContourLine(vec_polypoly,m_VecDir, m_vecFillPTemp,FillLineWidth);
		for (int i=1;i<m_vecFillPTemp.size();i++)
		{
			m_vecFillPTemp[0]->m_vecPPolygons.insert(m_vecFillPTemp[0]->m_vecPPolygons.end(),
				m_vecFillPTemp[i]->m_vecPPolygons.begin(),m_vecFillPTemp[i]->m_vecPPolygons.end());
			//delete m_vecFillPTemp[i];
		}
		m_vecFillP[z]->m_vecPPolygons.insert(m_vecFillP[z]->m_vecPPolygons.end(),
			m_vecFillPTemp[0]->m_vecPPolygons.begin(),m_vecFillPTemp[0]->m_vecPPolygons.end());
	}
}