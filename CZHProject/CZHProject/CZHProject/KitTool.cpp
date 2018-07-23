#include "KitTool.h"


KitTool::KitTool()
{
}


KitTool::~KitTool()
{
}

void KitTool::Ranktwodigit(double &d1, double &d2)
{
	double dtemp=0;
	if (d1>d2)
	{
		dtemp = d1;
		d1 = d2;
		d2 = dtemp;
	}
}
void KitTool::Rankthreedigit(PFACETTRI &PFc0,double d0, double d1, double d2)
{
	if (d0 > d1)
	{
		if (d0 > d2)
		{
			PFc0->Pvet= PFc0->m_PVerts[0];
		}
		else
		{
			PFc0->Pvet = PFc0->m_PVerts[2];
		}
	}
	else
	{
		if (d1 > d2)
		{
			PFc0->Pvet = PFc0->m_PVerts[1];
		}
		else
		{
			PFc0->Pvet = PFc0->m_PVerts[2];
		}
	}
}
void  KitTool::Rankthreedigit(PFACETTRI &PFc0, vector<double> re, double &value)
{
	if (re[0]>re[1])
	{
		if (re[0]  >re[2])
		{
			PFc0->Pvet = PFc0->m_PVerts[0];
			value = re[0];
		}
		else
		{
			PFc0->Pvet = PFc0->m_PVerts[2];
			value = re[2];
		}
	}
	else
	{
		if (re[1]  > re[2])
		{
			PFc0->Pvet = PFc0->m_PVerts[1];
			value = re[1];
		}
		else
		{
			PFc0->Pvet = PFc0->m_PVerts[2];
			value = re[2];
		}
	}
}
void KitTool::RankCircle(PPOLYGON &ploy)
{
	PPOLYGON ployTemp=new POLYGON;
	for (int i = ploy->m_vecPnts.size() - 1; i >= 0;i--)
	{
		ployTemp->m_vecPnts.push_back(ploy->m_vecPnts[i]);
	}
	ploy->m_vecPnts.clear();
	for (int i =0; i <ployTemp->m_vecPnts.size(); i++)
	{
		ploy->m_vecPnts.push_back(ployTemp->m_vecPnts[i]);
	}
	delete ployTemp;
}
void KitTool::Ranktwopoint(PHEDGE &PHEdge_1, PHEDGE &PHEdge_2)
{
	PHEDGE PHEdge_3;
	if (PHEdge_1->pVertEnd->z > PHEdge_2->pVertEnd->z)
	{
		PHEdge_3 = PHEdge_1;
		PHEdge_1 = PHEdge_2;
		PHEdge_2 = PHEdge_3;
	}
}
void KitTool::Ranktwoedge(PHEDGE &PHEdge_0, PHEDGE PHEdge_1, PHEDGE PHEdge_2)
{
	if (PHEdge_1->pHEdgePair->pVertEnd == PHEdge_2->pVertEnd)
	{
		PHEdge_0 = PHEdge_1;
	}
	else
	{
		PHEdge_0 = PHEdge_2;
	}
}


//求直线与平面的交点法一
bool KitTool::CalPlanePointIntPoint(POINT3D p00, POINT3D p10, float Zt, POINT3D &p)
{
	if (p00.z > p10.z)
	{
		POINT3D p03 = p00;
		p00 = p10;
		p10 = p03;
	}
	VECTOR3D  d1, d2, u, u1; POINT3D plane_point; double s, t;
	plane_point.x = 1; plane_point.y = 1; plane_point.z = Zt;
	u.dx = 0; u.dy = 0; u.dz = Zt;
	d1 = PlanePointDistance(p00, plane_point, u);
	d2 = PlanePointDistance(p10, plane_point, u);
	s = d1 | d2;
	if (s > 0)
	{
		return false;
	}
	if (fabs(s) == 0)
	{
		if ((d1.GetLength() == d2.GetLength() == 0) && (d1.dx == d2.dx == 0) && (d1.dy == d2.dy == 0) && (d1.dz == d2.dz == 0))
		{
			return false;
		}
		else
		{
			if ((d1.dx == 0) && (d1.dy == 0) && (d1.dz == 0)){ p = p00; }
			else { p = p10; }
		}
	}
	else
	{
		t = d2.GetLength() / (d2.GetLength() + d1.GetLength());
		u1 = p10 - p00;
		p = p10 - u1*t;
	}
	return true;
}
bool KitTool::CalPlaneLineIntPoint(POINT3D p00, POINT3D p10, POINT3D plane_point, VECTOR3D dir, POINT3D &p, int &vertType)
{
	if (p00.z > p10.z)
	{
		POINT3D p03 = p00;
		p00 = p10;
		p10 = p03;
	}
	VECTOR3D  d1, d2,u1;double s, t;
	d1 = PlanePointDistance(p00, plane_point, dir);
	d2 = PlanePointDistance(p10, plane_point, dir);
	s = d1 | d2;
	if (s > 0)
	{
		return false;
	}
	if (fabs(s) == 0)
	{
		if ((d1.GetLength() == d2.GetLength() == 0) && (d1.dx == d2.dx == 0) && (d1.dy == d2.dy == 0) && (d1.dz == d2.dz == 0))
		{
			return false;
		}
		else
		{
			if ((d1.dx == 0) && (d1.dy == 0) && (d1.dz == 0)){ p = p00; }
			else { p = p10; }
			vertType = 1;
		}
	}
	else
	{
		t = d2.GetLength() / (d2.GetLength() + d1.GetLength());
		u1 = p10 - p00;
		p = p10 - u1*t;
		vertType = 0;
	}
	return true;
}
bool KitTool::CalPlaneLineIntPoint(POINT3D p00, POINT3D p10, POINT3D plane_point, VECTOR3D dir, POINT3D &p)
{
	if (p00.z > p10.z)
	{
		POINT3D p03 = p00;
		p00 = p10;
		p10 = p03;
	}
	VECTOR3D  d1, d2, u1; double s, t;
	d1 = PlanePointDistance(p00, plane_point, dir);
	d2 = PlanePointDistance(p10, plane_point, dir);
	s = d1 | d2;
	if (s > 0)
	{
		return false;
	}
	if (fabs(s) == 0)
	{
		if ((d1.GetLength() == d2.GetLength() == 0) && (d1.dx == d2.dx == 0) && (d1.dy == d2.dy == 0) && (d1.dz == d2.dz == 0))
		{
			return false;
		}
		else
		{
			if ((d1.dx == 0) && (d1.dy == 0) && (d1.dz == 0)){ p = p00; }
			else { p = p10; }
			return false;
		}
	}
	else
	{
		t = d2.GetLength() / (d2.GetLength() + d1.GetLength());
		u1 = p10 - p00;
		p = p10 - u1*t;
	}
	return true;
}
//求直线与平面的交点法二
bool KitTool::CalPlaneLineIntersectPoint(POINT3D linePoint, POINT3D linePointend, float Zt, POINT3D &p)
{
	float  m1, m2, m3, r1, r2, r3, t;
	m1 = linePoint.x;
	m2 = linePoint.y;
	m3 = linePoint.z;
	r1 = linePointend.x;
	r2 = linePointend.y;
	r3 = linePointend.z;
	if (r3 == m3)
	{
		return false;
	}
	else
	{
		t = (Zt - m3) / (r3 - m3);
		if ((0 < t) && (1 >= t))
		{
			p.x = m1 + t*(r1 - m1);
			p.y = m2 + t*(r2 - m2);
			p.z = m3 + t*(r3 - m3);
		}
		else return false;
	}
	return true;
}
VECTOR3D KitTool::PlanePointDistance(POINT3D p0, POINT3D p1, VECTOR3D u)//点到平面的距离
{
	VECTOR3D d; VECTOR3D u1;
	u.Normalize();
	u1 = p0 - p1;
	double s = u1 | u;
	d = u*s;
	return d;
}
//确立sort比较法则
void KitTool::RankAllfact(STLModell* pMeshTri, vector<PFACETTRI> &vec_fac)
{
	PFACETTRI pFct; 
	int nFcnt = pMeshTri->m_vecPFacetTri.size();
	for (int nFct = 0; nFct < nFcnt; nFct++)
	{
		pFct = pMeshTri->m_vecPFacetTri[nFct];
		vec_fac.push_back(pFct);
	}
	sort(vec_fac.begin(), vec_fac.end(), RankPfact);
}
//确立sort比较法则
bool KitTool::RankPfact(PFACETTRI Pfact_0, PFACETTRI Pfact_1)
{
	float Z0, Z1;
	Thelest(Pfact_0, Z0);
	Thelest(Pfact_1, Z1);

	if (Z0 < Z1)
		return true;
	else { return false; }
}
bool KitTool::RankPfact1(PFACETTRI Pfact_0, PFACETTRI Pfact_1)
{

	if (Pfact_1->value > Pfact_0->value)
		return true;
	else { return false; }
}
//求一个面片上三个顶点的最小值
void KitTool::Thelest(PFACETTRI Pfact_0, float &Zmin)
{
	for (int nCt = 0; nCt < 3; nCt++)
	{
		int nCnt = (nCt + 1) % 3;
		if (Pfact_0->m_PVerts[nCt]->z < Pfact_0->m_PVerts[nCnt]->z)
		{
			Zmin = Pfact_0->m_PVerts[nCt]->z;
		}
	}

}
void KitTool::RankAlledge(vector<PEDGE> &vec_edg)
{
	sort(vec_edg.begin(), vec_edg.end(), RankPedge);
}

