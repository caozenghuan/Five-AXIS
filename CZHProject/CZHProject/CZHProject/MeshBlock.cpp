#include "MeshBlock.h"

MeshBlock::MeshBlock()
{
	pSimply = 0;
	m_Laplacian = false;
}
MeshBlock::~MeshBlock()
{
	if (pSimply)
	{
		delete pSimply;
	}
}
void MeshBlock::SharpEdgeDetection(STLModell *pMeshTri, vec_EDGE &vec_edge)
{
	for (unsigned int i = 0; i < pMeshTri->m_vecPEdge.size(); i++)
	{
		if (pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pHEdgePair != NULL)
		{
			vector<PFACETTRI> vecper;
			FACETTRI f1 = *PFACETTRI(pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pFacetAdj);
			FACETTRI f2 = *PFACETTRI(pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pHEdgePair->pFacetAdj);
			float angle = acos(*f1.m_PFacetNorm | *f2.m_PFacetNorm);
			float threshold = PI / 8.0;
			if (angle > threshold)
			{
				vec_edge.push_back(*pMeshTri->m_vecPEdge[i]);
			}
		}
		else
		{
			vec_edge.push_back(*pMeshTri->m_vecPEdge[i]);
		}
	}
}
//void MeshBlock::SharpVetexDetection(STLModell *pMeshTri)
//{
//	for (unsigned int i = 0; i < pMeshTri->m_vecPEdge.size(); i++)
//	{
//		if (pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pHEdgePair != NULL)
//		{
//			vector<PFACETTRI> vecper;
//			FACETTRI f1 = *PFACETTRI(pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pFacetAdj);
//			FACETTRI f2 = *PFACETTRI(pMeshTri->m_vecPEdge[i]->pHEdgeAdj->pHEdgePair->pFacetAdj);
//			float angle = acos(*f1.m_PFacetNorm | *f2.m_PFacetNorm);
//			float threshold = PI / 8.0;
//			if (angle > threshold)
//			{
//				vec_pvert.push_back(pMeshTri->m_vecPEdge[i]->pVertEnd);
//			}
//		}
//	}
//
//}
bool MeshBlock::CutLineTraceProjection(vector<POINT3D> pt, vector<PVERT>&path, vector<Insepoint> &pathTemp, STLModell* m_Tmesh)
{
	if (m_Tmesh == NULL)
	{
		return false;
	}
	//////建立KD树查找拾取点的最近点
	if (m_Tmesh->m_Tmesh_KD != NULL)  m_Tmesh->m_Tmesh_KD = NULL;
	vector<point> vec_p; vec_p.clear();
	for (int i = 0; i < m_Tmesh->m_vecPVert.size(); i++)
	{
		point vre(m_Tmesh->m_vecPVert[i]->x, m_Tmesh->m_vecPVert[i]->y, m_Tmesh->m_vecPVert[i]->z);
		vec_p.push_back(vre);
	}

	float* v0 = &vec_p[0][0];
	m_Tmesh->m_Tmesh_KD = new KDtree(v0, m_Tmesh->m_vecPVert.size());

	double juli = PkiTool.dis(*(m_Tmesh->m_vecPFacetTri.back()->m_PVerts[0]), *(m_Tmesh->m_vecPFacetTri.back()->m_PVerts[1]));
	pcedie.m_Point_BeginEnd.clear();
	for (int i = 0; i < pt.size(); i++)
	{
		int vindex = -1;
		vec p1 = vec(pt[i].x, pt[i].y, pt[i].z);
		const float *Match = m_Tmesh->m_Tmesh_KD->closest_to_pt(p1, juli * 10);
		if (Match)
		{
			vindex = (Match - v0) / 3;
			pcedie.m_Point_BeginEnd.push_back(vindex);
		}
	}
	if (pcedie.m_Point_BeginEnd.size() < 3)return false;
	////////查找点所在的三角面片
	//for (int i = 0; i < pt.size(); i++)
	//{
	//	/*vec_PFACETTRI vecper;
	//	PkiTool.FindOneRFac(m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]], vecper);*/
	//	for (int j = 0; j < m_Tmesh->m_vecPFacetTri.size();j++)
	//	{
	//		PFACETTRI pf0 = m_Tmesh->m_vecPFacetTri[j];
	//		double sFace=0,sumface;
	//		PVERT Pver = new VERT;
	//		for (int k = 0; k < 3; k++)
	//		{
	//			int nct = (k + 1) % 3;
	//			Pver->x = pt[i].x; Pver->y = pt[i].y; Pver->z = pt[i].z;
	//			sFace += PkiTool.AreaTri(pf0->m_PVerts[k], pf0->m_PVerts[nct], Pver);
	//		}
	//		sumface = PkiTool.AreaTri(pf0->m_PVerts[0], pf0->m_PVerts[1], pf0->m_PVerts[2]);
	//		if (/*PkiTool.Comparevector(pf0, dir_face)*/sFace==sumface)
	//		{
	//			pt[i].facenumber = pf0->Fnumber;
	//			break;
	//		}
	//		delete Pver;
	//	}
	//}
	//for (int i = 0; i<pcedie.m_Point_BeginEnd.size(); i++)//优化交互拾取的分割点
	//{
	//	PVERT Rpoint = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]];
	//	vec_PVERT vec_Twopvert; vec_Twopvert.clear();
	//	KitTool::FindOneRing(Rpoint, vec_Twopvert);
	//	//KitTool::FindTwoRing_new(Rpoint, vec_Twopvert);//二阶邻域点，若是效果不太好可以求三阶邻域

	//	int Sdist = 100; PVERT  PVer;
	//	for (int j = 0; j < vec_Twopvert.size(); j++)
	//	{
	//		vec_PHEDGE vecpH; double distance = 100; PVERT pOneVert;
	//		KitTool::FindOnePH(vec_Twopvert[j], vecpH);
	//		for (int k = 0; k < vecpH.size(); k++)
	//		{
	//			if (!vecpH[k]->bStatus)
	//			{
	//				vecpH[k]->bStatus = 1;
	//				if (vecpH[k]->pHEdgePair != NULL)
	//				{
	//					FACETTRI f1 = *PFACETTRI(vecpH[k]->pFacetAdj);
	//					FACETTRI f2 = *PFACETTRI(vecpH[k]->pHEdgePair->pFacetAdj);
	//					float angle = acos(*f1.m_PFacetNorm | *f2.m_PFacetNorm);
	//					float threshold = PI / 8.0;
	//					if (angle > threshold)
	//					{
	//						distance = PkiTool.dis(*vecpH[k]->pVertEnd, *Rpoint);
	//						pOneVert = vecpH[k]->pVertEnd;
	//					}
	//				}
	//				if (distance < Sdist)
	//				{
	//					Sdist = distance; PVer = pOneVert;
	//				}
	//			}
	//		}
	//	}
	//	if (Sdist == 100)
	//	{
	//		PVer = Rpoint;
	//	}
	//	pcedie.m_Point_BeginEnd[i] = PVer->VertexID;
	//}

	//投影法求轨迹点
	for (int i = 0; i < pcedie.m_Point_BeginEnd.size(); i++)
	{
		Insepoint pStart, pEnd;
		if (i == pcedie.m_Point_BeginEnd.size() - 1)
		{
			pStart.pTemp = *m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]];//给出初始交点值
			pStart.InsEdge_Start = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]];//给出初始交点所在边的起点
			pStart.InsEdge_End = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
			pEnd.pTemp = *m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[0]];//给出终点交点值
			pEnd.InsEdge_Start = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[0]];//给出终点交点所在边的起点
			pEnd.InsEdge_End = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[0]]->pHEdgeOut->pVertEnd;//给出终点交点所在边的终点
			pStart.Epoint = 1; pEnd.Epoint = 1;
		}
		else
		{
			pStart.pTemp = *m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]];
			pStart.InsEdge_Start = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]];
			pStart.InsEdge_End = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i]]->pHEdgeOut->pVertEnd;
			pEnd.pTemp = *m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i + 1]];
			pEnd.InsEdge_Start = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i + 1]];
			pEnd.InsEdge_End = m_Tmesh->m_vecPVert[pcedie.m_Point_BeginEnd[i + 1]]->pHEdgeOut->pVertEnd;
			pStart.Epoint = 1; pEnd.Epoint = 1;
		}
		//求出选定两点上所有相交点
		bool nwrite = true;
		Insepoint pTemp = pStart;
		vector<Insepoint> vec_point; int  ntime;
		do
		{
			Insepoint Ipoint;
			//分三种情况，一种在面片内，一种是在边上，另外一种是在顶点上
			vec_point.clear();
			if (pTemp.Epoint)
			{
				VECTOR3D VertexNormal; PVERT Pverts;
				VertexNormal = pTemp.InsEdge_Start->VertexNormal;
				Pverts = pTemp.InsEdge_Start;
				VECTOR3D F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();

				vec_PFACETTRI OneRingface = Pverts->vec_OneFac; ntime = 0;
				for (int j = 0; j < OneRingface.size(); j++)
				{
					for (int k = 0; k < 3; k++)
					{
						int nct = (k + 1) % 3;
						Insepoint poit;
						poit.InsEdge_Start = OneRingface[j]->m_PVerts[k];
						poit.InsEdge_End = OneRingface[j]->m_PVerts[nct];

						if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint) && ntime < 2)
						{
							if (poit.pTemp != pTemp.pTemp)
							{
								poit.pFac = OneRingface[j];
								vec_point.push_back(poit);
								ntime++;
							}
						}
					}

				}
			}
			else
			{
				PVERT pvertemp; pvertemp = new VERT;
				pvertemp->x = pTemp.pTemp.x; pvertemp->y = pTemp.pTemp.y; pvertemp->z = pTemp.pTemp.z;
				VECTOR3D VertexNormal = PkiTool.CalcuVerNormal(pvertemp, pTemp.pFac);
				VECTOR3D F_normal;
				F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();
				delete pvertemp; ntime = 0;
				vec_PFACETTRI FaceN;
				if (pTemp.PVertAdj != NULL)
				{
					FaceN.push_back(PFACETTRI(pTemp.PVertAdj->pFacetAdj));
				}
				else
				{
					return false;
				}
				FaceN.push_back(pTemp.pFac);
				for (int j = 0; j < 2; j++)
				{
					for (int k = 0; k < 3; k++)
					{
						int nct = (k + 1) % 3;
						Insepoint poit;
						poit.InsEdge_Start = FaceN[j]->m_PVerts[k];
						poit.InsEdge_End = FaceN[j]->m_PVerts[nct];

						if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint) && ntime < 2)
						{
							if (poit.pTemp != pTemp.pTemp)
							{
								poit.pFac = FaceN[j];
								vec_point.push_back(poit);
								ntime++;
							}
						}
					}
				}

			}
			VECTOR3D v1, v2, v3;
			v1 = pEnd.pTemp - pTemp.pTemp; v1.Normalize();
			v2 = vec_point[0].pTemp - pTemp.pTemp; v2.Normalize();
			v3 = vec_point[1].pTemp - pTemp.pTemp; v3.Normalize();
			double angel_0, angel_1;
			angel_0 = v1 | v2; angel_1 = v1 | v3;
			(angel_0 < angel_1) ? Ipoint = vec_point[1] : Ipoint = vec_point[0];//选取正确的交点
			if (pTemp.Epoint)
			{
				pTemp.pFac = Ipoint.pFac;//保存当前求交面片
			}
			//////排除较小交点，防止产生小三角形
			vector<Insepoint> TwoinsertVert; TwoinsertVert.clear();
			TwoinsertVert.push_back(pTemp); TwoinsertVert.push_back(Ipoint);
			double L, L0, L1; vector<double> vec_L;
			for (int j = 0; j < 2; j++)
			{
				POINT3D ppStart, ppEnd;
				ppStart = *TwoinsertVert[j].InsEdge_Start; ppEnd = *TwoinsertVert[j].InsEdge_End;
				L = PkiTool.dis(ppStart, ppEnd);
				L0 = PkiTool.dis(ppStart, TwoinsertVert[j].pTemp); L1 = PkiTool.dis(ppEnd, TwoinsertVert[j].pTemp);
				(L0 < L1) ? (L = L0 / L) : (L = L1 / L); vec_L.push_back(L);
				(L0 < L1) ? (TwoinsertVert[j].NearVert = TwoinsertVert[j].InsEdge_Start) : (TwoinsertVert[j].NearVert = TwoinsertVert[j].InsEdge_Start);
			}
			pTemp.NearVert = TwoinsertVert[0].NearVert; Ipoint.NearVert = TwoinsertVert[1].NearVert;
			if (pTemp.NearVert == Ipoint.NearVert)
			{
				double Sa1, Sa2;
				Sa1 = PkiTool.AreaTri(pTemp.NearVert, pTemp.pTemp, Ipoint.pTemp);
				Sa2 = PkiTool.AreaTri(pTemp.pFac->m_PVerts[0], pTemp.pFac->m_PVerts[1], pTemp.pFac->m_PVerts[2]);
				if (/*Sa1 / Sa2<0.20&&*/vec_L[0]<0.20)
				{
					/*				PVERT FarVert;
					(pTemp.NearVert == pTemp.InsEdge_Start) ? FarVert = pTemp.InsEdge_End : FarVert = pTemp.InsEdge_Start;
					pTemp.pTemp.x = 0.65*pTemp.NearVert->x + 0.35*FarVert->x;
					pTemp.pTemp.y = 0.65*pTemp.NearVert->y + 0.35*FarVert->y;
					pTemp.pTemp.z = 0.65*pTemp.NearVert->z + 0.35*FarVert->z;*/
				}
			}
			pathTemp.push_back(pTemp);
			PHEDGE vec_PH[3];
			vec_PH[0] = pTemp.pFac->pHEdge; vec_PH[1] = vec_PH[0]->pHEdgeNext; vec_PH[2] = vec_PH[0]->pHEdgePre;
			for (int j = 0; j < 3; j++)
			{
				if (vec_PH[j]->pHEdgePair != NULL)
				{
					if (vec_PH[j]->pVertEnd == Ipoint.InsEdge_End&&vec_PH[j]->pHEdgePre->pVertEnd == Ipoint.InsEdge_Start)
					{
						Ipoint.PVertAdj = vec_PH[j];
						Ipoint.pFac = PFACETTRI(vec_PH[j]->pHEdgePair->pFacetAdj); break;
					}
					if (vec_PH[j]->pVertEnd == Ipoint.InsEdge_Start&&vec_PH[j]->pHEdgePre->pVertEnd == Ipoint.InsEdge_End)
					{
						Ipoint.PVertAdj = vec_PH[j];
						Ipoint.pFac = PFACETTRI(vec_PH[j]->pHEdgePair->pFacetAdj); break;
					}
				}
				else
				{
					return false;
				}
			}
			pTemp = Ipoint;//继续循环
			nwrite = false;
		} while (PkiTool.Comparepoint(pTemp.pFac, pEnd.pTemp));
		pathTemp.push_back(pTemp);
	}
	for (int i = 0; i < pathTemp.size(); i++)
	{
		///////////////将所有轨迹点保存到Path容器中
		PVERT pver = new VERT;
		pver->x = pathTemp[i].pTemp.x; pver->y = pathTemp[i].pTemp.y; pver->z = pathTemp[i].pTemp.z;
		path.push_back(pver);
	}
	delete m_Tmesh->m_Tmesh_KD;//2017.7.2
	return true;
}
bool  MeshBlock::CutLineDir(vector<vec_PVERT> &FeaPointAll,vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace, STLModell* m_Tmesh)//追踪投影法求分割线
{
	//////////////求出各环附近的所有特征点//////////////////////
	/*vector<vec_PVERT> FeaPointAll;*/ vec_PVERT FeaPoint;
	for (int i = 0; i < path.size(); i++)
	{
		map<VERT, int>map_Vert;
		map<VERT, int>::iterator iter;
		for (int j = 0; j < path[i].size(); j++)
		{
			vec_PVERT vec_Fourpvert; vec_Fourpvert.clear();
			vec_PVERT vec_Twopvert; vec_Twopvert.clear();
			KitTool::FindTwoRing_new(path[i][j], vec_Twopvert);//二阶邻域点，若是效果不太好可以求三阶邻域
			KitTool::FindFourRing(path[i][j], vec_Fourpvert);
			for (int k = 0; k < vec_Fourpvert.size(); k++)
			{
				vec_PHEDGE vecpH; 
				KitTool::FindOnePH(vec_Fourpvert[k], vecpH);
				for (int m = 0; m < vecpH.size(); m++)
				{
					if (vecpH[m]->pHEdgePair != NULL&&!vecpH[m]->bStatus)
					{
						FACETTRI f1 = *PFACETTRI(vecpH[m]->pFacetAdj);
						FACETTRI f2 = *PFACETTRI(vecpH[m]->pHEdgePair->pFacetAdj);
						float angle = acos(*f1.m_PFacetNorm | *f2.m_PFacetNorm);
						float threshold = PI / 12.0;
						VECTOR3D v0, v1, v2;
						v0 = *vecpH[m]->pVertEnd - *vecpH[m]->pHEdgePre->pVertEnd; v0.Normalize();
						v1 = *f1.m_PFacetNorm; v2 = *f2.m_PFacetNorm;
						double Nangel = v0 | (v1*v2);
						if (angle > threshold&&Nangel<0)
						{
							PVERT POint[2];  POint[0] = vecpH[m]->pHEdgePre->pVertEnd; POint[1] = vecpH[m]->pVertEnd;
							for (int n = 0; n < 2;n++)
							{
								iter = map_Vert.find(*POint[n]);
								if (iter == map_Vert.end())//不存在该点
								{
									map_Vert.insert(pair<VERT, int>(*POint[n], FeaPoint.size()));
									FeaPoint.push_back(POint[n]);
								}
							}
						}
					}
					vecpH[m]->bStatus = 1;
				}
			}
		}
		for (int j = 0; j < m_Tmesh->m_vecPHEdge.size(); j++)////还原半边的标记
		{
			if (m_Tmesh->m_vecPHEdge[j]->bStatus)
			{
				m_Tmesh->m_vecPHEdge[j]->bStatus = 0;
			}
		}
		FeaPointAll.push_back(FeaPoint);
		FeaPoint.clear();
	}
	///估算各环的法矢量///
	vec_VECTOR3D RingDir;
	for (int i = 0; i < path.size(); i++)//平面拟合求环法向量
	{
//		Matrix<double, 3, 3> equ;    // 矩阵中元素值初始为0
//		equ = Matrix<double, 3, 3>::Zero();
//		Matrix<double, 3, 1>  B;
//		B = Matrix<double, 3, 1>::Zero();
//#pragma omp parallel for
//		for (int j = 0; j < path[i].size(); j++)
//		{
//			PVERT pPt = path[i][j];
//			float u = pPt->x; float v = pPt->y; float h = pPt->z;             //邻点坐标值u,v,h
//
//			//利用邻点的坐标通过最小二乘法拟合平面
//			//解线性方程组
//			equ(0, 0) += u*u; equ(0, 1) += u*v; equ(0, 2) += u;
//
//			equ(1, 0) += u*v; equ(1, 1) += v*v; equ(1, 2) += v;
//
//			equ(2, 0) += u; equ(2, 1) += v; equ(2, 2) += 1;
//
//
//			B(0, 0) += u *h;  B(1, 0) += v * h;    B(2, 0) += h;
//		}
//
//		Matrix<double, 3, 1>  solu;
//		solu = Matrix<double, 3, 1>::Zero();
//		solu = equ.lu().solve(B);
//		double a, b, c;
//		a = solu(0, 0); b = solu(1, 0); c = solu(2, 0);
//		VECTOR3D v2 = VECTOR3D(a,b,c); v2.Normalize();
//		RingDir.push_back(v2);

		int nct = path[i].size();
		int nct0 = int(0.3*nct); int nct1 = int(0.6*nct);
		VECTOR3D v0, v1, v2;
		v0 = *path[i][nct0] - *path[i][0];
		v1 = *path[i][nct1] - *path[i][0];
		v2 = v0*v1; v2.Normalize();
		RingDir.push_back(v2);
	}
	///////////////////对各个环求最近的凹特征点///////////////////////
	for (int i = 0; i < FeaPointAll.size();i++)
	{
		vec_PVERT FpointTem;
		double Alldis = 1000; PVERT Pver;
		FeaPoint = FeaPointAll[i];
		for (int j = 0; j < FeaPoint.size();j++)
		{
			VECTOR3D v1 = PkiTool.PlanePointDistance(*FeaPoint[j], *path[i][0], All_VecDir[i + 1]);
			double Udis = v1.GetLength();
			if (Udis <Alldis)
			 {
				Pver = FeaPoint[j]; Alldis = Udis ;
			 }
		}
		if (FeaPoint.size()==0)//没找到特征点
		{
			Pver = path[i][0];
		}
		FpointTem.push_back(Pver);
		CFpoint.push_back(FpointTem);
	}
	for (int i = 0; i < CFpoint.size(); i++)
	{
		vector<Insepoint> pathTemp; pathTemp.clear();
		vector<Insepoint> vec_point;
		Insepoint pStart, pEnd;
		pStart.pTemp = *CFpoint[i][0];//给出初始交点值
		pStart.InsEdge_Start = CFpoint[i][0];//给出初始交点所在边的起点
		pStart.InsEdge_End = CFpoint[i][0]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
		pStart.Epoint = 1;

		//找出环的重心点
		for (int j = 0; j < path[i].size();j++)
		{
			pEnd.pTemp.x += path[i][j]->x;
			pEnd.pTemp.y += path[i][j]->y;
			pEnd.pTemp.z += path[i][j]->z;
		}
		pEnd.pTemp.x /= path[i].size();
		pEnd.pTemp.y /= path[i].size();
		pEnd.pTemp.z /= path[i].size();
		Insepoint pTemp = pStart;
		vec_point.clear();
		VECTOR3D F_normal = All_VecDir[i + 1];//最优成型方向
		//VECTOR3D F_normal = RingDir[i];
		do
		{
			Insepoint Ipoint;
			//分三种情况，一种在面片内，一种是在边上，另外一种是在顶点上
			if (pTemp.Epoint)
			{
				VECTOR3D VertexNormal; PVERT Pverts;
				VertexNormal = pTemp.InsEdge_Start->VertexNormal;
				Pverts = pTemp.InsEdge_Start;

				VECTOR3D F_normal = RingDir[i];//环法矢量方向
				//核实成型方向与环法矢量是否平行
				double angel = RingDir[i] | F_normal;
				double OneZero = 0.06;//20度为界
				angel = abs(abs(angel) - 1);
				if (angel > OneZero)//与最优方向不平行
				{
					F_normal = RingDir[i];
					//for (int k = 0; k < All_VecDir.size(); k++)
					//{
					//	angel = RingDir[i] | All_VecDir[k];
					//	angel = abs(abs(angel) - 1);
					//	if (angel < OneZero)//最优方向与环平行
					//	{
					//		F_normal = All_VecDir[k];
					//		break;
					//	}
					//}
				}

				vec_PFACETTRI OneRingface = Pverts->vec_OneFac;
				for (int k = 0; k < OneRingface.size(); k++)
				{
					for (int m = 0; m < 3; m++)
					{
						int nct = (m + 1) % 3;
						Insepoint poit;
						poit.InsEdge_Start = OneRingface[k]->m_PVerts[m];
						poit.InsEdge_End = OneRingface[k]->m_PVerts[nct];

						if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint))
						{
							if (poit.pTemp != pTemp.pTemp)
							{
								poit.pFac = OneRingface[k];
								vec_point.push_back(poit);
							}
						}
					}

				}
				Insepoint IsTemp;
				VECTOR3D v1, v2; double angel_1,angel_0 = -1;
				v1 = pEnd.pTemp - pTemp.pTemp; v1.Normalize();
				for (int k = 0; k < vec_point.size();k++)
				{
					v2 = vec_point[k].pTemp - pTemp.pTemp; v2.Normalize();
					angel_1 = v1 | v2;
					if (angel_1>angel_0)
					{
						angel_0 = angel_1; IsTemp = vec_point[k];
					}
				}
				Ipoint = IsTemp;
				pTemp.pFac = Ipoint.pFac;//保存当前求交面片
			}
			else
			{
				PVERT pvertemp; pvertemp = new VERT;
				pvertemp->x = pTemp.pTemp.x; pvertemp->y = pTemp.pTemp.y; pvertemp->z = pTemp.pTemp.z;

				//VECTOR3D F_normal = All_VecDir[i + 1];//最优成型方向
				//VECTOR3D F_normal = RingDir[i];//环法矢量方向
				delete pvertemp;
				for (int k = 0; k < 3; k++)
				{
					int nct = (k + 1) % 3;
					Insepoint poit;
					poit.InsEdge_Start = pTemp.pFac->m_PVerts[k];
					poit.InsEdge_End = pTemp.pFac->m_PVerts[nct];

					if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint))
					{
						if (poit.pTemp != pTemp.pTemp)
						{
							Ipoint = poit; break;
						}
					}
				}

			}
			pathTemp.push_back(pTemp);

			PHEDGE vec_PH[3]; vec_PH[0] = pTemp.pFac->pHEdge;
			vec_PH[1] = vec_PH[0]->pHEdgeNext; vec_PH[2] = vec_PH[0]->pHEdgePre;
			for (int k = 0; k < 3; k++)
			{
				if (vec_PH[k]->pHEdgePair != NULL)
				{
					if (vec_PH[k]->pVertEnd == Ipoint.InsEdge_End&&vec_PH[k]->pHEdgePre->pVertEnd == Ipoint.InsEdge_Start)
					{
						Ipoint.PVertAdj = vec_PH[k];
						Ipoint.pFac = PFACETTRI(vec_PH[k]->pHEdgePair->pFacetAdj); break;
					}
					if (vec_PH[k]->pVertEnd == Ipoint.InsEdge_Start&&vec_PH[k]->pHEdgePre->pVertEnd == Ipoint.InsEdge_End)
					{
						Ipoint.PVertAdj = vec_PH[k];
						Ipoint.pFac = PFACETTRI(vec_PH[k]->pHEdgePair->pFacetAdj); break;
					}
				}
				else
				{
					return false;
				}
			}
			pTemp = Ipoint;//继续循环
		} while (PkiTool.Comparepoint(pTemp.pFac, pStart.pTemp));
		pathTemp.push_back(pTemp);
		vec_PVERT vec_InPoint;
		for (int j = 0; j < pathTemp.size(); j++)
		{
			///////////////将所有轨迹点保存到Path容器中
			PVERT pver = new VERT;
			pver->x = pathTemp[j].pTemp.x; pver->y = pathTemp[j].pTemp.y; pver->z = pathTemp[j].pTemp.z;
			vec_InPoint.push_back(pver);
		}
		double Raterot = pathTemp.size() / (pathTrace[i].size()*1.0);
		pathTrace[i] = pathTemp;
		path[i] = vec_InPoint;
	}
	return true;
}
bool  MeshBlock::CutLineDir1(vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace)//追踪投影法求分割线
{
	//对各个环求最近的凹特征点
	for (int i = 0; i < path.size(); i++)
	{
		double Sdist = 100;//每个环到凹点的最短距离,初始阈值设定为100
		PVERT PVer;
		vec_PVERT FpointTem;
		int FirNub; int ringSize = path[i].size();
		double sdistemp = 100; PVERT PVerTemp;
		for (int j = 0; j < path[i].size(); j++)
		{
			if (CalDispath(PVerTemp, path[i][j], sdistemp, path[i]))
			{
				if (sdistemp < Sdist)
				{
					Sdist = sdistemp; PVer = PVerTemp; FirNub = j;
				}
			}
		}
		(Sdist == 100) ? (FpointTem.push_back(path[i][0])) : (FpointTem.push_back(PVer));//是否找到凹点
		//查找环的第二个点，在距离初始点1/2处查找
		int n1 = int(0.5 * ringSize) + FirNub;  Sdist = 100;
		if (n1 < ringSize)//情况一
		{
			if (CalDispath(PVerTemp, path[i][n1], sdistemp, path[i]))
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)//没找到第二凹点
			{
				FpointTem.push_back(path[i][n1]);
			}
			else
			{
				FpointTem.push_back(PVer);
			}
		}
		else
		{
			int Nlet = n1 - ringSize;
			if (CalDispath(PVerTemp, path[i][Nlet], sdistemp, path[i]))//情况二
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)
			{
				FpointTem.push_back(path[i][Nlet]);
			}
			else
			{
				FpointTem.push_back(PVer);
			}
		}
		CFpoint.push_back(FpointTem);
	}
	int NBrach = 0;
	///////////////查找模型有几个主干///////
	for (int i = 0; i < Rbox.size(); i++)
	{
		if (Rbox[i].OneBlockPair.size() != 1) NBrach++;
	}
	path.clear(); pathTrace.clear();

	for (int i = 0; i < CFpoint.size(); i++)
	{
		vector<Insepoint> pathTemp; pathTemp.clear();
		vector<Insepoint> vec_point;
		for (int j = 0; j < 2; j++)
		{
			int nqt = (j + 1) % 2;
			Insepoint pStart, pEnd;
			pStart.pTemp = *CFpoint[i][j];//给出初始交点值
			pStart.InsEdge_Start = CFpoint[i][j];//给出初始交点所在边的起点
			pStart.InsEdge_End = CFpoint[i][j]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
			pStart.Epoint = 1;

			pEnd.pTemp = *CFpoint[i][nqt];//给出初始交点值
			pEnd.InsEdge_Start = CFpoint[i][nqt];//给出初始交点所在边的起点
			pEnd.InsEdge_End = CFpoint[i][nqt]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
			pEnd.Epoint = 1;
			Insepoint pTemp = pStart;
			int  ntime;
			do
			{
				Insepoint Ipoint;
				//分三种情况，一种在面片内，一种是在边上，另外一种是在顶点上
				if (pTemp.Epoint)
				{
					VECTOR3D VertexNormal;
					VertexNormal = pTemp.InsEdge_Start->VertexNormal;
					VECTOR3D F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();
					PVERT Pverts = pTemp.InsEdge_Start;
					vec_PFACETTRI OneRingface = Pverts->vec_OneFac;
					ntime = 0; vec_point.clear();
					for (int k = 0; k < OneRingface.size(); k++)
					{
						for (int m = 0; m < 3; m++)
						{
							int nct = (m + 1) % 3;
							Insepoint poit;
							poit.InsEdge_Start = OneRingface[k]->m_PVerts[m];
							poit.InsEdge_End = OneRingface[k]->m_PVerts[nct];

							if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint))
							{
								if (poit.pTemp != pTemp.pTemp)
								{
									poit.pFac = OneRingface[k];
									vec_point.push_back(poit);
									ntime++;
								}
							}
							if (ntime == 2)  break;
						}
						if (ntime == 2)  break;
					}
					VECTOR3D v1, v2, v3;
					v1 = pEnd.pTemp - pTemp.pTemp; v1.Normalize();
					v2 = vec_point[0].pTemp - pTemp.pTemp; v2.Normalize();
					v3 = vec_point[1].pTemp - pTemp.pTemp; v3.Normalize();
					double angel_0, angel_1;
					angel_0 = v1 | v2; angel_1 = v1 | v3;
					(angel_0 < angel_1) ? Ipoint = vec_point[1] : Ipoint = vec_point[0];//选取正确的交点
					pTemp.pFac = Ipoint.pFac;//保存当前求交面片
				}
				else
				{
					PVERT pvertemp; pvertemp = new VERT;
					pvertemp->x = pTemp.pTemp.x; pvertemp->y = pTemp.pTemp.y; pvertemp->z = pTemp.pTemp.z;

					VECTOR3D VertexNormal = PkiTool.CalcuVerNormal(pvertemp, pTemp.pFac);

					VECTOR3D F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();
					delete pvertemp;
					bool ifFind = false;
					for (int k = 0; k < 3; k++)
					{
						int nct = (k + 1) % 3;
						Insepoint poit;
						poit.InsEdge_Start = pTemp.pFac->m_PVerts[k];
						poit.InsEdge_End = pTemp.pFac->m_PVerts[nct];

						if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint))
						{
							if (poit.pTemp != pTemp.pTemp)
							{
								Ipoint = poit;
								ifFind = true; break;
							}
						}
					}
					if (!ifFind)
					{
						int ceshi = 0;
					}
				}
				pathTemp.push_back(pTemp);

				PHEDGE vec_PH[3]; vec_PH[0] = pTemp.pFac->pHEdge;
				vec_PH[1] = vec_PH[0]->pHEdgeNext; vec_PH[2] = vec_PH[0]->pHEdgePre;
				for (int k = 0; k < 3; k++)
				{
					if (vec_PH[k]->pHEdgePair != NULL)
					{
						if (vec_PH[k]->pVertEnd == Ipoint.InsEdge_End&&vec_PH[k]->pHEdgePre->pVertEnd == Ipoint.InsEdge_Start)
						{
							Ipoint.PVertAdj = vec_PH[k];
							Ipoint.pFac = PFACETTRI(vec_PH[k]->pHEdgePair->pFacetAdj); break;
						}
						if (vec_PH[k]->pVertEnd == Ipoint.InsEdge_Start&&vec_PH[k]->pHEdgePre->pVertEnd == Ipoint.InsEdge_End)
						{
							Ipoint.PVertAdj = vec_PH[k];
							Ipoint.pFac = PFACETTRI(vec_PH[k]->pHEdgePair->pFacetAdj); break;
						}
					}
					else
					{
						return false;
					}
				}
				pTemp = Ipoint;//继续循环
			} while (PkiTool.Comparepoint(pTemp.pFac, pEnd.pTemp));
		}
		vec_PVERT vec_InPoint;
		for (int j = 0; j < pathTemp.size(); j++)
		{
			///////////////将所有轨迹点保存到Path容器中
			PVERT pver = new VERT;
			pver->x = pathTemp[j].pTemp.x; pver->y = pathTemp[j].pTemp.y; pver->z = pathTemp[j].pTemp.z;
			vec_InPoint.push_back(pver);
		}
		pathTrace.push_back(pathTemp);
		path.push_back(vec_InPoint);
	}
	return true;
}
bool MeshBlock::CutLineDir2(vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace, vec_PVERT m_vecPVert)
{
	//对各个环求最近的凹特征点
	for (int i = 0; i < path.size(); i++)
	{
		double Sdist = 100;//每个环到凹点的最短距离,初始阈值设定为100
		PVERT PVer;
		vec_PVERT FpointTem;
		int FirNub; int ringSize = path[i].size();
		double sdistemp = 100; PVERT PVerTemp;
		for (int j = 0; j < path[i].size(); j++)
		{
			if (CalDispath(PVerTemp, path[i][j], sdistemp, path[i]))
			{
				if (sdistemp < Sdist)
				{
					Sdist = sdistemp; PVer = PVerTemp; FirNub = j;
				}
			}
		}
		//标记所找到点的一阶邻域点
		if (Sdist!=100)
		{
			for (int j = 0; j < PVer->vec_OneFac.size(); j++)
			{
				for (int k = 0; k < 3;k++)
				{
					PVer->vec_OneFac[j]->m_PVerts[k]->bStatus = 1;
				}
			}
		}
		(Sdist == 100) ? (FpointTem.push_back(path[i][0])) : (FpointTem.push_back(PVer));//是否找到凹点
		//查找环的第二个点，在距离初始点1/2处查找
		int n1 = int(0.32 * ringSize) + FirNub;  Sdist = 100;
		if (n1 < ringSize)//情况一
		{
			if (CalDispath(PVerTemp, path[i][n1], sdistemp, path[i]))
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)//没找到第二凹点
			{
				FpointTem.push_back(path[i][n1]);
			}
			else
			{
				for (int j = 0; j < PVer->vec_OneFac.size(); j++)
				{
					for (int k = 0; k < 3; k++)
					{
						PVer->vec_OneFac[j]->m_PVerts[k]->bStatus = 1;
					}
				}
				FpointTem.push_back(PVer);
			}
		}
		else
		{
			int Nlet = n1 - ringSize;
			if (CalDispath(PVerTemp, path[i][Nlet], sdistemp, path[i]))//情况二
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)
			{
				FpointTem.push_back(path[i][Nlet]);
			}
			else
			{
				for (int j = 0; j < PVer->vec_OneFac.size(); j++)
				{
					for (int k = 0; k < 3; k++)
					{
						PVer->vec_OneFac[j]->m_PVerts[k]->bStatus = 1;
					}
				}
				FpointTem.push_back(PVer);
			}
		}
		///////////////寻找第三个点
		n1 = int(0.32 * ringSize) + n1;  Sdist = 100;
		if (n1 < ringSize)//情况一
		{
			if (0/*CalDispath(PVerTemp, path[i][n1], sdistemp, path[i])*/)
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)//没找到第二凹点
			{
				FpointTem.push_back(path[i][n1]);
			}
			else
			{
				FpointTem.push_back(PVer);
			}
		}
		else
		{
			int Nlet = n1 - ringSize;
			if (0/*CalDispath(PVerTemp, path[i][Nlet], sdistemp, path[i])*/)//情况二
			{
				if (sdistemp < Sdist&&PVerTemp != FpointTem.back())
				{
					Sdist = sdistemp; PVer = PVerTemp;
				}
			}
			if (Sdist == 100)
			{
				FpointTem.push_back(path[i][Nlet]);
			}
			else
			{
				FpointTem.push_back(PVer);
			}
		}
		CFpoint.push_back(FpointTem);
	}
	path.clear(); pathTrace.clear();
	for (int j = 0; j < m_vecPVert.size(); j++)//去除标记
	{
		m_vecPVert[j]->bStatus = 1;
	}
	for (int i = 0; i < CFpoint.size(); i++)
	{
		vector<Insepoint> pathTemp; pathTemp.clear();
		vector<Insepoint> vec_point;
		for (int nt = 0; nt < 3; nt++)
		{
			int nqt = (nt + 1) % 3;
			Insepoint pStart, pEnd;
			pStart.pTemp = *CFpoint[i][nt];//给出初始交点值
			pStart.InsEdge_Start = CFpoint[i][nt];//给出初始交点所在边的起点
			pStart.InsEdge_End = CFpoint[i][nt]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
			pStart.Epoint = 1;

			pEnd.pTemp = *CFpoint[i][nqt];//给出初始交点值
			pEnd.InsEdge_Start = CFpoint[i][nqt];//给出初始交点所在边的起点
			pEnd.InsEdge_End = CFpoint[i][nqt]->pHEdgeOut->pVertEnd;//给出初始交点所在边的终点
			pEnd.Epoint = 1;
			Insepoint pTemp = pStart;
			int  ntime;
			//求出选定两点上所有相交点
			bool nwrite = true;
			do
			{
				Insepoint Ipoint;
				//分三种情况，一种在面片内，一种是在边上，另外一种是在顶点上
				vec_point.clear();
				if (pTemp.Epoint)
				{
					VECTOR3D VertexNormal; PVERT Pverts;
					VertexNormal = pTemp.InsEdge_Start->VertexNormal;
					Pverts = pTemp.InsEdge_Start;
					VECTOR3D F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();

					vec_PFACETTRI OneRingface = Pverts->vec_OneFac; ntime = 0;
					for (int j = 0; j < OneRingface.size(); j++)
					{
						for (int k = 0; k < 3; k++)
						{
							int nct = (k + 1) % 3;
							Insepoint poit;
							poit.InsEdge_Start = OneRingface[j]->m_PVerts[k];
							poit.InsEdge_End = OneRingface[j]->m_PVerts[nct];

							if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint) && ntime < 2)
							{
								if (poit.pTemp != pTemp.pTemp)
								{
									poit.pFac = OneRingface[j];
									vec_point.push_back(poit);
									ntime++;
								}
							}
						}

					}
					if (ntime!=2)
					{
						int tt = 0;
					}
				}
				else
				{
					PVERT pvertemp; pvertemp = new VERT;
					pvertemp->x = pTemp.pTemp.x; pvertemp->y = pTemp.pTemp.y; pvertemp->z = pTemp.pTemp.z;
					VECTOR3D VertexNormal = PkiTool.CalcuVerNormal(pvertemp, pTemp.pFac);
					VECTOR3D F_normal;
					F_normal = VECTOR3D(pEnd.pTemp - pTemp.pTemp)*VertexNormal; F_normal.Normalize();
					delete pvertemp; ntime = 0;
					vec_PFACETTRI FaceN;
					if (pTemp.PVertAdj != NULL)
					{
						FaceN.push_back(PFACETTRI(pTemp.PVertAdj->pFacetAdj));
					}
					else
					{
						return false;
					}
					FaceN.push_back(pTemp.pFac);
					for (int j = 0; j < 2; j++)
					{
						for (int k = 0; k < 3; k++)
						{
							int nct = (k + 1) % 3;
							Insepoint poit;
							poit.InsEdge_Start = FaceN[j]->m_PVerts[k];
							poit.InsEdge_End = FaceN[j]->m_PVerts[nct];

							if (PkiTool.CalPlaneLineIntPoint(*poit.InsEdge_Start, *poit.InsEdge_End, pTemp.pTemp, F_normal, poit.pTemp, poit.Epoint) && ntime < 2)
							{
								if (poit.pTemp != pTemp.pTemp)
								{
									poit.pFac = FaceN[j];
									vec_point.push_back(poit);
									ntime++;
								}
							}
						}
					}
					if (ntime != 2)
					{
						int tt = 0;
					}
				}
				VECTOR3D v1, v2, v3;
				v1 = pEnd.pTemp - pTemp.pTemp; v1.Normalize();
				v2 = vec_point[0].pTemp - pTemp.pTemp; v2.Normalize();
				v3 = vec_point[1].pTemp - pTemp.pTemp; v3.Normalize();
				double angel_0, angel_1;
				angel_0 = v1 | v2; angel_1 = v1 | v3;
				(angel_0 < angel_1) ? Ipoint = vec_point[1] : Ipoint = vec_point[0];//选取正确的交点
				if (pTemp.Epoint)
				{
					pTemp.pFac = Ipoint.pFac;//保存当前求交面片
				}
				//////排除较小交点，防止产生小三角形
				vector<Insepoint> TwoinsertVert; TwoinsertVert.clear();
				TwoinsertVert.push_back(pTemp); TwoinsertVert.push_back(Ipoint);
				double L, L0, L1; vector<double> vec_L;
				for (int j = 0; j < 2; j++)
				{
					POINT3D ppStart, ppEnd;
					ppStart = *TwoinsertVert[j].InsEdge_Start; ppEnd = *TwoinsertVert[j].InsEdge_End;
					L = PkiTool.dis(ppStart, ppEnd);
					L0 = PkiTool.dis(ppStart, TwoinsertVert[j].pTemp); L1 = PkiTool.dis(ppEnd, TwoinsertVert[j].pTemp);
					(L0 < L1) ? (L = L0 / L) : (L = L1 / L); vec_L.push_back(L);
					(L0 < L1) ? (TwoinsertVert[j].NearVert = TwoinsertVert[j].InsEdge_Start) : (TwoinsertVert[j].NearVert = TwoinsertVert[j].InsEdge_Start);
				}
				pTemp.NearVert = TwoinsertVert[0].NearVert; Ipoint.NearVert = TwoinsertVert[1].NearVert;
				if (pTemp.NearVert == Ipoint.NearVert)
				{
					double Sa1, Sa2;
					Sa1 = PkiTool.AreaTri(pTemp.NearVert, pTemp.pTemp, Ipoint.pTemp);
					Sa2 = PkiTool.AreaTri(pTemp.pFac->m_PVerts[0], pTemp.pFac->m_PVerts[1], pTemp.pFac->m_PVerts[2]);
					if (/*Sa1 / Sa2<0.20&&*/vec_L[0]<0.20)
					{
						/*				PVERT FarVert;
						(pTemp.NearVert == pTemp.InsEdge_Start) ? FarVert = pTemp.InsEdge_End : FarVert = pTemp.InsEdge_Start;
						pTemp.pTemp.x = 0.65*pTemp.NearVert->x + 0.35*FarVert->x;
						pTemp.pTemp.y = 0.65*pTemp.NearVert->y + 0.35*FarVert->y;
						pTemp.pTemp.z = 0.65*pTemp.NearVert->z + 0.35*FarVert->z;*/
					}
				}
				pathTemp.push_back(pTemp);
				PHEDGE vec_PH[3];
				vec_PH[0] = pTemp.pFac->pHEdge; vec_PH[1] = vec_PH[0]->pHEdgeNext; vec_PH[2] = vec_PH[0]->pHEdgePre;
				bool ceshi = true;
				for (int j = 0; j < 3; j++)
				{
					if (vec_PH[j]->pHEdgePair != NULL)
					{
						if (vec_PH[j]->pVertEnd == Ipoint.InsEdge_End&&vec_PH[j]->pHEdgePre->pVertEnd == Ipoint.InsEdge_Start)
						{
							Ipoint.PVertAdj = vec_PH[j]; ceshi = false;
							Ipoint.pFac = PFACETTRI(vec_PH[j]->pHEdgePair->pFacetAdj); break;
						}
						if (vec_PH[j]->pVertEnd == Ipoint.InsEdge_Start&&vec_PH[j]->pHEdgePre->pVertEnd == Ipoint.InsEdge_End)
						{
							Ipoint.PVertAdj = vec_PH[j]; ceshi = false;
							Ipoint.pFac = PFACETTRI(vec_PH[j]->pHEdgePair->pFacetAdj); break;
						}
					}
					else
					{
						return false;
					}
				}
				if (ceshi)
				{
					int tt = 0;
					Insepoint Ipt;
					POINT3D p0,p1;
				}
				pTemp = Ipoint;//继续循环
				nwrite = false;
			} while (PkiTool.Comparepoint(pTemp.pFac, pEnd.pTemp));
		}
		vec_PVERT vec_InPoint;
		for (int j = 0; j < pathTemp.size(); j++)
		{
			///////////////将所有轨迹点保存到Path容器中
			PVERT pver = new VERT;
			pver->x = pathTemp[j].pTemp.x; pver->y = pathTemp[j].pTemp.y; pver->z = pathTemp[j].pTemp.z;
			vec_InPoint.push_back(pver);
		}
		pathTrace.push_back(pathTemp);
		path.push_back(vec_InPoint);
	}
	return true;
}
bool MeshBlock::CalDispath(PVERT& PVer, PVERT Rpoint, double &Sdist, vector<PVERT> path)
{
	vec_PVERT vec_Twopvert; vec_Twopvert.clear();
	KitTool::FindTwoRing_new(Rpoint, vec_Twopvert);//二阶邻域点，若是效果不太好可以求三阶邻域
	//KitTool::FindOneRing(Rpoint, vec_Twopvert);
	for (int i = 0; i < vec_Twopvert.size(); i++)
	{
		vec_PHEDGE vecpH; double distance = 100; PVERT pOneVert;
		KitTool::FindOnePH(vec_Twopvert[i], vecpH);
		for (int j = 0; j < vecpH.size(); j++)
		{
			if (!vecpH[j]->bStatus&&!vecpH[j]->pVertEnd->bStatus)
			{
				vecpH[j]->bStatus = 1;
				if (vecpH[j]->pHEdgePair != NULL)
				{
					FACETTRI f1 = *PFACETTRI(vecpH[j]->pFacetAdj);
					FACETTRI f2 = *PFACETTRI(vecpH[j]->pHEdgePair->pFacetAdj);
					float angle = acos(*f1.m_PFacetNorm | *f2.m_PFacetNorm);
					float threshold = PI / 8.0;
					if (angle > threshold)
					{
						for (int k = 0; k < path.size(); k++)
						{
							double disTemp = PkiTool.dis(*vecpH[j]->pVertEnd, *path[k]);
							if (distance>disTemp&&disTemp!=0)
							{
								distance = PkiTool.dis(*vecpH[j]->pVertEnd, *path[k]);
								pOneVert = vecpH[j]->pVertEnd;
							}
						}
					}
				}
			}
		}
		if (distance<Sdist)
		{
			Sdist = distance; PVer = pOneVert;
		}
	}
	if (Sdist==100)
	{
		return false;
	}
	return true;
}
bool MeshBlock::IfNeedLoop(vector<Insepoint> path)
{
	for (int i = 0; i < path.size(); i++)
	{
		if (!path[i].Epoint)
		{
			return false;
		}
	}
	return true;
}
void MeshBlock::Updatecutline(vector<vector<PVERT>> &path, vector<vector<Insepoint>>  pathTemp, vec_PVERT m_vecPVert)
{
	for (int i = 0; i < path.size(); i++)
	{
		if (IfNeedLoop(pathTemp[i]))
		{
			for (int j = 0; j < path[i].size();j++)
			{
				for (int k = 0; k < m_vecPVert.size(); k++)
				{
					if (*path[i][j] == *m_vecPVert[k])
					{
						m_vecPVert[k]->bused = 1;
						path[i][j] = m_vecPVert[k]; break;
					}
				}
			}
			
		}
	}
	
}
void MeshBlock::CutTraceProjection(vector<Insepoint> pathTemp, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert, STLModell* m_Tmesh)
{
	////////标记需处理当前圈的面片///////////////
	for (int i = 0; i < pathTemp.size(); i++)
	{
		PFACETTRI pf0=pathTemp[i].pFac;
		for (int j = 0; j < 3;j++)
		{
			int nct = (j + 1) % 3; int ncnt = (j + 2) % 3;
			for (int k = 0; k < m_Tmesh->m_vecPFacetTri.size(); k++)
			{
				PFACETTRI PFACT0= m_Tmesh->m_vecPFacetTri[k];
				for (int m = 0; m < 3;m++)
				{
					int nqt = (m + 1) % 3; int nqnt = (m + 2) % 3;
					if (pf0->m_PVerts[j]->x == PFACT0->m_PVerts[m]->x&&pf0->m_PVerts[j]->y == PFACT0->m_PVerts[m]->y
						&&pf0->m_PVerts[j]->z == PFACT0->m_PVerts[m]->z)
					{
						if (pf0->m_PVerts[nct]->x == PFACT0->m_PVerts[nqt]->x&&pf0->m_PVerts[nct]->y == PFACT0->m_PVerts[nqt]->y
							&&pf0->m_PVerts[nct]->z == PFACT0->m_PVerts[nqt]->z)
						{
							if (pf0->m_PVerts[ncnt]->x == PFACT0->m_PVerts[nqnt]->x&&pf0->m_PVerts[ncnt]->y == PFACT0->m_PVerts[nqnt]->y
								&&pf0->m_PVerts[ncnt]->z == PFACT0->m_PVerts[nqnt]->z)
							{
								PFACT0->becut = 1;
							}
						}
					}
				}
			}
		}
		
	}
	/*重点调试对象，用于分割数据光滑显示*/
	////////保存新的面片数据和顶点数据///////////////
	int VetxID = 0;
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	PkiTool.StoreFactandVer(m_Tmesh->m_vecPFacetTri, m_vecPFacetTri, m_vecPVert, map_Vert, iter, VetxID);
	//存储处理后分割线上的面片
	vec_PFACETTRI vec_face;
	map<VERT, int>map_VertTemp;
	map<VERT, int>::iterator iterTemp;
	for (int i = 0; i < pathTemp.size(); i++)
	{
		Insepoint Ipot1, Ipot2;
		(i == pathTemp.size() - 1) ? (Ipot1 = pathTemp[i], Ipot2 = pathTemp[0]) : (Ipot1 = pathTemp[i], Ipot2 = pathTemp[i + 1]);
		PFACETTRI PFat0, PFat1, PFat2;

		PVERT p0, p1, p2, p3, p4;
		p1 = new VERT; p2 = new VERT;
		p1->x = Ipot1.pTemp.x; p1->y = Ipot1.pTemp.y; p1->z = Ipot1.pTemp.z;
		p2->x = Ipot2.pTemp.x; p2->y = Ipot2.pTemp.y; p2->z = Ipot2.pTemp.z;
		p1->bused = 1; p2->bused = 1;
		/////////处理第一种情况////////////////////
		if ((Ipot1.Epoint&&!Ipot2.Epoint) || (!Ipot1.Epoint&&Ipot2.Epoint))
		{
			POINT3D ppint;
			/////////处理第一种情况////////////////////
			if (Ipot1.Epoint&&!Ipot2.Epoint)
			{
				p1->x = Ipot2.pTemp.x; p1->y = Ipot2.pTemp.y; p1->z = Ipot2.pTemp.z;
				ppint = Ipot1.pTemp;
			}
			else	/////////处理第四种情况////////////////////
			{
				p1->x = Ipot1.pTemp.x; p1->y = Ipot1.pTemp.y; p1->z = Ipot1.pTemp.z;
				ppint = Ipot2.pTemp;
			}
			p0 = new VERT; p3 = new VERT;
			for (int j = 0; j < 3; j++)
			{
				int nct, ncnt;
				nct = (j + 1) % 3; ncnt = (j + 2) % 3;
				if (Ipot1.pFac->m_PVerts[j]->x == ppint.x&&Ipot1.pFac->m_PVerts[j]->y == ppint.y
					&&Ipot1.pFac->m_PVerts[j]->z == ppint.z)
				{
					p2->x = ppint.x; p2->y = ppint.y; p2->z = ppint.z;
					p0->x = Ipot1.pFac->m_PVerts[nct]->x; p0->y = Ipot1.pFac->m_PVerts[nct]->y; p0->z = Ipot1.pFac->m_PVerts[nct]->z;
					p3->x = Ipot1.pFac->m_PVerts[ncnt]->x; p3->y = Ipot1.pFac->m_PVerts[ncnt]->y; p3->z = Ipot1.pFac->m_PVerts[ncnt]->z;
					break;
				}
			}
			PFat0 = new FACETTRI; PFat1 = new FACETTRI;
			if (Ipot1.Epoint&&!Ipot2.Epoint)
			{
				if (CalVetexNormal(p1, p2, p3, Ipot1.pFac->m_PFacetNorm))
				{
					PFat0->m_PVerts[0] = p1; PFat0->m_PVerts[1] = p2; PFat0->m_PVerts[2] = p3;
				}
				else
				{
					PFat0->m_PVerts[0] = p2; PFat0->m_PVerts[1] = p1; PFat0->m_PVerts[2] = p3;
				}
				if (CalVetexNormal(p1, p2, p0, Ipot1.pFac->m_PFacetNorm))
				{
					PFat1->m_PVerts[0] = p1; PFat1->m_PVerts[1] = p2; PFat1->m_PVerts[2] = p0;
				}
				else
				{
					PFat1->m_PVerts[0] = p2; PFat1->m_PVerts[1] = p1; PFat1->m_PVerts[2] = p0;
				}
			}
			else	/////////处理第四种情况////////////////////
			{
				if (CalVetexNormal(p1, p2, p0, Ipot1.pFac->m_PFacetNorm))
				{
					PFat0->m_PVerts[0] = p1; PFat0->m_PVerts[1] = p2; PFat0->m_PVerts[2] = p0;
				}
				else
				{
					PFat0->m_PVerts[0] = p2; PFat0->m_PVerts[1] = p1; PFat0->m_PVerts[2] = p0;
				}
				if (CalVetexNormal(p1, p2, p3, Ipot1.pFac->m_PFacetNorm))
				{
					PFat1->m_PVerts[0] = p1; PFat1->m_PVerts[1] = p2; PFat1->m_PVerts[2] = p3;
				}
				else
				{
					PFat1->m_PVerts[0] = p2; PFat1->m_PVerts[1] = p1; PFat1->m_PVerts[2] = p3;
				}
			}
			vec_face.push_back(PFat0); vec_face.push_back(PFat1);
		}
		/////////处理第二种情况////////////////////
		if (Ipot1.Epoint&&Ipot2.Epoint)
		{
			p0 = new VERT;
			for (int j = 0; j < 3; j++)
			{
				int nct = (j + 1) % 3; int nrt = (j + 2) % 3;
				vec_POINT3D vec_pint; vec_pint.push_back(Ipot1.pTemp); vec_pint.push_back(Ipot2.pTemp);
				for (int k = 0; k < 2; k++)
				{
					int ncnt = (k + 1) % 3;
					if (Ipot1.pFac->m_PVerts[j]->x == vec_pint[k].x&&Ipot1.pFac->m_PVerts[j]->y == vec_pint[k].y
						&&Ipot1.pFac->m_PVerts[j]->z == vec_pint[k].z)
					{
						if (Ipot1.pFac->m_PVerts[nct]->x == vec_pint[ncnt].x&&Ipot1.pFac->m_PVerts[nct]->y == vec_pint[ncnt].y
							&&Ipot1.pFac->m_PVerts[nct]->z == vec_pint[ncnt].z)
						{
							p0->x = Ipot1.pFac->m_PVerts[nrt]->x; p0->y = Ipot1.pFac->m_PVerts[nrt]->y; p0->z = Ipot1.pFac->m_PVerts[nrt]->z;
							p1->x = Ipot1.pFac->m_PVerts[j]->x; p1->y = Ipot1.pFac->m_PVerts[j]->y; p1->z = Ipot1.pFac->m_PVerts[j]->z;
							p2->x = Ipot1.pFac->m_PVerts[nct]->x; p2->y = Ipot1.pFac->m_PVerts[nct]->y; p2->z = Ipot1.pFac->m_PVerts[nct]->z;
						}
					}
				}
			}
			PFat0 = new FACETTRI;
			PFat0->m_PVerts[0] = p0; PFat0->m_PVerts[1] = p1; PFat0->m_PVerts[2] = p2;
			vec_face.push_back(PFat0);
		}
		/////////处理第三种情况////////////////////
		if (!Ipot1.Epoint&&!Ipot2.Epoint)
		{
			p0 = new VERT; p3 = new VERT; p4 = new VERT;
			PVERT pTempver0[2], pTempver1[2];
			pTempver0[0] = Ipot1.InsEdge_Start; pTempver0[1] = Ipot1.InsEdge_End;
			pTempver1[0] = Ipot2.InsEdge_Start; pTempver1[1] = Ipot2.InsEdge_End;
			for (int j = 0; j < 2; j++)//查找相交边公共顶点以及定位其他点
			{
				int nct = (j + 1) % 2; bool ifFind = false;
				for (int k = 0; k < 2; k++)//查找相交边公共顶点
				{
					int ncnt = (k + 1) % 2;
					if (pTempver0[j] == pTempver1[k])
					{
						p0->x = pTempver0[j]->x; p0->y = pTempver0[j]->y; p0->z = pTempver0[j]->z;
						p3->x = pTempver0[nct]->x; p3->y = pTempver0[nct]->y; p3->z = pTempver0[nct]->z;
						p4->x = pTempver1[ncnt]->x; p4->y = pTempver1[ncnt]->y; p4->z = pTempver1[ncnt]->z;
						ifFind = true;
						break;
					}
				}
				if (ifFind)
				{
					break;
				}
			}
			PFat0 = new FACETTRI; PFat1 = new FACETTRI; PFat2 = new FACETTRI;
			if (CalVetexNormal(p0, p1, p2, Ipot1.pFac->m_PFacetNorm))
			{
				PFat0->m_PVerts[0] = p0; PFat0->m_PVerts[1] = p1; PFat0->m_PVerts[2] = p2;
			}
			else
			{
				PFat0->m_PVerts[0] = p1; PFat0->m_PVerts[1] = p0; PFat0->m_PVerts[2] = p2;
			}
			//比较面片大小，去面片比例小的为新的面片方式
			double sFce0, sFce1, sface;
			auto stemp = [](PVERT pPoint1, PVERT pPoint2, PVERT pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
			sFce0 = stemp(p1, p2, p3); sFce1 = stemp(p2, p3, p4);
			//sFce0 = PkiTool.AreaTri(p1, p2, p3); sFce1 = PkiTool.AreaTri(p2, p3, p4);
			sFce0 > sFce1 ? (sface = sFce0 / sFce1) : (sface = sFce1 / sFce0);

			sFce0 = stemp(p1, p2, p4); sFce1 = stemp(p1, p3, p4);
			//sFce0 = PkiTool.AreaTri(p1, p2, p4); sFce1 = PkiTool.AreaTri(p1, p3, p4);
			sFce0 > sFce1 ? (sFce0 = sFce0 / sFce1) : (sFce0 = sFce1 / sFce0);
			VECTOR3D v1, v2; double angle;
			if (sface > sFce0)
			{
				if (CalVetexNormal(p1, p2, p4, Ipot1.pFac->m_PFacetNorm))
				{
					PFat1->m_PVerts[0] = p1; PFat1->m_PVerts[1] = p2; PFat1->m_PVerts[2] = p4;
				}
				else
				{
					PFat1->m_PVerts[0] = p2; PFat1->m_PVerts[1] = p1; PFat1->m_PVerts[2] = p4;
				}
				if (CalVetexNormal(p1, p3, p4, Ipot1.pFac->m_PFacetNorm))
				{
					PFat2->m_PVerts[0] = p1; PFat2->m_PVerts[1] = p3; PFat2->m_PVerts[2] = p4;
				}
				else
				{
					PFat2->m_PVerts[0] = p3; PFat2->m_PVerts[1] = p1; PFat2->m_PVerts[2] = p4;
				}
			}
			else
			{
				if (CalVetexNormal(p2, p4, p3, Ipot1.pFac->m_PFacetNorm))
				{
					PFat1->m_PVerts[0] = p2; PFat1->m_PVerts[1] = p4; PFat1->m_PVerts[2] = p3;
				}
				else
				{
					PFat1->m_PVerts[0] = p4; PFat1->m_PVerts[1] = p2; PFat1->m_PVerts[2] = p3;
				}
				if (CalVetexNormal(p1, p2, p3, Ipot1.pFac->m_PFacetNorm))
				{
					PFat2->m_PVerts[0] = p1; PFat2->m_PVerts[1] = p2; PFat2->m_PVerts[2] = p3;
				}
				else
				{
					PFat2->m_PVerts[0] = p2; PFat2->m_PVerts[1] = p1; PFat2->m_PVerts[2] = p3;
				}
			}
			vec_face.push_back(PFat0); vec_face.push_back(PFat1); vec_face.push_back(PFat2);
		}
	}
	PkiTool.StoreFactandVer(vec_face, m_vecPFacetTri, m_vecPVert, map_Vert, iter, VetxID);
	
	//清除点表和面片，释放内存
	vec_PVERT vec_point;
	for (int i = 0; i < vec_face.size(); i++)
	{
		PFACETTRI Pf0 = vec_face[i];
		for (int j = 0; j < 3; j++)
		{
			iterTemp = map_VertTemp.find(*Pf0->m_PVerts[j]);
			if (iterTemp == map_VertTemp.end())//不存在该点
			{
				vec_point.push_back(Pf0->m_PVerts[j]);
				map_VertTemp.insert(pair<VERT, int>(*Pf0->m_PVerts[j], vec_point.size()));
			}
		}
	}
	for (int i = 0; i < vec_face.size(); i++)
	{
		PFACETTRI Pf0 = vec_face[i];
		delete Pf0;
	}
	for (int i = 0; i < vec_point.size(); i++)
	{
		delete vec_point[i];
	}
	vec_face.clear(); map_VertTemp.clear(); vec_point.clear();
}
bool MeshBlock::CalVetexNormal(PVERT p0, PVERT p1, PVERT p2, PVECTOR3D FaceNormal)
{
	VECTOR3D v1, v2; double angel;
	v1 = *PkiTool.CalcuPfacNormal(p0, p1, p2); v1.GetNormal();
	angel = *FaceNormal | v1;
	if (angel > 0.5)
	{
		return true;
	}
	else
	{
		return false;
	}
	return true;
}
void MeshBlock::PickPoint(vector<POINT3D> pt, vector<PVERT>&path, STLModell* m_Tmesh)
{
	if (m_Tmesh == NULL)
	{
		return;
	}
	if (m_Tmesh->m_Tmesh_KD != NULL)  m_Tmesh->m_Tmesh_KD = NULL;
	vector<point> vec_p; vec_p.clear();
	for (int i = 0; i < m_Tmesh->m_vecPVert.size(); i++)
	{
		point vre(m_Tmesh->m_vecPVert[i]->x, m_Tmesh->m_vecPVert[i]->y, m_Tmesh->m_vecPVert[i]->z);
		vec_p.push_back(vre);
	}

	float* v0 = &vec_p[0][0];
	m_Tmesh->m_Tmesh_KD = new KDtree(v0, m_Tmesh->m_vecPVert.size());

	double juli = PkiTool.dis(*(m_Tmesh->m_vecPFacetTri.back()->m_PVerts[0]), *(m_Tmesh->m_vecPFacetTri.back()->m_PVerts[1]));
	pcedie.m_Point_BeginEnd.clear();
	for (int i = 0; i < pt.size(); i++)
	{
		int vindex = -1;
		vec p1 = vec(pt[i].x, pt[i].y, pt[i].z);
		const float *Match = m_Tmesh->m_Tmesh_KD->closest_to_pt(p1, juli * 10);
		if (Match)
		{
			vindex = (Match - v0) / 3;
			pcedie.m_Point_BeginEnd.push_back(vindex);
		}
		//m_Path_Point.clear();
	}
	if (pcedie.m_Point_BeginEnd.size() < 2)return;
	vector<int>pathTemp;
	for (int i = 0; i < pcedie.m_Point_BeginEnd.size(); i++)
	{
		pathTemp.clear();
		if (i == pt.size() - 1)
		{
			pcedie.Propagate(m_Tmesh, pcedie.m_Point_BeginEnd[i], pcedie.m_Point_BeginEnd[0], pathTemp);
		}
		else
		{
			pcedie.Propagate(m_Tmesh, pcedie.m_Point_BeginEnd[i], pcedie.m_Point_BeginEnd[i + 1], pathTemp);
		}
		int n = pathTemp.size();
		for (int j = n - 1; j >= 0; j--)               //因为path内存点的顺序是从目标点到源点的，需要倒过来
		{
			if (j == 0)
			{
				if (i == pcedie.m_Point_BeginEnd.size() - 1)
				{
					m_Tmesh->m_vecPVert[pathTemp[0]]->bused = 1;
					path.push_back(m_Tmesh->m_vecPVert[pathTemp[0]]);
				}
			}
			else
			{
				m_Tmesh->m_vecPVert[pathTemp[j]]->bused = 1;
				path.push_back(m_Tmesh->m_vecPVert[pathTemp[j]]);
			}
		}
	}
}
void MeshBlock::Getreadblock(vec_PFACETTRI& m_vecPFacetTri, vector<vector<int>> All_PairBlock, vector<MyBlock> &Rbox)
{
	int nct = m_vecPFacetTri.size();
	int Blocknumber = m_vecPFacetTri[nct - 1]->Nblock;
	for (int i = 0; i < m_vecPFacetTri.size();i++)
	{
		if (Blocknumber<m_vecPFacetTri[i]->Nblock)
		{
			Blocknumber = m_vecPFacetTri[i]->Nblock;
		}
	}
	Blocknumber += 1;
	for (int i = 0; i < Blocknumber; i++)
	{
		MyBlock BlockTemp;
		vec_PFACETTRI tempFace;
		for (int j = 0; j < nct; j++)
		{
			PFACETTRI Pf0 = m_vecPFacetTri[j];

			if (Pf0->Nblock == i&&Pf0->beused == 0)
			{
				tempFace.push_back(Pf0);
				Pf0->beused = 1;
			}
			if (Pf0->Nblock == i + 1)     break;
		}
		BlockTemp.OneBlock = tempFace;
		Rbox.push_back(BlockTemp);
	}
	for (int i = 0; i < All_PairBlock.size();i++)//读取拓扑关系
	{
		if (All_PairBlock[i][0] == -1)  return;//分割存储错误
	
		Rbox[i].vec_PairBlock = All_PairBlock[i];
	}

	for (int i = 0; i < m_vecPFacetTri.size();i++)
	{
		if (m_vecPFacetTri[i]->beused)
		{
			m_vecPFacetTri[i]->beused = 0;
		}
	}
	
	for (int i = 0; i<All_PairBlock.size(); i++)
	{
		for (int j = 0; j < All_PairBlock[i].size();j++)
		{
			Rbox[i].OneBlockPair.push_back(Rbox[All_PairBlock[i][j]]);
		}
	}
}
void MeshBlock::DrawPlat(drawTool* pDC, vector<vec_POINT3D> PlatFace, vector<vec_POINT3D> p_Row)
{
	COLORREF old_clr;
	pDC->GetMaterialColor(old_clr);
	pDC->SetMaterialColor(RGB(255, 255, 255));
	pDC->SetColor(RGB(255, 255, 255));
	for (int i = 0; i < p_Row.size(); i++)
	{
		pDC->DrawLine(p_Row[i][0], p_Row[i][p_Row[i].size() - 1]);
	}
	for (int i = 0; i < p_Row.size(); i++)
	{
		pDC->DrawLine(p_Row[0][i], p_Row[p_Row.size()-1][i]);
	}
	pDC->SetMaterialColor(old_clr);

	VECTOR3D V_Normal;
	for (int i = 0; i < PlatFace.size(); i++)
	{
		vec_POINT3D vec_Face = PlatFace[i];
		pDC->SetMaterialColor(RGB(87, 96, 105));
		pDC->SetColor(RGB(87, 96, 105));
		/*pDC->SetMaterialColor(RGB((47 * (i + 1)) % 255, (183 * i) % 255, (197 * i) % 255));
		pDC->SetColor(RGB((47 * (i + 1)) % 255, (183 * i) % 255, (197 * i) % 255));*/
		V_Normal = KitTool::CalcuPfacNormal(vec_Face[0], vec_Face[1], vec_Face[2]);
		pDC->DrawTriChip(V_Normal.dx, V_Normal.dy, V_Normal.dz,
			vec_Face[0].x, vec_Face[0].y, vec_Face[0].z,
			vec_Face[1].x, vec_Face[1].y, vec_Face[1].z,
			vec_Face[2].x, vec_Face[2].y, vec_Face[2].z);
		pDC->SetMaterialColor(old_clr);
	}
}
void MeshBlock::showfeatureEdge(drawTool *pDC)
{
	COLORREF old;
	pDC->GetColor(old);
	pDC->GetMaterialColor(old);
	pDC->SetMaterialColor(RGB(0, 255, 0));
	for (int i = 0; i < pSimply->edgepoint.size(); i++)
	{
		POINT3D p1, p2;

		if (i == pSimply->edgepoint.size() - 1)
		{
			p1 = pSimply->edgepoint[i];
			p2 = pSimply->edgepoint[0];

			pDC->DrawLine(p1, p2);
		}
		else
		{
			p1 = pSimply->edgepoint[i];
			p2 = pSimply->edgepoint[i + 1];
			pDC->DrawLine(p1, p2);
			pDC->DrawSphere(p1, 0.5);
		}
	}
	pDC->SetMaterialColor(old);
}
void MeshBlock::featurPOINT(vec_PFACETTRI& m_vecPFacetTri, vec_PHEDGE& m_vecPHEdge, vec_PVERT& m_vecPVert, double &NVlimit)
{
	vector<double> potList;
	double theMAX, theMIN;
	theMAX = 0; theMIN = 999;
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		m_vecPVert[i]->bused = 0;
		m_vecPVert[i]->bStatus = 0;
	}
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		double NV = 0;
		if (!m_vecPVert[i]->cdrgee)
		{
			m_vecPVert[i]->cdrgee = PkiTool.CalcuPointCurve(m_vecPVert[i]);
		}
		NV = m_vecPVert[i]->cdrgee;
		//////////////////
		potList.push_back(NV);
		theMAX = theMAX + NV;
	}
	double MAMI = theMAX / m_vecPVert.size();
	double SIGEMA = 0;
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		SIGEMA = SIGEMA + (potList[i] - MAMI)*(potList[i] - MAMI) / m_vecPVert.size();
	}
	SIGEMA = sqrt(SIGEMA);
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		double NV = 0;
		NV = potList[i];
		NV = abs((NV - MAMI) / SIGEMA);
		potList[i] = NV;
		POINT3D ptemp;
		if (NV > NVlimit)
		{
			//ptemp.x = m_vecPVert[i]->x; ptemp.y = m_vecPVert[i]->y; ptemp.z = m_vecPVert[i]->z;
			//m_vecpcut.push_back(ptemp);
			m_vecPVert[i]->bused = 1;
		}
	}
}

