#include "adaptslice.h"


adaptslice::adaptslice()
{
}


adaptslice::~adaptslice()
{
}
vector<float> adaptslice::Saliency(STLModell *pSTL)
{
	int nVertnum = pSTL->m_vecPVert.size();
	vector<float> vec_Gauss;
	vector<float>   vec_Sal;
	for (int nct = 0; nct < nVertnum; nct++)
	{
		PVERT pter = pSTL->m_vecPVert[nct];
		float curve = CalcuVerMeanCurvature(pter);
		vec_Gauss.push_back(curve);
	}

	return vec_Sal;
}




float adaptslice::CalcuVerMeanCurvature(PVERT p00)/////计算顶点的平均曲率
{
	int nct=p00->vec_OneFac.size();
	float angel = 0,Tarea=0,k;
	for (int i = 0; i < nct;i++)
	{
		FacetTri ptri = *p00->vec_OneFac[i];
		vector<PVERT> pu;
		for (int j = 0; j < 3;j++)
		{
			if (ptri.m_PVerts[j]!=p00)
			{
				pu.push_back(ptri.m_PVerts[j]);
			}
			VECTOR3D u0, u1;
			u0 = *pu[0] - *p00; u1 = *pu[1] - *p00;
			angel+=_AngleBetween3D(u0, u1);	
		}
		POINT3D t00, t01,t02;
		t00 = (*pu[0] + *p00)*0.5;
		t01 = (*pu[1] + *p00)*0.5;
		t02 = (*pu[0] + *pu[1])*0.5;
		Tarea+=Trianglearea(t00, t01, t02);
		delete pu[0]; delete pu[1];
	}
	k = ( PI+PI - angel) / Tarea;
	return k;
}
double adaptslice::Trianglearea(POINT3D p0, POINT3D p1, POINT3D p2)
{
	tagMatrix3D matrix;
	matrix.A[0][0] = p0.x - p2.x;
	matrix.A[1][1] = p1.y - p2.y;
	matrix.A[2][2] = matrix.A[0][2] = matrix.A[1][2] = 1;
	matrix.A[0][1] = p1.x - p2.x;
	matrix.A[1][0] = p0.y - p2.y;
	matrix.A[2][0] = p0.z - p2.z;
	matrix.A[2][1] = p1.z - p2.z;
	double s = matrix.GetValue()*0.5;
	return s;
}