bool KitTool::RankPedge(PEDGE edge_0, PEDGE edge_1)
{
	if (edge_1->pHEdgeAdj->value > edge_0->pHEdgeAdj->value)
		return true;
	else { return false; }
}
bool KitTool::Comparepoint(PFACETTRI Pfact_0, POINT3D p0)
{
	for (int i = 0; i < 3;i++)
	{
		if (Pfact_0->m_PVerts[i]->x == p0.x&&Pfact_0->m_PVerts[i]->y == p0.y
			&&Pfact_0->m_PVerts[i]->z == p0.z)
		{
			return false;
		}
	}
	return true;
}
void KitTool::Gettriperimeter(PHEDGE ph0, double &cir)
{
	PHEDGE ph[3];
	ph[0] = ph0; ph[1] = ph0->pHEdgePre; ph[2] = ph0->pHEdgeNext;
	for (int i = 0; i < 3; i++)
	{
		int nct = (i + 1) % 3;
		POINT3D v1, v2 = PVERTtoPOINT3D(ph[i]->pVertEnd); v1 = PVERTtoPOINT3D(ph[nct]->pVertEnd);
		VECTOR3D v0 = v2 - v1;
		cir += v0.GetLength();
	}
}
POINT3D KitTool::PVERTtoPOINT3D(PVERT pv)
{
	POINT3D p;
	p.x = pv->x;
	p.y = pv->y;
	p.z = pv->z;
	return p;
}
///空间点转化为三角面片顶点
void  KitTool::POINT3DtoPVERT(POINT3D p, PVERT pv)
{
	pv->x = p.x;
	pv->y = p.y;
	pv->z = p.z;
}
double KitTool::CalGaussCurvature(PVERT pVer)
{
	double VerGausCur = 0;
	vec_PVERT vecpVer;
	bool IsOne = FindOneRing(pVer, vecpVer);       //搜寻一阶领域点
	if (IsOne == false) return false;
	double Am = AreaTriMixSum(pVer);

	VECTOR3D Vij1, Vij2;
	double dA, dAM = 0;

	int nNum = vecpVer.size();                //一阶领域点在容器中逆时针存储
	for (int i = 0; i < nNum; i++)
	{
		Vij1 = *vecpVer[i] - *pVer;
		Vij2 = *vecpVer[(i + 1) % nNum] - *pVer;
		dA = _AngleBetween3D(Vij1, Vij2);
		dAM += dA;
	}
	VerGausCur = (2 * PI - dAM) / Am;

	return VerGausCur;
}