void MeshBlock::Render(drawTool* pDC, vector<PVERT> m_Path_Point)
{
	if (m_Path_Point.size() < 1) return;
	COLORREF old;
	pDC->GetColor(old);
	pDC->GetMaterialColor(old);
	pDC->SetMaterialColor(RGB(0, 255, 0));
	if (m_Path_Point.size() > 1)
	{
		POINT3D ver1, ver2;
		for (int i = 0; i < m_Path_Point.size(); i++)
		{
			if (i==m_Path_Point.size() - 1)
			{
				ver1 = POINT3D(m_Path_Point[i]->x, m_Path_Point[i]->y, m_Path_Point[i]->z);
				ver2 = POINT3D(m_Path_Point[0]->x, m_Path_Point[0]->y, m_Path_Point[0]->z);
			}
			else
			{
				ver1 = POINT3D(m_Path_Point[i]->x, m_Path_Point[i]->y, m_Path_Point[i]->z);
				ver2 = POINT3D(m_Path_Point[i + 1]->x, m_Path_Point[i + 1]->y, m_Path_Point[i + 1]->z);
			}
			glLineWidth(5);
			pDC->DrawLine(ver1, ver2);
			glLineWidth(1.0f);
		}
		glLineWidth(1.0f);
	}
	pDC->SetMaterialColor(old);
}

void MeshBlock::DrawPoint(drawTool* pDC, vector<vec_PVERT> CFpoint)
{
	if (CFpoint.size() < 1) return;
	for (int i = 0; i < CFpoint.size(); i++)
	{
		for (int j = 0; j < CFpoint[i].size(); j++)
		{
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetMaterialColor(RGB(255, 0, 0));
			pDC->DrawSphere(*CFpoint[i][j], 2);
			pDC->SetMaterialColor(old);
		}
	}
}
void MeshBlock::DrawFeapoint(drawTool* pDC, vector<vec_PVERT> FeaPointAll)
{
	if (FeaPointAll.size() < 1) return;
	for (int i = 0; i < FeaPointAll.size(); i++)
	{
		for (int j = 0; j < FeaPointAll[i].size(); j++)
		{
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetMaterialColor(RGB(255, 0, 0));
			pDC->DrawSphere(*FeaPointAll[i][j], 0.5);
			pDC->SetMaterialColor(old);
		}
	}
}
void MeshBlock::Drawblock(drawTool* pDC, vector<MyBlock> &Rbox)
{
	COLORREF old_clr;
	for (int i = 0; i < Rbox.size(); i++)
	{
		pDC->GetMaterialColor(old_clr);
		pDC->SetMaterialColor(RGB((47 * (i + 1)) % 255, (183 * i) % 255, (197 * i) % 255));
		pDC->SetColor(RGB((47 * (i + 1)) % 255, (183 * i) % 255, (197 * i) % 255));
		for (int j = 0; j < Rbox[i].OneBlock.size(); j++)
		{
			pDC->DrawTriChip(Rbox[i].OneBlock[j]->m_PFacetNorm->dx, Rbox[i].OneBlock[j]->m_PFacetNorm->dy, Rbox[i].OneBlock[j]->m_PFacetNorm->dz,
				Rbox[i].OneBlock[j]->m_PVerts[0]->x, Rbox[i].OneBlock[j]->m_PVerts[0]->y, Rbox[i].OneBlock[j]->m_PVerts[0]->z,
				Rbox[i].OneBlock[j]->m_PVerts[1]->x, Rbox[i].OneBlock[j]->m_PVerts[1]->y, Rbox[i].OneBlock[j]->m_PVerts[1]->z,
				Rbox[i].OneBlock[j]->m_PVerts[2]->x, Rbox[i].OneBlock[j]->m_PVerts[2]->y, Rbox[i].OneBlock[j]->m_PVerts[2]->z);
		}
		pDC->SetMaterialColor(old_clr);
	}
}
void MeshBlock::Drawblock(drawTool* pDC, COLORREF newcolor, vec_PFACETTRI& m_vecPFacetTri)
{
	COLORREF old_clr;
	for (int i = 0; i < m_vecPFacetTri.size(); i++)
	{
		pDC->GetMaterialColor(old_clr);
		pDC->SetMaterialColor(newcolor);
		pDC->SetColor(newcolor);
		for (int j = 0; j <m_vecPFacetTri.size(); j++)
		{
			pDC->DrawTriChip(m_vecPFacetTri[j]->m_PFacetNorm->dx, m_vecPFacetTri[j]->m_PFacetNorm->dy, m_vecPFacetTri[j]->m_PFacetNorm->dz,
				m_vecPFacetTri[j]->m_PVerts[0]->x, m_vecPFacetTri[j]->m_PVerts[0]->y, m_vecPFacetTri[j]->m_PVerts[0]->z,
				m_vecPFacetTri[j]->m_PVerts[1]->x, m_vecPFacetTri[j]->m_PVerts[1]->y, m_vecPFacetTri[j]->m_PVerts[1]->z,
				m_vecPFacetTri[j]->m_PVerts[2]->x, m_vecPFacetTri[j]->m_PVerts[2]->y, m_vecPFacetTri[j]->m_PVerts[2]->z);
		}
		pDC->SetMaterialColor(old_clr);
	}
}
void MeshBlock::showEdge(drawTool *pDC, vec_EDGE vec_Edge)
{
	COLORREF old;
	pDC->GetColor(old);
	pDC->GetMaterialColor(old);
	pDC->SetMaterialColor(RGB(0, 255, 0));
	for (int i = 0; i < vec_Edge.size(); i++)
	{
		POINT3D p1, p2;
		p1 = POINT3D(vec_Edge[i].pVertStart->x, vec_Edge[i].pVertStart->y, vec_Edge[i].pVertStart->z);
		p2 = POINT3D(vec_Edge[i].pVertEnd->x, vec_Edge[i].pVertEnd->y, vec_Edge[i].pVertEnd->z);
		pDC->DrawLine(p1, p2);
	}
	pDC->SetMaterialColor(old);
}
void MeshBlock::DrawSwichBlock(drawTool *pDC,int nBlock, vector<MyBlock> &Rbox)
{
	COLORREF old_clr;
	pDC->GetMaterialColor(old_clr);
	pDC->SetMaterialColor(RGB((47 * (nBlock + 1)) % 255, (183 * nBlock) % 255, (197 * nBlock) % 255));
	pDC->SetColor(RGB((47 * (nBlock + 1)) % 255, (183 * nBlock) % 255, (197 * nBlock) % 255));
	for (int j = 0; j < Rbox[nBlock].OneBlock.size(); j++)
	{
		pDC->DrawTriChip(Rbox[nBlock].OneBlock[j]->m_PFacetNorm->dx, Rbox[nBlock].OneBlock[j]->m_PFacetNorm->dy, Rbox[nBlock].OneBlock[j]->m_PFacetNorm->dz,
			Rbox[nBlock].OneBlock[j]->m_PVerts[0]->x, Rbox[nBlock].OneBlock[j]->m_PVerts[0]->y, Rbox[nBlock].OneBlock[j]->m_PVerts[0]->z,
			Rbox[nBlock].OneBlock[j]->m_PVerts[1]->x, Rbox[nBlock].OneBlock[j]->m_PVerts[1]->y, Rbox[nBlock].OneBlock[j]->m_PVerts[1]->z,
			Rbox[nBlock].OneBlock[j]->m_PVerts[2]->x, Rbox[nBlock].OneBlock[j]->m_PVerts[2]->y, Rbox[nBlock].OneBlock[j]->m_PVerts[2]->z);
	}
	pDC->SetMaterialColor(old_clr);
}