//计算顶点的平均曲率
double KitTool::CalcuVerMeanCurvature(PVERT pVer)
{
	double VerMeanCur = 0;
	vec_PVERT     vecpVer;
	vec_PFACETTRI vecpFac;
	bool IsOne = FindOneRing_Me(pVer, vecpVer);       //搜寻一阶领域点
	if (IsOne == false) return false;
	//FindOneRFac(pVer, vecpFac);
	vecpFac = pVer->vec_OneFac;
	double Am = AreaTriMixSum(pVer);
	VECTOR3D vNor = CalcuVerNormal(pVer);

	VECTOR3D Vij, V1ji, V1jj, V2ji, V2jj;
	double dCotA1, dCotA2;
	VECTOR3D vSin1, vSin2;

	double dVN;

	int nFac = vecpFac.size();
	int nVer = vecpVer.size(); //一阶领域点在容器中逆时针存储
	int nNum = 0;
	if (nFac == nVer) nNum = nVer;//判断是否为边界点
	else              nNum = nFac;
	for (int i = 0; i<nNum; i++)
	{
		Vij = *vecpVer[i] - *pVer;
		V1ji = *pVer - *vecpVer[(i - 1 + nVer) % nVer];      //***********当点为边界点的时候该离散方法就无法使用了！！！！！
		V1jj = *vecpVer[i] - *vecpVer[(i - 1 + nVer) % nVer];
		V2ji = *pVer - *vecpVer[(i + 1) % nVer];
		V2jj = *vecpVer[i] - *vecpVer[(i + 1) % nVer];

		vSin1 = V1jj*V1ji;                         //a*b
		vSin2 = V2ji*V2jj;



		dCotA1 = (V1jj | V1ji) / vSin1.GetLength();      //cotA = (a|b)/|a*b|
		dCotA2 = (V2ji | V2jj) / vSin2.GetLength();

		dVN = Vij | vNor;
		VerMeanCur += (dCotA1 + dCotA2)*dVN;
	}

	VerMeanCur = 0.25*VerMeanCur / Am;
	return VerMeanCur;
}
//计算顶点一阶领域锐角三角形的混合面积（三角以外心为界分割，与顶点[pPoint1]相邻的部分的面积）
double KitTool::AreaTriMix(PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3)   //逆时针排列
{
	double AreaTM;
	VECTOR3D vEdge1, vEdge2, vEdge3;
	vEdge1 = *pPoint2 - *pPoint1;
	vEdge2 = *pPoint3 - *pPoint2;
	vEdge3 = *pPoint1 - *pPoint3;
	double dV1 = vEdge1 | vEdge1;                                  //a^2
	double dV3 = vEdge3 | vEdge3;
	double dCotA2, dCotA3;
	VECTOR3D vSin2, vSin3;
	vSin2 = (VECTOR3D(0, 0, 0) - vEdge1)*vEdge2;                         //a*b
	vSin3 = (VECTOR3D(0, 0, 0) - vEdge2)*vEdge3;

	dCotA2 = ((VECTOR3D(0, 0, 0) - vEdge1) | vEdge2) / vSin2.GetLength();      //cotA = (a|b)/|a*b|
	dCotA3 = ((VECTOR3D(0, 0, 0) - vEdge2) | vEdge3) / vSin3.GetLength();

	AreaTM = 0.125*(dV3*dCotA2 + dV1*dCotA3);     //Am = 0.125*(c^2*cotA2 + a^2*cotA3)

	return AreaTM;
}
//混合面积总和
double KitTool::AreaTriMixSum(PVERT pVer)
{
	double AreaTMS = 0, AreaM = 0;
	VECTOR3D vE1, vE2, vE3;
	double dE1, dE2, dE3, MaxE;               //三角形三条边的长度
	double deta;                             //deta = c^2-(a^2+b^2)
	vec_PVERT     vecpVer;
	vec_PFACETTRI vecpFac;

	bool IsOne = FindOneRing(pVer, vecpVer);       //搜寻一阶领域点
	if (IsOne == false) return false;
	FindOneRFac(pVer, vecpFac);

	int nFac = vecpFac.size();
	int nVer = vecpVer.size(); //一阶领域点在容器中逆时针存储
	int nNum = 0;
	if (nFac == nVer) nNum = nVer;//判断是否为边界点
	else              nNum = nFac;
	auto stemp = [](PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
	for (int i = 0; i < nNum; i++)
	{
		vE1 = *vecpVer[i] - *pVer;
		vE2 = *vecpVer[(i + 1) % nVer] - *vecpVer[i];
		vE3 = *pVer - *vecpVer[(i + 1) % nVer];
		dE1 = vE1.GetLength();
		dE2 = vE2.GetLength();
		dE3 = vE3.GetLength();
		MaxE = max(max(dE1, dE2), dE3);

		if (MaxE == dE1)                            //判断三角形是否为锐角三角形
		{
			deta = dE1*dE1 - dE2*dE2 - dE3*dE3;
		}
		else
		{
			if (MaxE == dE2)
			{
				deta = dE2*dE2 - dE1*dE1 - dE3*dE3;
			}
			else
			{
				deta = dE3*dE3 - dE1*dE1 - dE2*dE2;
			}
		}

		if (deta < 0)
		{
			AreaM = stemp(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
			//AreaM = AreaTriMix(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
		}
		else
		{
			if (MaxE == dE2)
			{
				AreaM = 0.5*stemp(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
				//AreaM = 0.5*AreaTri(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
			}
			else
			{
				AreaM = 0.25*stemp(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
				//AreaM = 0.25*AreaTri(pVer, vecpVer[i], vecpVer[(i + 1) % nVer]);
			}
		}
		AreaTMS = AreaTMS + AreaM;
	}
	return AreaTMS;
}
POINT3D KitTool::Center(PHEDGE trigle)
{
	POINT3D mindle;
	mindle.x = (trigle->pVertEnd->x + trigle->pHEdgeNext->pVertEnd->x + trigle->pHEdgeNext->pHEdgeNext->pVertEnd->x) / 3;
	mindle.y = (trigle->pVertEnd->y + trigle->pHEdgeNext->pVertEnd->y + trigle->pHEdgeNext->pHEdgeNext->pVertEnd->y) / 3;
	mindle.z = (trigle->pVertEnd->z + trigle->pHEdgeNext->pVertEnd->z + trigle->pHEdgeNext->pHEdgeNext->pVertEnd->z) / 3;
	return mindle;
}

double KitTool::AreaTri(PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3)
{
	VECTOR3D vEdge1, vEdge2, vArea;
	double Area;
	vEdge1 = *pPoint2 - *pPoint1;
	vEdge2 = *pPoint3 - *pPoint1;
	vArea = vEdge1*vEdge2;
	Area = 0.5*vArea.GetLength();

	return Area;
	//return 0;
}
double KitTool::AreaTri(POINT3D pPoint1, POINT3D pPoint2, POINT3D pPoint3)
{
	VECTOR3D vEdge1, vEdge2, vArea;
	double Area;
	vEdge1 = pPoint2 - pPoint1;
	vEdge2 = pPoint3 - pPoint1;
	vArea = vEdge1*vEdge2;
	Area = 0.5*vArea.GetLength();

	return Area;
	//return 0;
}
double KitTool::AreaTri(PVERT pPoint1, PVERT pPoint2, POINT3D pPoint3)
{
	VECTOR3D vEdge1, vEdge2, vArea;
	double Area;
	vEdge1 = VECTOR3D(pPoint2->x - pPoint1->x, pPoint2->y - pPoint1->y, pPoint2->z - pPoint1->z);
	vEdge2 = VECTOR3D(pPoint3.x - pPoint1->x, pPoint3.y - pPoint1->y, pPoint3.z - pPoint1->z);
	vArea = vEdge1*vEdge2;
	Area = 0.5*vArea.GetLength();

	return Area;
}
double KitTool::AreaTri(PVERT pPoint1, POINT3D pPoint2, POINT3D pPoint3)
{
	VECTOR3D vEdge1, vEdge2, vArea;
	double Area;
	vEdge1 = VECTOR3D(pPoint2.x - pPoint1->x, pPoint2.y - pPoint1->y, pPoint2.z - pPoint1->z);
	vEdge2 = VECTOR3D(pPoint3.x - pPoint1->x, pPoint3.y - pPoint1->y, pPoint3.z - pPoint1->z);
	vArea = vEdge1*vEdge2;
	Area = 0.5*vArea.GetLength();

	return Area;
}
double KitTool::GetTriArea(PFACETTRI ptri)/////////////////获得三角面片的面积
{
	vector<POINT3D> m_vecPoint;
	for (int i = 0; i < 3; i++)
	{
		m_vecPoint.push_back(*ptri->m_PVerts[i]);
	}
	VECTOR3D v1 = m_vecPoint[1] - m_vecPoint[0];
	VECTOR3D v2 = m_vecPoint[2] - m_vecPoint[0];
	VECTOR3D v3 = v1*v2;
	return 0.5*v3.GetLength();
}

PVECTOR3D KitTool::CalcuPfacNormal(PVERT pVer0, PVERT pVer1, PVERT pVer2)
{
	PVECTOR3D Normal; Normal = new VECTOR3D;
	VECTOR3D   V1, V2;
	V1 = *pVer1 - *pVer0;
	V2 = *pVer2 - *pVer1;
	V1 = V1*V2;
	V1.Normalize();
	Normal->dx = V1.dx; Normal->dy = V1.dy; Normal->dz = V1.dz;
	return Normal;
}
VECTOR3D KitTool::CalcuPfacNormal(POINT3D pVer0, POINT3D pVer1, POINT3D pVer2)
{ 
	VECTOR3D   V1, V2;
	V1 = pVer1 - pVer0;
	V2 = pVer2 - pVer1;
	V1 = V1*V2;
	V1.Normalize();
	return V1;
}
VECTOR3D KitTool::CalcuVerNormal(PVERT pVer)
{
	VECTOR3D Normal = VECTOR3D(0, 0, 0);
	VECTOR3D NorFac, V1, V2;
	vec_PVERT     vecpVer;
	vec_PFACETTRI vecpFac, vecpFac1;
	double Area, Angle, AA = 0.0;

	//FindOneRing(pVer, vecpVer);//这个函数老是出问题
	//FindOneRing_New(pVer, vecpVer);
	FindOneRing_Me(pVer, vecpVer);
	vecpFac = pVer->vec_OneFac;
	//FindOneRFac(pVer, vecpFac);
	int Nver = vecpVer.size();
	int Nfac = vecpFac.size();
	if (Nfac>Nver)
	{
		int ceshi = 0;
	}
	int nNum = 0;
	if (Nfac == Nver) nNum = Nver;
	else              nNum = Nfac;

	for (int i = 0; i < nNum; i++)
	{
		if (i<Nver)
		{
			auto stemp = [](PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
			Area = stemp(pVer, vecpVer[(i + 1) % Nver], vecpVer[i]);
			//Area = AreaTri(pVer, vecpVer[(i + 1) % Nver], vecpVer[i]);//计算相邻面片的面积
			V1 = *vecpVer[i] - *pVer;
			V2 = *vecpVer[(i + 1) % Nver] - *vecpVer[i];
			Angle = _AngleBetween3D(V1, V2);                     //计算点发散出去向量的夹角
			NorFac = *(vecpFac[i]->m_PFacetNorm);
			Normal = Normal + NorFac*Area*Angle;
			AA = AA + Area*Angle;
		}
	}

	Normal = Normal / AA;                               //法矢加权N = W(NorFac*Area*Angle)/W(Area*Angle)
	Normal = Normal.GetNormal();                          //单位化
	return Normal;
}
VECTOR3D KitTool::CalcuVerNormal(PVERT pVer, PFACETTRI pFac)
{
	vector<VECTOR3D> vec_Pnormal;
	vector<double> vec_Area;
	auto stemp = [](PVERT pPoint1, PVERT pPoint2, PVERT pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
	double Sumarea = stemp(pFac->m_PVerts[0], pFac->m_PVerts[1], pFac->m_PVerts[2]);
	//double Sumarea = AreaTri(pFac->m_PVerts[0], pFac->m_PVerts[1], *pFac->m_PVerts[2]);
	for (int i = 0; i < 3; i++)
	{
		int nct = (i + 1) % 3;
		VECTOR3D Tempnormal = CalcuVerNormal(pFac->m_PVerts[i]);
		vec_Pnormal.push_back(Tempnormal);

		double Temparea = stemp(pFac->m_PVerts[i], pFac->m_PVerts[nct], pVer);
		//double Temparea = AreaTri(pFac->m_PVerts[i], pFac->m_PVerts[nct], pVer);
		Temparea = Temparea / Sumarea;
		vec_Area.push_back(Temparea);
	}
	double xdiv = 0, ydiv = 0, zdiv = 0;
	for (int i = 0; i < 3; i++)
	{
		int nct = (i + 2) % 3;
		xdiv += vec_Area[i] * vec_Pnormal[nct].dx;
		ydiv += vec_Area[i] * vec_Pnormal[nct].dy;
		zdiv += vec_Area[i] * vec_Pnormal[nct].dz;
	}
	VECTOR3D Certer_Normal = VECTOR3D(xdiv, ydiv, zdiv);//估算三角形重心的法矢
	Certer_Normal.Normalize();
	return Certer_Normal;
}
double KitTool::CalcuPointCurve(PVERT A)
{
	double meanc, gassc, cdrgee;
	meanc = CalcuVerMeanCurvature(A);
	gassc = CalGaussCurvature(A);
	cdrgee = 4 * meanc*meanc - 2 * gassc;
	return cdrgee;
}

BOOL KitTool::FindOneRing(PVERT& pVer, vec_PVERT& vecpVer)
{
	PVERT pVerAdj;
	PHEDGE pHEO;
	it_vec_PVERT it_vecPV;
	it_vecPV = vecpVer.begin();
	pHEO = pVer->pHEdgeOut;
	vecpVer.clear();
	do
	{
		pVerAdj = pHEO->pVertEnd;
		vecpVer.push_back(pVerAdj);             //点发散半边的终点为其一阶领域点

		if (pHEO->pHEdgePre->pHEdgePair != NULL)   //假如没碰到边界
		{
			pHEO = pHEO->pHEdgePre->pHEdgePair; //逆时针寻找，点在点表中逆时针排列
		}
		else                                    //假如碰到边界
		{
			pVerAdj = pHEO->pHEdgeNext->pVertEnd;
			vecpVer.push_back(pVerAdj);             //将逆时针方向边界上的点放入一阶领域
			if (pVer->pHEdgeOut->pHEdgePair != NULL) //假如点的发散半边不是最顺时针的半边
			{
				pHEO = pVer->pHEdgeOut->pHEdgePair->pHEdgeNext;//将点下个判断半边转向起始半边的顺时针侧
				do
				{
					pVerAdj = pHEO->pVertEnd;
					it_vecPV = vecpVer.begin();
					vecpVer.insert(it_vecPV, pVerAdj);    //在容器的开头插入点，使一阶领域点还是逆时针排列
					if (pHEO->pHEdgePair != NULL)
					{
						pHEO = pHEO->pHEdgePair->pHEdgeNext;
					}
					else
					{
						pHEO = pVer->pHEdgeOut;
						break;
					}

				} while (pHEO != pVer->pHEdgeOut);
			}
			else                       //假如点的发散半边是最顺时针的半边
			{
				pHEO = pVer->pHEdgeOut;
				break;
			}

		}
	} while (pHEO != pVer->pHEdgeOut);          //向外发散的半边不等于起始的半边，继续循环

	if (vecpVer.size() != 0)                     //容器中有点，搜寻成功
	{
		return TRUE;
	}
	return FALSE;
}

BOOL KitTool::FindOneRing_New(PVERT& pVer, vec_PVERT& vecpVer)
{
	set<PVERT>  setPVert;
	for (int i = 0; i < pVer->vec_OneFac.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			setPVert.insert(pVer->vec_OneFac[i]->m_PVerts[j]);
		}
	}
	setPVert.erase(pVer);
	vecpVer.insert(vecpVer.end(), setPVert.begin(), setPVert.end());
	if (vecpVer.size() != 0)                     //容器中有点，搜寻成功
	{
		return true;
	}
	return false;
}
BOOL KitTool::FindOneRing_Me(PVERT& pVer, vec_PVERT& vecpVer)
{
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;

	for (int i = 0; i < pVer->vec_OneFac.size();i++)
	{
		PVERT Pvert[3]; 
		Pvert[0] = pVer->vec_OneFac[i]->m_PVerts[0];
		Pvert[1] = pVer->vec_OneFac[i]->m_PVerts[1];
		Pvert[2] = pVer->vec_OneFac[i]->m_PVerts[2];
		for (int j = 0; j < 3;j++)
		{
			iter = map_Vert.find(*Pvert[j]);
			if (iter == map_Vert.end() && Pvert[j]!=pVer)
			{
				map_Vert.insert(pair<VERT, int>(*Pvert[j], vecpVer.size()));
				vecpVer.push_back(Pvert[j]);
			}
		}
	}
	if (vecpVer.size() != 0)                     //容器中有点，搜寻成功
	{
		return TRUE;
	}
	return FALSE;
}

void KitTool::FindedgeOneringvertex_Me(PHEDGE Ph0, vec_PVERT& vecpVer)
{
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	PVERT vec_Pvert[2];
	vec_Pvert[0] = Ph0->pVertEnd; vec_Pvert[1] = Ph0->pHEdgePre->pVertEnd;
	for (int i = 0; i < 2;i++)
	{
		for (int m = 0; m < vec_Pvert[i]->vec_OneFac.size(); m++)
		{
			PVERT Pvert[3];
			Pvert[0] = vec_Pvert[i]->vec_OneFac[m]->m_PVerts[0];
			Pvert[1] = vec_Pvert[i]->vec_OneFac[m]->m_PVerts[1];
			Pvert[2] = vec_Pvert[i]->vec_OneFac[m]->m_PVerts[2];
			for (int n = 0; n < 3; n++)
			{
				iter = map_Vert.find(*Pvert[n]);
				if (iter == map_Vert.end())
				{
					map_Vert.insert(pair<VERT, int>(*Pvert[n], vecpVer.size()));
					vecpVer.push_back(Pvert[n]);
				}
			}
		}
	}
}
BOOL KitTool::FindOnePH(PVERT A, vec_PHEDGE& vecpH)
{
	vecpH.clear();
	PHEDGE temp;
	temp = A->pHEdgeOut;//是以顶点A为起点的半边吗？
	vecpH.push_back(temp);
	int k = 0;
	while (1)
	{
		if (temp->pHEdgePre->pHEdgePair!=NULL)
		{
			temp = temp->pHEdgePre->pHEdgePair;
			if (temp == A->pHEdgeOut)
			{
				break;
			}
			vecpH.push_back(temp);
		}
		
	}
	return TRUE;
}



BOOL KitTool::FindOneRFac(PVERT pVer, vec_PFACETTRI& vecpFac)
{
	vecpFac.clear();
	PFACETTRI pFacAdj;
	PHEDGE    pHEO;
	it_vec_PFACETTRI it_vecPF;
	it_vecPF = vecpFac.begin();
	pHEO = pVer->pHEdgeOut;

	do
	{
		pFacAdj = (PFACETTRI)pHEO->pFacetAdj;
		vecpFac.push_back(pFacAdj);             //点发散半边的相邻面为其一阶领域面

		if (pHEO->pHEdgePre->pHEdgePair != NULL)   //假如没碰到边界
		{
			pHEO = pHEO->pHEdgePre->pHEdgePair; //逆时针寻找，点在点表中逆时针排列
		}
		else                                    //假如碰到边界
		{
			if (pVer->pHEdgeOut->pHEdgePair != NULL) //假如点的发散半边不是最顺时针的半边
			{
				pHEO = pVer->pHEdgeOut->pHEdgePair->pHEdgeNext;//将面下个判断半边转向起始半边的顺时针侧
				do
				{
					pFacAdj = (PFACETTRI)pHEO->pFacetAdj;
					it_vecPF = vecpFac.begin();
					vecpFac.insert(it_vecPF, pFacAdj);    //在容器的开头插入面，使一阶领域面还是逆时针排列
					if (pHEO->pHEdgePair != NULL)
					{
						pHEO = pHEO->pHEdgePair->pHEdgeNext;
					}
					else
					{
						pHEO = pVer->pHEdgeOut;
					}

				} while (pHEO != pVer->pHEdgeOut);
			}
			else
			{
				pHEO = pVer->pHEdgeOut;
			}

		}

	} while (pHEO != pVer->pHEdgeOut);          //向外发散的半边不等于起始的半边，继续循环

	if (vecpFac.size() != 0)                     //容器中有面，搜寻成功
	{
		return TRUE;
	}
	return FALSE;
}

BOOL KitTool::FindPOneRFAC(PFACETTRI pFac, vec_PFACETTRI& vecpFacP)
{
	pFac->becut = 1; vec_PFACETTRI vecSum;
	vecSum.push_back(pFac);
	for (int i = 0; i<3; i++)
	{
		vec_PFACETTRI vectemp;
		if (pFac->m_PVerts[i]->pHEdgeOut != NULL)
		{
			vectemp = pFac->m_PVerts[i]->vec_OneFac;
			for (int j = 0; j < vectemp.size(); j++)
			{
				if (vectemp[j]->becut == 0)
				{
					vecpFacP.push_back(vectemp[j]);
					vectemp[j]->becut = 1;
				}
				vecSum.push_back(vectemp[j]);
			}
		}
	}

	for (int i = 0; i<vecSum.size(); i++)
	{
		vecSum[i]->becut = 0;
	}
	return TRUE;
}
BOOL KitTool::FindPOneRFAC_NEW(PFACETTRI pFac, vec_PFACETTRI& vecpFacP)
{
	vecpFacP.clear();
	PFACETTRI B;
	if (pFac->pHEdge->pHEdgePair != NULL)
	{
		B = (PFACETTRI)(pFac->pHEdge->pHEdgePair->pFacetAdj);
		vecpFacP.push_back(B);
	}
	if (pFac->pHEdge->pHEdgeNext->pHEdgePair != NULL)
	{
		B = (PFACETTRI)(pFac->pHEdge->pHEdgeNext->pHEdgePair->pFacetAdj);
		vecpFacP.push_back(B);
	}
	if (pFac->pHEdge->pHEdgePre->pHEdgePair != NULL)
	{
		B = (PFACETTRI)(pFac->pHEdge->pHEdgePre->pHEdgePair->pFacetAdj);
		vecpFacP.push_back(B);
	}
	return TRUE;
}

void KitTool::FindPtwoRFAC(PFACETTRI pFac, vec_PFACETTRI& vecpFacP)
{
	vec_PFACETTRI vectempFac;
	FindPOneRFAC(pFac, vectempFac);
	for (int i = 0; i < vectempFac.size(); ++i)
	{
		vectempFac[i]->becut = true;
		vecpFacP.push_back(vectempFac[i]);
	}
	for (int i = 0; i < vectempFac.size(); ++i)
	{
		vec_PFACETTRI vectempFac2;
		FindPOneRFAC(vectempFac[i], vectempFac2);
		for (int k = 0; k < vectempFac2.size(); ++k)
		{
			if (vectempFac2[k]->becut == false)
			{
				vecpFacP.push_back(vectempFac2[k]);
			}
		}
	}
	for (int i = 0; i < vecpFacP.size(); ++i)
	{
		vecpFacP[i]->becut = false;
	}
}
BOOL KitTool::FindPOneRVetex(PFACETTRI pFac, vec_PVERT& vecpVer)
{
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	vec_PFACETTRI vec1; vec1.clear();
	FindPOneRFAC(pFac, vec1);
	for (int i = 0; i < vec1.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			iter = map_Vert.find(*vec1[i]->m_PVerts[j]);
			if (iter == map_Vert.end())
			{
				map_Vert.insert(pair<VERT, int>(*vec1[i]->m_PVerts[j], vecpVer.size()));
				vecpVer.push_back(vec1[i]->m_PVerts[j]);
			}
		}
	}
	return TRUE;
}
void KitTool::FindedgeOnering(PHEDGE Ph0, vector<PFACETTRI> &vecper)
{
	vec_PFACETTRI vec1, vec2;
	vec1 = Ph0->pVertEnd->vec_OneFac;
	vec2 = Ph0->pHEdgePre->pVertEnd->vec_OneFac;
	map<PFACETTRI, int> map_Pfact; map_Pfact.clear();
	map<PFACETTRI, int>::iterator iter_Pfact;
	for (int i = 0; i < vec1.size(); i++)
	{
		vecper.push_back(vec1[i]);
		map_Pfact.insert(pair<PFACETTRI, int>(vec1[i], vecper.size()));
	}
	for (int i = 0; i < vec2.size(); i++)
	{
		iter_Pfact = map_Pfact.find(vec2[i]);
		if (iter_Pfact == map_Pfact.end() )//不存在该面片
		{
			vecper.push_back(vec2[i]);
		}
	}
}

double KitTool::dis(POINT3D A, POINT3D B)
{
	double L = 0;
	L = sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y) + (A.z - B.z)*(A.z - B.z));
	return L;
}
BOOL KitTool::FinOneFac_Less(PFACETTRI pfc0)
{
	vector<PFACETTRI> vec1, vec2;//一阶领域面片
	FindPOneRFAC(pfc0, vec1);//所有一阶领域面片
	FindPOneRFAC_NEW(pfc0, vec2);
	for (int m = 0; m < vec1.size(); m++)
	{
		int nt = 0;
		for (int n = 0; n < vec2.size(); n++)
		{
			if (vec1[m] != vec2[n])
			{
				nt++;
			}
		}
		if (nt == vec2.size())
		{
			pfc0->vec_FOneFac.push_back(vec1[m]);
		}
	}
	return TRUE;
}
//计算网格点的二阶领域面
BOOL KitTool::FindTwoRing(PVERT pVer, vec_PFACETTRI& vecpFacT)
{
	vec_PVERT  vecpVer;
	PFACETTRI pFacAdj;
	PVERT      pVerAdjOne, pVerAdj;
	set<PFACETTRI> setpFacT;
	pair<set<PFACETTRI>::iterator, bool> pair_insertResult;
	it_vec_PFACETTRI it_vecPV;
	it_vecPV = vecpFacT.begin();

	BOOL IsOne = FindOneRing(pVer, vecpVer);       //搜寻一阶领域点
	if (IsOne == FALSE)
	{
		return FALSE;
	}

	int nNum = vecpVer.size();
	for (int i = 0; i < nNum; i++)
	{
		pVerAdjOne = vecpVer[i];
		PHEDGE pHEO;
		pHEO = pVerAdjOne->pHEdgeOut;
		do
		{
			pFacAdj = (PFACETTRI)pHEO->pFacetAdj;
			if (pHEO->pHEdgePair == NULL)
			{
				//pVerAdjOne->bStatus= TRUE;
				pFacAdj->bStatus = TRUE;
				break;
			}
			pHEO = pHEO->pHEdgePair->pHEdgeNext;   //顺时针搜索

		} while (pHEO != pVerAdjOne->pHEdgeOut);


		if (pFacAdj->bStatus == TRUE)
		{
			pair_insertResult = setpFacT.insert(pFacAdj);
			if (pair_insertResult.second)
			{
				vecpFacT.push_back(pFacAdj);
			}
		}
		else
		{
			do
			{
				pVerAdj = pHEO->pVertEnd;
				pFacAdj = (PFACETTRI)pHEO->pFacetAdj;
				if (pFacAdj != pVer->pHEdgeOut->pFacetAdj)
				{
					pair_insertResult = setpFacT.insert(pFacAdj);
					if (pair_insertResult.second)
					{
						vecpFacT.push_back(pFacAdj);
					}

				}
				pHEO = pHEO->pHEdgePre->pHEdgePair; //逆时针寻找，点在点表中逆时针排列

			} while (pHEO != pVerAdjOne->pHEdgeOut);          //向外发散的半边不等于起始的半边，继续循环
		}
	}

	if (vecpFacT.size() != 0)                     //容器中有点，搜寻成功
	{
		return TRUE;
	}
	return FALSE;
}
BOOL KitTool::FindTwoRing_new(PVERT pVer, vec_PVERT& vecpVerT)
{
	vecpVerT.clear();
	vec_PVERT  vecpVer;
	FindOneRing(pVer, vecpVer);
	set<PVERT> thetemp;
	set<PVERT>::iterator tehIT;
	for (int i = 0; i < vecpVer.size(); i++)
	{
		vec_PVERT vec_temp;
		FindOneRing(vecpVer[i], vec_temp);
		for (int j = 0; j < vec_temp.size(); j++)
		{
			thetemp.insert(vec_temp[j]);
		}
	}
	for (tehIT = thetemp.begin(); tehIT != thetemp.end(); tehIT++)
	{
		PVERT tempV = *tehIT;
		vecpVerT.push_back(tempV);
	}
	return vecpVerT.empty();
}
BOOL KitTool::FindFourRing(PVERT pVer, vec_PVERT& vecpVerT)
{
	vecpVerT.clear();
	vec_PVERT  vecpVer;
	FindTwoRing_new(pVer, vecpVer);
	set<PVERT> thetemp;
	set<PVERT>::iterator tehIT;
	for (int i = 0; i < vecpVer.size(); i++)
	{
		vec_PVERT vec_temp;
		FindTwoRing_new(vecpVer[i], vec_temp);
		for (int j = 0; j < vec_temp.size(); j++)
		{
			thetemp.insert(vec_temp[j]);
		}
	}
	for (tehIT = thetemp.begin(); tehIT != thetemp.end(); tehIT++)
	{
		PVERT tempV = *tehIT;
		vecpVerT.push_back(tempV);
	}
	return vecpVerT.empty();
}
double KitTool::CalcuPOINTV(PVERT A)
{
	vec_PFACETTRI vecpFacT;
	vec_VECTOR3D vecVTR;
	VECTOR3D N;
	N = A->VertexNormal;
	FindOneRFac(A, vecpFacT);
	//FindTwoRing(A, vecpFacT);
	for (int i = 0; i < vecpFacT.size(); i++)
	{
		VECTOR3D P;
		P.dx = Center(vecpFacT[i]->pHEdge).x - A->x;
		P.dy = Center(vecpFacT[i]->pHEdge).y - A->y;
		P.dz = Center(vecpFacT[i]->pHEdge).z - A->z;
		P.Normalize();
		vecVTR.push_back(P);
	}
	double NV = 0;
	for (int i = 0; i < vecVTR.size(); i++)
	{
		if ((N | vecVTR[i]) > NV)
		{
			NV = N | vecVTR[i];
		}
	}
	return NV;
}
void KitTool::closedBoundary(vec_PHEDGE m_vecPHEdge, PPOLYPOLYGON polypoly)
{
	//PPOLYPOLYGON  polypoly;
	//polypoly = new POLYPOLYGON;
	PPOLYGON poly;
	poly = new POLYGON;
	if (!m_vecPHEdge.empty())
	{

		POINT3D p = PVERTtoPOINT3D(m_vecPHEdge[0]->pVertEnd);
		POINT3D pp = PVERTtoPOINT3D(m_vecPHEdge[0]->pHEdgePre->pVertEnd);
		poly->m_vecPnts.push_back(pp);
		poly->m_vecPnts.push_back(p);
		m_vecPHEdge.erase(m_vecPHEdge.begin());
		do
		{
			bool ifFind = false;
			for (int i = 0; i < m_vecPHEdge.size(); i++)
			{
				POINT3D p1 = PVERTtoPOINT3D(m_vecPHEdge[i]->pVertEnd);
				POINT3D p2 = PVERTtoPOINT3D(m_vecPHEdge[i]->pHEdgePre->pVertEnd);
				if (p == p1 || p == p2)
				{
					if (p == p1)
					{
						poly->m_vecPnts.push_back(p2);
						p = p2;
					}
					else
					{
						poly->m_vecPnts.push_back(p1);
						p = p1;
					}
					it_vec_PHEDGE it;
					it = m_vecPHEdge.begin() + i;
					m_vecPHEdge.erase(it);
					ifFind = true;
					break;
				}
			}
			if (!m_vecPHEdge.empty() && ifFind == false)
			{
				poly->m_vecPnts.push_back(p);
				polypoly->m_vecPPolygons.push_back(poly);
				poly = new POLYGON;
				p = PVERTtoPOINT3D(m_vecPHEdge[0]->pVertEnd);
				pp = PVERTtoPOINT3D(m_vecPHEdge[0]->pHEdgePre->pVertEnd);
				poly->m_vecPnts.push_back(pp);
				poly->m_vecPnts.push_back(p);
				m_vecPHEdge.erase(m_vecPHEdge.begin());
			}
		} while (!m_vecPHEdge.empty());
		poly->m_vecPnts.pop_back();
		reverse(poly->m_vecPnts.begin(), poly->m_vecPnts.end());//把点转换过来的函数
		polypoly->m_vecPPolygons.push_back(poly);
		//去除冗余点
		vector<vector<POINT3D>> m_vert;
		for (int i = 0; i < polypoly->m_vecPPolygons.size();i++)
		{
			vector<POINT3D> vec_pert;
			PPOLYGON ppoly = polypoly->m_vecPPolygons[i];
			for (int j = 0; j < ppoly->m_vecPnts.size(); j++)
			{
				vec_pert.push_back(ppoly->m_vecPnts[j]);
			}
			m_vert.push_back(vec_pert);
		}
		map<VERT, int>map_Vert;
		map<VERT, int>::iterator iter;
		for (int i = 0; i < m_vert.size(); i++)
		{
			polypoly->m_vecPPolygons[i]->m_vecPnts.clear();
			for (int j = 0; j < m_vert[i].size();j++)
			{
				VERT p0; p0.x = m_vert[i][j].x; p0.y = m_vert[i][j].y; p0.z = m_vert[i][j].z;
				iter = map_Vert.find(p0);
				if (iter == map_Vert.end())//
				{
					map_Vert.insert(pair<VERT, int>(p0, polypoly->m_vecPPolygons[i]->m_vecPnts.size()));
					polypoly->m_vecPPolygons[i]->m_vecPnts.push_back(p0);
				}
			}
		}
	}
}
void KitTool::create_cone_new(vector<VECTOR3D>& m_vecVECTOR3D, VECTOR3D LineNorZ, VECTOR3D LineNor, POINT3D orig, double angA, int numB)//建立扇形可视锥
{
	m_vecVECTOR3D.clear();
	VECTOR3D tempLine;
	MATRIX3D matriZ, currentB, currentM;
	create_current_Frenet(currentM, LineNorZ, orig);
	create_BACK_Frenet(currentB, LineNorZ, orig);
	create_current_matriZ(matriZ, angA);
	double mm[4];
	mm[0] = LineNor.dx; mm[1] = LineNor.dy; mm[2] = LineNor.dz;
	mm[3] = 1;
	LineNor.dx = (currentM.A[0][0] * mm[0] + currentM.A[1][0] * mm[1] + currentM.A[2][0] * mm[2] + currentM.A[3][0] * mm[3]);
	LineNor.dy = (currentM.A[0][1] * mm[0] + currentM.A[1][1] * mm[1] + currentM.A[2][1] * mm[2] + currentM.A[3][1] * mm[3]);
	LineNor.dz = (currentM.A[0][2] * mm[0] + currentM.A[1][2] * mm[1] + currentM.A[2][2] * mm[2] + currentM.A[3][2] * mm[3]);
	mm[0] = LineNor.dx; mm[1] = LineNor.dy; mm[2] = LineNor.dz;
	mm[3] = 1;
	tempLine.dx = (matriZ.A[0][0] * mm[0] + matriZ.A[1][0] * mm[1] + matriZ.A[2][0] * mm[2] + matriZ.A[3][0] * mm[3]);
	tempLine.dy = (matriZ.A[0][1] * mm[0] + matriZ.A[1][1] * mm[1] + matriZ.A[2][1] * mm[2] + matriZ.A[3][1] * mm[3]);
	tempLine.dz = (matriZ.A[0][2] * mm[0] + matriZ.A[1][2] * mm[1] + matriZ.A[2][2] * mm[2] + matriZ.A[3][2] * mm[3]);
	tempLine.Normalize();
	create_current_matriZ(matriZ, 5);
	for (int i = 0; i < numB; i++)
	{
		double mm[4];
		mm[0] = tempLine.dx; mm[1] = tempLine.dy; mm[2] = tempLine.dz;
		mm[3] = 1;
		tempLine.dx = (matriZ.A[0][0] * mm[0] + matriZ.A[1][0] * mm[1] + matriZ.A[2][0] * mm[2] + matriZ.A[3][0] * mm[3]);
		tempLine.dy = (matriZ.A[0][1] * mm[0] + matriZ.A[1][1] * mm[1] + matriZ.A[2][1] * mm[2] + matriZ.A[3][1] * mm[3]);
		tempLine.dz = (matriZ.A[0][2] * mm[0] + matriZ.A[1][2] * mm[1] + matriZ.A[2][2] * mm[2] + matriZ.A[3][2] * mm[3]);
		tempLine.Normalize();
		m_vecVECTOR3D.push_back(tempLine);
	}
	for (int i = 0; i < m_vecVECTOR3D.size(); i++)
	{
		double mm[4];
		mm[0] = m_vecVECTOR3D[i].dx; mm[1] = m_vecVECTOR3D[i].dy; mm[2] = m_vecVECTOR3D[i].dz;
		mm[3] = 1;
		m_vecVECTOR3D[i].dx = (currentB.A[0][0] * mm[0] + currentB.A[1][0] * mm[1] + currentB.A[2][0] * mm[2] + currentB.A[3][0] * mm[3]);
		m_vecVECTOR3D[i].dy = (currentB.A[0][1] * mm[0] + currentB.A[1][1] * mm[1] + currentB.A[2][1] * mm[2] + currentB.A[3][1] * mm[3]);
		m_vecVECTOR3D[i].dz = (currentB.A[0][2] * mm[0] + currentB.A[1][2] * mm[1] + currentB.A[2][2] * mm[2] + currentB.A[3][2] * mm[3]);
	}
}
void KitTool::create_current_Frenet(MATRIX3D& currentM, VECTOR3D LineNor, POINT3D Center)
{
	double cosT, cosU, cosV;
	cosT = cosU = cosV = 0;
	VECTOR3D X, Y, Z;
	double A, B, C, D;
	A = B = C = D = 0;
	X.dx = 1; X.dy = 0; X.dz = 0; Y.dx = 0; Y.dy = 1; Y.dz = 0; Z.dx = 0; Z.dy = 0; Z.dz = 1;
	cosT = (LineNor | X);
	cosU = (LineNor | Y);
	cosV = (LineNor | Z);
	D = sqrt(cosT*cosT + cosU*cosU);
	A = (-Center.x*cosT*cosV / D) - (Center.y*cosU*cosV / D) + Center.z*D;
	B = (Center.x*cosU / D) - (Center.y*cosT / D);
	C = -Center.x*cosT - Center.y*cosU - Center.z*cosV;
	currentM.A[0][0] = cosT*cosV / D; currentM.A[0][1] = (-cosU / D); currentM.A[0][2] = cosT; currentM.A[0][3] = 0;
	currentM.A[1][0] = cosU*cosV / D; currentM.A[1][1] = cosT / D; currentM.A[1][2] = cosU; currentM.A[1][3] = 0;
	currentM.A[2][0] = (-D); currentM.A[2][1] = 0; currentM.A[2][2] = cosV; currentM.A[2][3] = 0;
	currentM.A[3][0] = A; currentM.A[3][1] = B; currentM.A[3][2] = C; currentM.A[3][3] = 1;
}
void KitTool::create_BACK_Frenet(MATRIX3D& currentB, VECTOR3D LineNor, POINT3D Center)
{
	double cosT, cosU, cosV;
	cosT = cosU = cosV = 0;
	VECTOR3D X, Y, Z;
	X.dx = 1; X.dy = 0; X.dz = 0; Y.dx = 0; Y.dy = 1; Y.dz = 0; Z.dx = 0; Z.dy = 0; Z.dz = 1;
	cosT = (LineNor | X);
	cosU = (LineNor | Y);
	cosV = (LineNor | Z);
	double ABC = sqrt(cosT*cosT + cosU*cosU);
	currentB.A[0][0] = (cosT*cosV) / ABC; currentB.A[0][1] = (cosU*cosV) / ABC; currentB.A[0][2] = -ABC; currentB.A[0][3] = 0;
	currentB.A[1][0] = (-cosU) / ABC; currentB.A[1][1] = cosT / ABC; currentB.A[1][2] = 0; currentB.A[1][3] = 0;
	currentB.A[2][0] = cosT; currentB.A[2][1] = cosU; currentB.A[2][2] = cosV; currentB.A[2][3] = 0;
	currentB.A[3][0] = Center.x; currentB.A[3][1] = Center.y; currentB.A[3][2] = Center.z; currentB.A[3][3] = 1;
}
///////////////////////////////建立绕X轴旋转的坐标
void KitTool::create_current_matriX(MATRIX3D& matriX, double a)
{
	/*MATRIX3D matriX;
	double m[4];*/
	a = (a / 180)*3.1415926;
	matriX.A[0][0] = cos(a); matriX.A[0][1] = 0; matriX.A[0][2] = (-sin(a)); matriX.A[0][3] = 0;
	matriX.A[1][0] = 0; matriX.A[1][1] = 1; matriX.A[1][2] = 0; matriX.A[1][3] = 0;
	matriX.A[2][0] = sin(a); matriX.A[2][1] = 0; matriX.A[2][2] = cos(a); matriX.A[2][3] = 0;
	matriX.A[3][0] = 0; matriX.A[3][1] = 0; matriX.A[3][2] = 0; matriX.A[3][3] = 1;
	//m[0]=LineNor.dx;m[1]=LineNor.dy;m[2]=LineNor.dz;
	//m[3]=1;
	//LineNor.dx=(matriX.A[0][0]*m[0]+matriX.A[1][0]*m[1]+matriX.A[2][0]*m[2]+matriX.A[3][0]*m[3]);
	//LineNor.dy=(matriX.A[0][1]*m[0]+matriX.A[1][1]*m[1]+matriX.A[2][1]*m[2]+matriX.A[3][1]*m[3]);
	//LineNor.dz=(matriX.A[0][2]*m[0]+matriX.A[1][2]*m[1]+matriX.A[2][2]*m[2]+matriX.A[3][2]*m[3]);
}
void KitTool::create_current_matriZ(MATRIX3D& matriZ, double a)
{
	a = (a / 180)*3.1415926;
	matriZ.A[0][0] = cos(a); matriZ.A[0][1] = sin(a); matriZ.A[0][2] = 0; matriZ.A[0][3] = 0;
	matriZ.A[1][0] = (-sin(a)); matriZ.A[1][1] = cos(a); matriZ.A[1][2] = 0; matriZ.A[1][3] = 0;
	matriZ.A[2][0] = 0; matriZ.A[2][1] = 0; matriZ.A[2][2] = 1; matriZ.A[2][3] = 0;
	matriZ.A[3][0] = 0; matriZ.A[3][1] = 0; matriZ.A[3][2] = 0; matriZ.A[3][3] = 1;
}
void KitTool::cone_delel(vector<double>& vec_dis)
{
	vector<double>::iterator ite;
	sort(vec_dis.begin(), vec_dis.end());
	//double cent_dis=0;//中位数
	//if (vec_dis.size()%2==0)
	//{
	//	cent_dis=vec_dis[vec_dis.size()*0.5-1]+vec_dis[vec_dis.size()*0.5];
	//	cent_dis=cent_dis*0.5;
	//}
	//else
	//{
	//	cent_dis=vec_dis[(vec_dis.size()-1)*0.5];
	//}
	//double nomal_dis=0;//中位数标准差
	//for (int i=0;i<vec_dis.size();i++)
	//{
	//	nomal_dis=nomal_dis+(vec_dis[i]-cent_dis)*(vec_dis[i]-cent_dis);
	//}
	//nomal_dis=sqrt(nomal_dis);
	//for (int i=0;i<vec_dis.size();i++)
	//{
	//	if ((vec_dis[i]-cent_dis)>nomal_dis&&(vec_dis[i]-cent_dis)<(-nomal_dis))
	//	{
	//		ite=vec_dis.begin()+i;
	//		vec_dis.erase(ite);
	//		i--;
	//	}
	//}
	vector<double> temp;
	for (int i = 0; i < vec_dis.size(); i++)
	{
		if (vec_dis[i] < 2 * vec_dis[0])
		{
			temp.push_back(vec_dis[i]);
		}
		else
		{
			break;
		}
	}
	vec_dis.clear();
	vec_dis = temp;
}
BOOL KitTool::IntersectTriangle(POINT3D orig, VECTOR3D dir, POINT3D v0, POINT3D v1, POINT3D v2)
{
	double t, u, v;
	VECTOR3D E1 = v1 - v0;
	VECTOR3D E2 = v2 - v0;
	VECTOR3D P = dir*E2;
	double det = E1 | P;
	VECTOR3D T;
	VECTOR3D QJ;
	if (det > 0)
	{
		T = orig - v0;
	}
	else
	{
		T = v0 - orig;
		det = -det;
	}

	if (det < 0)
		return false;
	u = T | P;
	if (u < 0 || u > det)
		return false;
	QJ = (T*E1);
	v = (dir | QJ);
	if (v < 0 || u + v > det)
		return false;
	t = (E2 | QJ);
	if (t < 0)
	{
		return false;
	}
	double fInvDet = 1 / det;
	t = fInvDet;
	u = fInvDet;
	v = fInvDet;
	return TRUE;
}
//整个模型上,两个点之间的最短路径
void KitTool::Dijkstra_Point_ST_EN_new(PVERT ST, PVERT EN, map<PVERT, PVERT>& Path, vector<PVERT>& BLOOP, vec_PVERT& m_vecPVert)
{
	queue <PVERT> Q;
	map<PVERT, double> Dist;
	vector<PVERT> Pnear;
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		m_vecPVert[i]->bStatus = 0;
		Dist.insert(pair<PVERT, double>(m_vecPVert[i], 999));
	}
	Dist[ST] = 0;
	Q.push(ST);
	Path[ST] = ST->pHEdgeOut->pVertEnd;
	ST->bStatus = 1;
	while (!Q.empty())
	{
		vec_PVERT vecpVer;
		FindOneRing(Q.front(), vecpVer);
		for (int i = 0; i < vecpVer.size(); i++)
		{
			Pnear.push_back(vecpVer[i]);
			double k = 1;//
			if ((Dist[Q.front()] + k) < Dist[vecpVer[i]])
			{
				Path[vecpVer[i]] = Q.front();
				Dist[vecpVer[i]] = Dist[Q.front()] + k;
			}
		}
		while (!Pnear.empty())
		{
			if (Pnear.size() > 1)
			{
				for (int i = 0; i < Pnear.size() - 1; i++)
				{
					if (Dist[Pnear[i]] < Dist[Pnear[i + 1]])
					{
						swap(Pnear[i], Pnear[i + 1]);
					}
				}
				if (Pnear.back()->bStatus == 0)
				{
					Pnear.back()->bStatus = 1;
					Q.push(Pnear.back());
				}
				Pnear.pop_back();
			}
			if (Pnear.size() == 1)
			{
				if (Pnear.back()->bStatus == 0)
				{
					Pnear.back()->bStatus = 1;
					Q.push(Pnear.back());
				}
				Pnear.pop_back();
			}
		}
		Q.pop();
		if (Q.size() > 0)
		{
			if (Q.front() == EN)
			{
				break;
			}
		}
	}
}
void KitTool::ClearTheList(STLModell *pMeshTriDate)
{
	for (int ncnt = 0; ncnt < pMeshTriDate->m_vecPEdge.size(); ncnt++)//清除边表
	{
		delete pMeshTriDate->m_vecPEdge[ncnt];
	}
	pMeshTriDate->m_vecPEdge.clear();

	for (int ncnt = 0; ncnt < pMeshTriDate->m_vecPHEdge.size(); ncnt++)//清除半边表
	{
		delete pMeshTriDate->m_vecPHEdge[ncnt];
	}
	pMeshTriDate->m_vecPHEdge.clear();

	for (int ncnt = 0; ncnt < pMeshTriDate->m_vecPFacetTri.size(); ncnt++)//清除面片表
	{
		delete pMeshTriDate->m_vecPFacetTri[ncnt];
	}
	pMeshTriDate->m_vecPFacetTri.clear();

	for (int ncnt = 0; ncnt < pMeshTriDate->m_vecPVert.size(); ncnt++)//清除点表
	{
		if (pMeshTriDate->m_vecPVert[ncnt]->vec_OneFac.size() != 0)
		{
			delete pMeshTriDate->m_vecPVert[ncnt];
		}
	}
	pMeshTriDate->m_vecPVert.clear();

	for (int ncnt = 0; ncnt < pMeshTriDate->m_vecFacetNorm.size(); ncnt++)//清除法矢表
	{
		delete pMeshTriDate->m_vecFacetNorm[ncnt];
	}
	pMeshTriDate->m_vecFacetNorm.clear();
}
void KitTool::ClearTheList(Blockpath &OneBlock)
{
	int	nCnt;
	int nCntTmp;
	nCnt = OneBlock.m_vecPEdge.size();
	for (nCntTmp = 0; nCntTmp < nCnt; nCntTmp++)
	{
		delete OneBlock.m_vecPEdge[nCntTmp];
	}
	OneBlock.m_vecPEdge.clear();

	nCnt = OneBlock.m_vecPHEdge.size();
	for (nCntTmp = 0; nCntTmp < nCnt; nCntTmp++)
	{
		delete OneBlock.m_vecPHEdge[nCntTmp];
	}
	OneBlock.m_vecPHEdge.clear();
	for (int ncnt = 0; ncnt < OneBlock.m_vecPFacetTri.size(); ncnt++)//清除面片表
	{
		delete OneBlock.m_vecPFacetTri[ncnt];
	}
	OneBlock.m_vecPFacetTri.clear();

	for (int ncnt = 0; ncnt < OneBlock.m_vecPVert.size(); ncnt++)//清除点表
	{
		if (OneBlock.m_vecPVert[ncnt]->vec_OneFac.size() != 0)
		{
			delete OneBlock.m_vecPVert[ncnt];
		}
	}
	OneBlock.m_vecPVert.clear();
}
void KitTool::StoretheMeshTri(STLModell *pMeshTri, STLModell *pMeshTriDate, int Onetime)
{
	int VetxID = 0;
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	for (int i = 0; i < pMeshTri->m_vecPFacetTri.size(); i++)//将三角面片放入容器中
	{
		PFACETTRI PFat; PFat = new FACETTRI; PVERT p0;
		if (pMeshTri->m_vecPFacetTri[i]->befrost&&Onetime != 0)  PFat->befrost = 1;
		for (int j = 0; j < 3; j++)
		{
			p0 = new VERT;
			p0->x = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->x;
			p0->y = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->y;
			p0->z = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->z;
			p0->Sdegree = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->Sdegree;//新顶点的平滑度
			if (pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->bused)
			{
				p0->bused = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->bused;
			}
			iter = map_Vert.find(*p0);
			if (iter != map_Vert.end())//已存在该点
			{
				int Num = iter->second;
				pMeshTriDate->m_vecPVert[Num]->vec_OneFac.push_back(PFat);
				PFat->m_PVerts[j] = pMeshTriDate->m_vecPVert[Num];
			}
			else
			{
				p0->VertexID = VetxID;
				p0->vec_OneFac.push_back(PFat);
				map_Vert.insert(pair<VERT, int>(*p0, pMeshTriDate->m_vecPVert.size()));
				pMeshTriDate->m_vecPVert.push_back(p0);
				PFat->m_PVerts[j] = p0;
				VetxID++;
			}
		}
		PFat->m_PFacetNorm = CalcuPfacNormal(PFat->m_PVerts[0], PFat->m_PVerts[1], PFat->m_PVerts[2]);///求面片的法矢
		PFat->m_PFacetNorm->Normalize();
		pMeshTriDate->m_vecPFacetTri.push_back(PFat);
	}
}
void KitTool::CreateEdgeTopByFctVert(Blockpath &OneBlock)
{
	int	nFct, nFCnt;
	PFACETTRI pFct;

	int nCntTmp;

	set<PEDGE, CompPEDGE> setEdge;
	pair<set<PEDGE, CompPEDGE>::iterator, bool> pair_setEdge_it_b;

	nFct = OneBlock.m_vecPFacetTri.size();
	for (nFCnt = 0; nFCnt < nFct; nFCnt++)
	{
		pFct = OneBlock.m_vecPFacetTri[nFCnt];

		PHEDGE pHEdge[3];
		pHEdge[0] = new HEDGE; pHEdge[1] = new HEDGE; pHEdge[2] = new HEDGE;

		OneBlock.m_vecPHEdge.push_back(pHEdge[0]);//FOR Lin JunFeng
		OneBlock.m_vecPHEdge.push_back(pHEdge[1]);

		OneBlock.m_vecPHEdge.push_back(pHEdge[2]);//FOR Lin JunFeng

		for (int nCnt = 0; nCnt < 3; nCnt++)
		{
			nCntTmp = (nCnt + 1) % 3;
			//半边拓扑信息
			pHEdge[nCnt]->pFacetAdj = pFct;
			pHEdge[nCnt]->pHEdgeNext = pHEdge[nCntTmp];
			pHEdge[nCnt]->pHEdgePre = pHEdge[(nCnt + 2) % 3];
			pHEdge[nCnt]->pVertEnd = pFct->m_PVerts[nCntTmp];

			//顶点拓扑信息
			pFct->m_PVerts[nCnt]->pHEdgeOut = pHEdge[nCnt];//这里好像不需要判断pHEdgeOut是否已经被赋值了，你怎么看 ^_^

			//边拓扑信息
			PEDGE	pEdge = new EDGE;
			if (pFct->m_PVerts[nCnt] < pFct->m_PVerts[nCntTmp])
			{
				pEdge->pVertStart = pFct->m_PVerts[nCnt];
				pEdge->pVertEnd = pFct->m_PVerts[nCntTmp];
			}
			else
			{
				pEdge->pVertEnd = pFct->m_PVerts[nCnt];
				pEdge->pVertStart = pFct->m_PVerts[nCntTmp];
			}

			pair_setEdge_it_b = setEdge.insert(pEdge);
			if (pair_setEdge_it_b.second)
			{
				pEdge->pHEdgeAdj = pHEdge[nCnt];

				//pEdge->m_nIDEdge = pMeshTri->m_vecPEdge.size();
				OneBlock.m_vecPEdge.push_back(pEdge);
			}
			else
			{
				delete pEdge;
				pEdge = 0;

				//完善半边拓扑信息
				pHEdge[nCnt]->pHEdgePair = (*pair_setEdge_it_b.first)->pHEdgeAdj;
				(*pair_setEdge_it_b.first)->pHEdgeAdj->pHEdgePair = pHEdge[nCnt];
			}
		}
		//面片拓扑信息
		pFct->pHEdge = pHEdge[0];
	}
}
bool KitTool::JudgeFaceDir(POINT3D v00, POINT3D v10, POINT3D v20, VECTOR3D FaceDir)
{
	VECTOR3D v1, v2, v3; double angel;
	v1 = v10 - v00;
	v2 = v20 - v00;
	v3 = v1*v2; v3.Normalize();
	angel = v3 | FaceDir;
	if (angel > 0)
	{
		return true;
	}
	return false;
}
void KitTool::StoreFactandVer(vec_PFACETTRI vec_Fact, vec_PFACETTRI &vec_PFact, vec_PVERT &m_vecPVert, map<VERT, int> &map_Vert, map<VERT, int>::iterator &iter, int &vetexID)
{
	for (int i = 0; i < vec_Fact.size(); i++)
	{
		PFACETTRI pF0 = vec_Fact[i];
		if (!pF0->becut)
		{
			PFACETTRI PFat; PFat = new FACETTRI; PVERT p0 = NULL;
			for (int j = 0; j < 3; j++)
			{
				p0 = new VERT;
				p0->VertexID = vetexID;
				p0->x = pF0->m_PVerts[j]->x;
				p0->y = pF0->m_PVerts[j]->y;
				p0->z = pF0->m_PVerts[j]->z;
				iter = map_Vert.find(*p0);
				if (pF0->m_PVerts[j]->bused)
				{
					p0->bused = pF0->m_PVerts[j]->bused;
				}
				if (iter != map_Vert.end())//已存在该点
				{
					int Num = iter->second;
					if (p0->bused)
					{
						m_vecPVert[Num]->bused = 1;
					}
					PFat->m_PVerts[j] = m_vecPVert[Num];
					m_vecPVert[Num]->vec_OneFac.push_back(PFat);
					delete p0;
					p0 = NULL;
				}
				else
				{
					map_Vert.insert(pair<VERT, int>(*p0, m_vecPVert.size()));
					m_vecPVert.push_back(p0);
					p0->vec_OneFac.push_back(PFat);
					PFat->m_PVerts[j] = p0;
					vetexID++;
				}
			}
			PFat->m_PFacetNorm = CalcuPfacNormal(PFat->m_PVerts[0], PFat->m_PVerts[1], PFat->m_PVerts[2]);///求面片的法矢
			vec_PFact.push_back(PFat);
		}
	}
}
bool KitTool::judeorientation(PPOLYGON poly)//判断旋向逆时针为bool为正
{
	vector<int>  vec_BoxN;
	vec_BoxN.resize(4, 0);
	int m = poly->m_vecPnts.size();
	double minx = poly->m_vecPnts[0].x;
	double maxx = poly->m_vecPnts[0].x;
	double miny = poly->m_vecPnts[0].y;
	double maxy = poly->m_vecPnts[0].y;

	for (int i = 1; i < m; i++)
	{
		if (poly->m_vecPnts[i].x < minx)
		{
			minx = poly->m_vecPnts[i].x;
			vec_BoxN[0] = i;
		}
		else
		{
			if (poly->m_vecPnts[i].x > maxx)
			{
				maxx = poly->m_vecPnts[i].x;
				vec_BoxN[1] = i;

			}
		}
		if (poly->m_vecPnts[i].y < miny)
		{
			miny = poly->m_vecPnts[i].y;
			vec_BoxN[2] = i;
		}
		else
		{
			if (poly->m_vecPnts[i].y > maxy)
			{
				maxy = poly->m_vecPnts[i].y;
				vec_BoxN[3] = i;
			}
		}
	}
	///set排序
	set<int>  setN;
	for (int i = 0; i < 4; i++)
	{
		setN.insert(vec_BoxN[i]);
	}
	int IsetSZ = setN.size();
	if (IsetSZ == 2)
	{
		VECTOR3D   vz(0, 0, 1);
		set<int> ::iterator iterS;
		iterS = setN.begin();
		if (*setN.begin() == 0)//处理特殊情况
		{
			VECTOR3D  v1 = poly->m_vecPnts[0] - poly->m_vecPnts[poly->m_vecPnts.size() - 2];
			VECTOR3D  v2 = poly->m_vecPnts[1] - poly->m_vecPnts[0];
			if (((v1*v2) | vz) > 0) return true;
			else return false;
		}
		else
		{
			VECTOR3D  v1 = poly->m_vecPnts[*setN.begin()] - poly->m_vecPnts[*setN.begin() - 1];
			VECTOR3D  v2 = poly->m_vecPnts[*setN.begin() + 1] - poly->m_vecPnts[*setN.begin()];
			if (((v1*v2) | vz) > 0) return true;
			else return false;
		}
	}
	else
	{
		VECTOR3D   vz(0, 0, 1);
		set<int> ::iterator iterS;
		iterS = setN.begin();
		iterS++;
		VECTOR3D  v1 = poly->m_vecPnts[*iterS] - poly->m_vecPnts[*setN.begin()];
		int num = *iterS;
		iterS++;
		VECTOR3D  v2 = poly->m_vecPnts[*iterS] - poly->m_vecPnts[num];
		if (((v1*v2) | vz) > 0) return true;
		else return false;
	}
}
bool KitTool::pointInpoly(POINT3D p, PPOLYGON poly)
{
	int counter = 0;
	double xinters;
	int sz = poly->m_vecPnts.size();
	int N = sz - 1;
	POINT3D p1 = poly->m_vecPnts[0];
	POINT3D p2;
	for (int i = 1; i<N; i++)
	{
		p2 = poly->m_vecPnts[i%N];
		if (p.y > min(p1.y, p2.y)) {
			if (p.y <= max(p1.y, p2.y)) {
				if (p.x <= max(p1.x, p2.x)) {
					if (p1.y != p2.y) {
						xinters = (p.y - p1.y)*(p2.x - p1.x) / (p2.y - p1.y) + p1.x;
						if (p1.x == p2.x || p.x <= xinters)
							counter++;
					}
				}
			}
		}
		p1 = p2;

		// POINT3D P2=poly->m_vecPnts[j];
		/* if(P1.y<p.y &&P2.y>=p.y
		||  P2.y<p.y&&P1.y>=p.y) {
		if(P1.x+(p.y-P1.y)/(P2.y-P1.y)*(P2.x-P1.x)<p.x) {
		ifIn=!ifIn;}}*/
	}
	if (counter % 2 == 0)
		return(false);
	else
		return(true);
}
double KitTool::GetPolyArea(PPOLYGON poly)
{
	vector<POINT3D>  vec_p = poly->m_vecPnts;
	double sum = 0;
	for (int j = 1; j < vec_p.size() - 1; j++)
	{
		double  s = 0;
		vector<POINT3D> m_vecPoint;
		m_vecPoint.push_back(vec_p[0]);
		m_vecPoint.push_back(vec_p[j]);
		m_vecPoint.push_back(vec_p[j + 1]);
		VECTOR3D v1 = m_vecPoint[1] - m_vecPoint[0];
		VECTOR3D v2 = m_vecPoint[2] - m_vecPoint[0];
		VECTOR3D v3 = v1*v2;
		s = 0.5*v3.GetLength();
		VECTOR3D vz(0, 0, 1);
		if ((v3 | vz)>0) sum += s;
		else sum -= s;
		sum = abs(sum);
	}
	return sum;
}

double KitTool::GetPolypolyArea(PPOLYPOLYGON ppolypolygon)
{
	double sum = 0;
	for (int i = 0; i < ppolypolygon->m_vecPPolygons.size(); i++)
	{

		double sumone = GetPolyArea(ppolypolygon->m_vecPPolygons[i]);

		if (judeorientation(ppolypolygon->m_vecPPolygons[i]))
		{
			sum += sumone;
		}
		else
		{
			sum -= sumone;
		}
	}
	return sum;
}
void  KitTool::RotateAllFacet(vector<PFACETTRI> &OneBlock, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < OneBlock.size(); i++)
	{
		POINT3D p0;
		for (int j = 0; j < 3; j++)
		{
			if (OneBlock[i]->m_PVerts[j]->bused==0)
			{
				p0.x = OneBlock[i]->m_PVerts[j]->x;
				p0.y = OneBlock[i]->m_PVerts[j]->y;
				p0.z = OneBlock[i]->m_PVerts[j]->z;
				p0 = p0 * TM;
				OneBlock[i]->m_PVerts[j]->x = p0.x;
				OneBlock[i]->m_PVerts[j]->y = p0.y;
				OneBlock[i]->m_PVerts[j]->z = p0.z;
				OneBlock[i]->m_PVerts[j]->bused=1;
			}
		}
	}
	for (int i = 0; i < OneBlock.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			if (OneBlock[i]->m_PVerts[j]->bused == 1)
			{
				OneBlock[i]->m_PVerts[j]->bused = 0;
			}
		}
	}
}
void  KitTool::RotatePolygon(vec_PPOLYPOLYGON pployploy, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < pployploy.size(); i++)
	{
		for (int j = 0; j < pployploy[i]->m_vecPPolygons.size(); j++)
		{
			for (int u = 0; u < pployploy[i]->m_vecPPolygons[j]->m_vecPnts.size(); u++)
			{
				pployploy[i]->m_vecPPolygons[j]->m_vecPnts[u] = pployploy[i]->m_vecPPolygons[j]->m_vecPnts[u] * TM;
			}
		}
	}
}
void  KitTool::RotateOneRingPolygon(PPOLYGON ploy, VECTOR3D curr_Dir, VECTOR3D tager_Dir)//旋转环
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < ploy->m_vecPnts.size(); i++)
	{
		ploy->m_vecPnts[i] = ploy->m_vecPnts[i] * TM;
	}
}
void  KitTool::RotateTwoRingPolygon(PPOLYPOLYGON vec_poly, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < vec_poly->m_vecPPolygons.size(); i++)
	{
		for (int j = 0; j < vec_poly->m_vecPPolygons[i]->m_vecPnts.size(); j++)
		{
			vec_poly->m_vecPPolygons[i]->m_vecPnts[j] = vec_poly->m_vecPPolygons[i]->m_vecPnts[j] * TM;
		}
	}
}
double KitTool::GetPerimeter(POLYGON poly)//得到周长
{
	double sum = 0;
	int SZ = poly.m_vecPnts.size();
	for (int i = 0; i < SZ - 1; i++)
	{
		VECTOR3D v = poly.m_vecPnts[i + 1] - poly.m_vecPnts[i];
		double S = v.GetLength();
		sum = sum + S;
	}
	return sum;
}
bool KitTool::lineSegInter(POINT3D p1, POINT3D p2, POINT3D p3, POINT3D p4, POINT3D &p)//求出线段交点
{
	bool   ifRegular = false;
	VECTOR2D p3p4;
	VECTOR2D p3p1;
	VECTOR2D p3p2;
	VECTOR2D p1p2;
	VECTOR2D p1p3;
	VECTOR2D p1p4;
	p3p4.dx = p4.x - p3.x;
	p3p4.dy = p4.y - p3.y;
	p3p1.dx = p1.x - p3.x;
	p3p1.dy = p1.y - p3.y;
	p3p2.dx = p2.x - p3.x;
	p3p2.dy = p2.y - p3.y;
	p1p2.dx = p2.x - p1.x;
	p1p2.dy = p2.y - p1.y;
	p1p3.dx = p3.x - p1.x;
	p1p3.dy = p3.y - p1.y;
	p1p4.dx = p4.x - p1.x;
	p1p4.dy = p4.y - p1.y;
	VECTOR3D cross_product_vector1 = p3p1*p3p4;
	VECTOR3D cross_product_vector2 = p3p2*p3p4;
	VECTOR3D cross_product_vector3 = p1p3*p1p2;
	VECTOR3D cross_product_vector4 = p1p4*p1p2;
	float cross_product_value1 = cross_product_vector1.dz*cross_product_vector2.dz;
	float cross_product_value2 = cross_product_vector3.dz*cross_product_vector4.dz;
	if (cross_product_vector1.dz == 0 && cross_product_vector2.dz != 0)
	{
		double min_x = min(p3.x, p4.x);
		double min_y = min(p3.y, p4.y);
		double max_x = max(p3.x, p4.x);
		double max_y = max(p3.y, p4.y);
		if (p1.x >= min_x&&p1.x <= max_x&&p1.y >= min_y&&p1.y <= max_y)
		{
			p.x = p1.x;
			p.y = p1.y;
			p.z = p1.z;
			ifRegular = true;
			return true;
		}
		else return false;

	}
	if (cross_product_vector2.dz == 0 && cross_product_vector1.dz != 0)
	{
		double min_x = min(p3.x, p4.x);
		double min_y = min(p3.y, p4.y);
		double max_x = max(p3.x, p4.x);
		double max_y = max(p3.y, p4.y);
		if (p2.x >= min_x&&p2.x <= max_x&&p2.y >= min_y&&p2.y <= max_y)
		{
			p.x = p2.x;
			p.y = p2.y;
			p.z = p2.z;
			ifRegular = true;
			return true;
		}
		else return false;

	}
	if (cross_product_vector3.dz == 0 && cross_product_vector4.dz != 0)
	{
		double min_x = min(p1.x, p2.x);
		double min_y = min(p1.y, p2.y);
		double max_x = max(p1.x, p2.x);
		double max_y = max(p1.y, p2.y);
		if (p3.x >= min_x&&p3.x <= max_x&&p3.y >= min_y&&p3.y <= max_y)
		{
			p.x = p3.x;
			p.y = p3.y;
			p.z = p3.z;
			ifRegular = true;
			return true;
		}
		else return false;
	}
	if (cross_product_vector4.dz == 0 && cross_product_vector3.dz != 0)
	{
		double min_x = min(p1.x, p2.x);
		double min_y = min(p1.y, p2.y);
		double max_x = max(p1.x, p2.x);
		double max_y = max(p1.y, p2.y);
		if (p4.x >= min_x&&p4.x <= max_x&&p4.y >= min_y&&p4.y <= max_y)
		{
			p.x = p4.x;
			p.y = p4.y;
			p.z = p4.z;
			ifRegular = true;
			return true;
		}
		else return false;
	}
	if (cross_product_vector1.dz == 0 && cross_product_vector2.dz == 0)
	{
		if (p1.x == p4.x&&p1.y == p4.y)
		{
			p.x = p1.x;
			p.y = p1.y;
			p.z = p1.z;
			ifRegular = true;
			return true;
		}
		else return false;
	}

	if (ifRegular == false)
	{
		double b1 = (p2.y - p1.y)*p1.x + (p1.x - p2.x)*p1.y;
		double b2 = (p4.y - p3.y)*p3.x + (p3.x - p4.x)*p3.y;
		double D = (p2.x - p1.x)*(p4.y - p3.y) - (p4.x - p3.x)*(p2.y - p1.y);
		double D1 = b2*(p2.x - p1.x) - b1*(p4.x - p3.x);
		double D2 = b2*(p2.y - p1.y) - b1*(p4.y - p3.y);
		double x0 = D1 / D;
		double y0 = D2 / D;
		p.x = x0;
		p.y = y0;
		p.z = p1.z;
	}
	if (cross_product_value1 < 0 && cross_product_value2 < 0)return true;
	else return false;

}
void KitTool::FindCircleNearPoint(POINT3D p, POINT3D &NearP, PPOLYGON poly)
{
	double dis, dmin, d2, d;

	vector<double>  vecDis;

	int n = poly->m_vecPnts.size();

	for (int m = 0; m < n; m++)
	{
		NearP = poly->m_vecPnts[m];

		dis = _DistOf(p, NearP);

		vecDis.push_back(dis);

	}
	int k = vecDis.size();

	dmin = vecDis[0];

	int num = 0;
	for (int i = 1; i < k; i++)
	{

		d2 = vecDis[i];
		if (d2 < dmin)
		{

			dmin = d2;
			num = i;
		}
		NearP = poly->m_vecPnts[num];
	}
}
void KitTool::FindCircleNearPoint1(POINT3D p, int &num, PPOLYGON poly)
{
	POINT3D pp;

	double dis, dmin, d2;

	vector<double>  vecDis;

	int n = poly->m_vecPnts.size();

	for (int m = 0; m < n; m++)
	{
		pp = poly->m_vecPnts[m];
		dis = _DistOf(p, pp);
		vecDis.push_back(dis);
	}
	int k = vecDis.size();

	dmin = vecDis[0];
	num = 0;
	for (int i = 1; i < k; i++)
	{

		d2 = vecDis[i];
		if (d2 < dmin)
		{

			dmin = d2;
			num = i;
		}
		pp = poly->m_vecPnts[num];
	}
}