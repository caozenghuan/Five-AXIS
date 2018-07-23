#include "CSimplify.h"


CSimplify::CSimplify()
{
	Kit = new KitTool;
	Kit = 0;
	featureTime = 0;
	TriangleTime = 0;
}


CSimplify::~CSimplify()
{
	if (Kit)
	{
		delete Kit;
		Kit = NULL;
	}
}

void CSimplify::EvaluatemeshQuality(STLModell *pMeshTri, double &neq)
{
	for (int i = 0; i < pMeshTri->m_vecPFacetTri.size();i++)
	{
		FACETTRI Pf0 = *pMeshTri->m_vecPFacetTri[i];
		double Are, L1, L2, L3; 
		VECTOR3D V1 = *Pf0.m_PVerts[0] - *Pf0.m_PVerts[1];
		VECTOR3D V2 = *Pf0.m_PVerts[0] - *Pf0.m_PVerts[2];
		VECTOR3D V3 = *Pf0.m_PVerts[1] - *Pf0.m_PVerts[2];
		auto stemp = [](PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
		Are += stemp(Pf0.m_PVerts[0], Pf0.m_PVerts[1], Pf0.m_PVerts[2]);
		//Are = Kit->AreaTri(Pf0.m_PVerts[0], Pf0.m_PVerts[1], Pf0.m_PVerts[2]);
		L1 = V1.GetLength();
		L2 = V2.GetLength();
		L3 = V3.GetLength();
		Are= 4 * sqrt(3.0)*Are/(L1*L1 + L2*L2 + L3*L3);
		if (Are>0.4)
		{
			neq++;
		}
	/*	neq += Are;*/
	}
	//neq = neq / pMeshTri->m_vecPFacetTri.size();
}

void CSimplify::GetsimpleResult(STLModell *pMeshTri, STLModell *pSimple, int simpway, int R,int ratepercent, QProgressBar  *progressBar)
{
	progressBar->setRange(-1, R - 1);
	progressBar->setValue(-1);

	STLModell *pSimpleTemp; 

	//pSimpleTemp = STLModell::GetInstance(); 
	pSimpleTemp = new STLModell();
	int Onetime = 0;
	double t0 = (double)clock(); 
	BOOL Edgeboardway=TRUE;
	(simpway == 1) ? (Edgeboardway=Markboundary(pMeshTri, pSimpleTemp)) : (Kit->StoretheMeshTri(pMeshTri, pSimpleTemp, Onetime));//是否做边界简化
	if (!Edgeboardway)
	{
		Kit->StoretheMeshTri(pMeshTri, pSimpleTemp, Onetime);
	}
	for (int i = 0; i < R; i++)
	{
		Onetime = i;
		double rate = 70/*80-i*3*/;//每次简化率80%
		progressBar->setValue(i);
		Kit->ClearTheList(pSimple);
		STLModell::CreateEdgeTopByFctVert(pSimpleTemp);
		switch (simpway)
		{
		case EDGESUB:
		{
			EdgesimpleResult(pSimpleTemp, pSimple, rate); break;
		}	
		case ME:
		{
			TrianglesimpleResultMe(pSimpleTemp, pSimple, rate, Onetime); break;
		}
		case SMOOTH:
		{
			TrianglesimplesmoothResult(pSimpleTemp, pSimple, rate); break;
		}
		case ZHANGXING:
		{
			TrianglesimpleResult(pSimpleTemp, pSimple, rate); break;
		}
		}
		featureTime += featureTimeTemp;
		TriangleTime += TriangleTimeTemp;
		int rate1 = pSimpleTemp->m_vecPFacetTri.size() -pSimple->m_vecPFacetTri.size();
		float ratetemp = 100 - pSimple->m_vecPFacetTri.size()*100.0 / pMeshTri->m_vecPFacetTri.size();
		if (rate1<300 || i == R - 1||ratetemp>ratepercent)
		{
			for (int j = 0; j < pSimple->m_vecPFacetTri.size();j++)
			{
				pSimple->m_vecPFacetTri[j]->m_PFacetNorm = Kit->CalcuPfacNormal(pSimple->m_vecPFacetTri[j]->m_PVerts[0], pSimple->m_vecPFacetTri[j]->m_PVerts[1], pSimple->m_vecPFacetTri[j]->m_PVerts[2]);///求面片的法矢
			}
			STLModell::CreateEdgeTopByFctVert(pSimple);
			Kit->ClearTheList(pSimpleTemp);
			progressBar->setValue(R - 1);
			break;
		}
		Kit->ClearTheList(pSimpleTemp);
		Kit->StoretheMeshTri(pSimple, pSimpleTemp,Onetime);
	}
}
void CSimplify::TrianglesimpleResultMe(STLModell *pMeshTri, STLModell *pSimple, float R, int Onetime)
{
	//标记所有边界三角形
	if (Onetime)
	{
		for (int i = 0; i < pMeshTri->m_vecPEdge.size();i++)
		{
			PHEDGE Ph = pMeshTri->m_vecPEdge[i]->pHEdgeAdj;
			if (!Ph->pHEdgePair)
			{
				PFACETTRI(Ph->pFacetAdj)->beboard = 1;
			}
		}
	}
	map<VERT, int>map_Vert; int VetxID = 0;
	map<VERT, int>::iterator iter;
	CTimer t;
	//double t0 = (double)clock();
	if (Onetime<2)
	{
		MarkparticularArea(pMeshTri, pSimple, map_Vert, iter, VetxID);//对特征边进行边折叠
	}
	vector<PFACETTRI> vec_Temp = pMeshTri->m_vecPFacetTri;
	featureTimeTemp = t.elapsed_micro();
	t.reset();
	//double t1 = (double)clock();  featureTimeTemp = (t1 - t0) / 1000;
	//double t2 = (double)clock(); double t01 = (t2 - t1) / 1000;
	CalvalueAndpoint(vec_Temp);
	TriangleTimeTemp = t.elapsed_micro();
	//double t3 = (double)clock(); TriangleTimeTemp = (t3 - t2) / 1000;
	sort(vec_Temp.begin(), vec_Temp.end(), Kit->RankPfact1);//按照折叠误差大小对面片进行排序
	int n = int(R / 100.0 * (vec_Temp.size() )) ; 
	for (int i = 0; i < n; i++)
	{
		if (!vec_Temp[i]->beused&&vec_Temp[i]->Pvet&&!vec_Temp[i]->beboard)
		{
			BOOL  legal;
			legal = Judgelegal(vec_Temp[i]);//合法性检查，防止折叠后面片法矢发生过大翻转
			if (!legal)   { continue; }

			PHEDGE Phedg[3];
			Phedg[0] = vec_Temp[i]->pHEdge; Phedg[1] = vec_Temp[i]->pHEdge->pHEdgePre; Phedg[2] = vec_Temp[i]->pHEdge->pHEdgeNext;
			for (int r = 0; r < 3; r++)//对折叠三角面片的邻接三角形进行标记
			{
				if (Phedg[r]->pHEdgePair != NULL)
				{
					PFACETTRI(Phedg[r]->pHEdgePair->pFacetAdj)->beused = 1;
				}
			}
			//////////////////对三角形进行折叠操作////////////////////
			PVERT p2 = new VERT;
			vector<PFACETTRI> vecper = vec_Temp[i]->vec_FOneFac;
			p2->x = vec_Temp[i]->Pvet->x; p2->y = vec_Temp[i]->Pvet->y; p2->z = vec_Temp[i]->Pvet->z;
			delete vec_Temp[i]->Pvet;
			for (int j = 0; j < vecper.size(); j++)
			{
				PFACETTRI PFact; PFact = new FACETTRI;
				for (int k = 0; k < 3; k++)
				{
					for (int r = 0; r < 3; r++)
					{
						int ncnt = (r + 1) % 3, nct = (r + 2) % 3;
						if (vec_Temp[i]->m_PVerts[k] == vecper[j]->m_PVerts[r])
						{
							PVERT p0, p1; p0 = new VERT; p1 = new VERT;
							p0->x = vecper[j]->m_PVerts[ncnt]->x; p0->y = vecper[j]->m_PVerts[ncnt]->y; p0->z = vecper[j]->m_PVerts[ncnt]->z;
							p1->x = vecper[j]->m_PVerts[nct]->x; p1->y = vecper[j]->m_PVerts[nct]->y; p1->z = vecper[j]->m_PVerts[nct]->z;

							PVERT POint[3];  POint[0] = p0; POint[1] = p1; POint[2] = p2;
							for (int nt = 0; nt < 3; nt++)
							{
								iter = map_Vert.find(*POint[nt]);
								if (iter != map_Vert.end())//已存在该点
								{
									int Num = iter->second;
									pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFact);//构建一阶邻域
									PFact->m_PVerts[nt] = pSimple->m_vecPVert[Num];
								}
								else
								{
									POint[nt]->VertexID = VetxID;
									POint[nt]->vec_OneFac.push_back(PFact);//构建一阶邻域
									map_Vert.insert(pair<VERT, int>(*POint[nt], pSimple->m_vecPVert.size()));
									pSimple->m_vecPVert.push_back(POint[nt]);
									PFact->m_PVerts[nt] = POint[nt];
									VetxID++;
								}
							}
						}
					}
				}
				vecper[j]->beused = 1;
				if (PFact->m_PVerts[0] != NULL&& PFact->m_PVerts[1] != NULL && PFact->m_PVerts[2] != NULL)
				{
					pSimple->m_vecPFacetTri.push_back(PFact);
				}
			}
			vec_Temp[i]->beused = 1;
		}
	}
	PushUntreatedFace(vec_Temp, pSimple, map_Vert, iter, VetxID, Onetime);//处理没有被简化的面片
	vec_Temp.clear();
}

void CSimplify::CalvalueAndpoint(vector<PFACETTRI> vecPfact)
{
    //#pragma omp parallel for /*private(i)*/
	for (int i = 0; i < vecPfact.size(); i++)
	{
		PFACETTRI Pfact = vecPfact[i];
		vec_PFACETTRI vecper; BOOL MarkRing=FALSE;
		Kit->FindPOneRFAC(Pfact, vecper);
		for (int j = 0; j < vecper.size();j++)
		{
			if (vecper[j]->befrost || vecper[j]->beboard||vecper[j]->beused)
			{
				if (vecper[j]->beboard)
				{
					PHEDGE vec_ph[3]; vec_ph[0] = vecper[j]->pHEdge;
					vec_ph[1] = vec_ph[0]->pHEdgePre; vec_ph[2] = vec_ph[0]->pHEdgeNext;
					for (int k = 0; k < 3;k++)
					{
						if (vec_ph[k]->pHEdgePair==NULL)
						{
							PVERT myvert[2];
							myvert[0] = vec_ph[k]->pVertEnd; myvert[0] = vec_ph[k]->pHEdgePre->pVertEnd;
							for (int m = 0; m < 2;m++)
							{
								for (int n = 0; n < 3;n++)
								{
									if (myvert[m] == Pfact->m_PVerts[n])
									{
										MarkRing = TRUE; break;//查找与待折叠三角面片共顶点的半边中有无边界半边
									}
								}
								if (MarkRing)
								{
									break;
								}
							}
						}
						if (MarkRing)
						{
							break;
						}
					}
					if (MarkRing)
					{
						break;
					}
				}
				if (vecper[j]->befrost || vecper[j]->beused)
				{
					MarkRing = TRUE; break;
				}
			}
			
		}

		if (!Pfact->befrost&&!Pfact->beboard&&!Pfact->beused&&!MarkRing)
		{
			//AcqnewVertex_Oneofthree(Pfact, vecper);//三点选一法
			//AcqnewVertex_Gravity(Pfact);//重心点作为新顶点
			AcqnewVertex_curdegree(Pfact);//曲度插值

			CalValue_pointplane(Pfact,vecper);
			//CalValue_Wmsa(Pfact);
			//CalValue_Fitcurve(Pfact);
			//CalValue_Fitplane(Pfact);
			//CalValue_QEM(Pfact, vecper);
		}
	}
}
void CSimplify::MarkparticularArea(STLModell *pMeshTri, STLModell *pSimple, map<VERT, int> &map_Vert,
	map<VERT, int>::iterator &iter, int &VetxID)
{
	for (int i = 0; i< pMeshTri->m_vecPEdge.size(); i++)
	{
		//////////////////SOD搜寻算法寻找特征边///////////////////////
		PHEDGE Ph = pMeshTri->m_vecPEdge[i]->pHEdgeAdj;
		if (Ph->pHEdgePair != NULL)
		{
			vector<PFACETTRI> vecper; vecper.clear();
			vec_PVERT vecpVer;
			bool inmark = false;
			Kit->FindedgeOnering(Ph, vecper);
			Kit->FindedgeOneringvertex_Me(Ph, vecpVer);
			if (vecper.size() > vecpVer.size())
			{
				continue;
			}
			vecpVer.clear();
			for (int j = 0; j < vecper.size(); j++)
			{
				if (vecper[j]->beused || vecper[j]->beboard)
				{
					inmark = true; break;
				}
			}
			if (!inmark)
			{
				VECTOR3D m_vNorm1, m_vNorm2;
				m_vNorm1 = *Ph->pFacetAdj->m_PFacetNorm;
				m_vNorm2 = *Ph->pHEdgePair->pFacetAdj->m_PFacetNorm;
				double angel = acos(m_vNorm1 | m_vNorm2);
				//通过二面角法则寻找特征边
				if (angel > (PI / 4))
				{
					if (!Ph->pVertEnd->cdrgee)
						Ph->pVertEnd->cdrgee = Kit->CalcuPointCurve(Ph->pVertEnd);
					if (!Ph->pHEdgePre->pVertEnd->cdrgee)
						Ph->pHEdgePre->pVertEnd->cdrgee = Kit->CalcuPointCurve(Ph->pHEdgePre->pVertEnd);
					double cdrgee = Ph->pVertEnd->cdrgee + Ph->pHEdgePre->pVertEnd->cdrgee;
					PVERT p2 = new VERT;
					p2->x = Ph->pVertEnd->cdrgee / cdrgee*Ph->pVertEnd->x + Ph->pHEdgePre->pVertEnd->cdrgee / cdrgee*Ph->pHEdgePre->pVertEnd->x;
					p2->y = Ph->pVertEnd->cdrgee / cdrgee*Ph->pVertEnd->y + Ph->pHEdgePre->pVertEnd->cdrgee / cdrgee*Ph->pHEdgePre->pVertEnd->y;
					p2->z = Ph->pVertEnd->cdrgee / cdrgee*Ph->pVertEnd->z + Ph->pHEdgePre->pVertEnd->cdrgee / cdrgee*Ph->pHEdgePre->pVertEnd->z;

					////////开始特征区域边折叠操作
					PFACETTRI(Ph->pFacetAdj)->beused = 1;
					PFACETTRI(Ph->pHEdgePair->pFacetAdj)->beused = 1;
					for (int j = 0; j < vecper.size(); j++)
					{
						if (!vecper[j]->beused)
						{
							PFACETTRI PFact; PFact = new FACETTRI;
							PVERT Pvert[2]; Pvert[0] = Ph->pVertEnd; Pvert[1] = Ph->pHEdgePre->pVertEnd;
							for (int k = 0; k < 2; k++)
							{
								for (int r = 0; r < 3; r++)
								{
									int ncnt = (r + 1) % 3, nct = (r + 2) % 3;
									if (Pvert[k] == vecper[j]->m_PVerts[r])
									{
										PVERT p0, p1; p0 = new VERT; p1 = new VERT;
										p0->x = vecper[j]->m_PVerts[ncnt]->x; p0->y = vecper[j]->m_PVerts[ncnt]->y; p0->z = vecper[j]->m_PVerts[ncnt]->z;
										p1->x = vecper[j]->m_PVerts[nct]->x; p1->y = vecper[j]->m_PVerts[nct]->y; p1->z = vecper[j]->m_PVerts[nct]->z;
										PVERT POint[3];  POint[0] = p0; POint[1] = p1; POint[2] = p2;
										for (int nt = 0; nt < 3; nt++)
										{
											iter = map_Vert.find(*POint[nt]);
											if (iter != map_Vert.end())//已存在该点
											{
												int Num = iter->second;
												pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFact);//构建一阶邻域
												PFact->m_PVerts[nt] = pSimple->m_vecPVert[Num];
											}
											else
											{
												POint[nt]->VertexID = VetxID;
												POint[nt]->vec_OneFac.push_back(PFact);//构建一阶邻域
												map_Vert.insert(pair<VERT, int>(*POint[nt], pSimple->m_vecPVert.size()));
												pSimple->m_vecPVert.push_back(POint[nt]);
												PFact->m_PVerts[nt] = POint[nt];
												VetxID++;
											}
										}
									}
								}
							}
							vecper[j]->beused = 1; vecper[j]->befrost = 1;
							PFact->befrost = 1;//为下一次简化做好标记
							pSimple->m_vecPFacetTri.push_back(PFact);
						}
					}
				}
			}
		}
	}
}

BOOL CSimplify::Judgelegal(PFACETTRI PFc0)
{  
	Kit->FinOneFac_Less(PFc0);
	vector<PFACETTRI> vecper = PFc0->vec_FOneFac; 
	for (int i = 0; i < vecper.size(); i++)
	{
		if (vecper[i]->befrost || vecper[i]->beboard || vecper[i]->beused)
		{
			return FALSE;
		}
		VECTOR3D V0;
		for (int j = 0; j < 3; j++)
		{
			////////////////////判断面片的一阶邻域面片折叠前后法矢有无明显翻转///////////////
			for (int k = 0; k < 3; k++)
			{
				int nct = (k + 1) % 3;    int ncnt = (k + 2) % 3;

				if (PFc0->m_PVerts[j] == vecper[i]->m_PVerts[k])
				{
					V0 = *Kit->CalcuPfacNormal(vecper[i]->m_PVerts[nct], vecper[i]->m_PVerts[ncnt], PFc0->Pvet);
				}
			}
		}
		double angel = acos(V0 | (*vecper[i]->m_PFacetNorm));
		if (angel>(PI / 12))
		{
			return FALSE;
		}
	}
	return TRUE;
}

BOOL CSimplify::Markboundary(STLModell *pMeshTri, STLModell *pMeshDate)
{
	vec_PHEDGE BordRing;//存储所有的边界边，查找边界边的一阶邻域面片
	for (int i = 0; i < pMeshTri->m_vecPEdge.size(); i++)
	{
		PHEDGE Ph = pMeshTri->m_vecPEdge[i]->pHEdgeAdj;
		if (!Ph->pHEdgePair)
		{
			Kit->FindedgeOnering(Ph, Ph->vec_FOneFac);
			BordRing.push_back(Ph);
			for (int j = 0; j < Ph->vec_FOneFac.size(); j++)
			{
				if (!Ph->vec_FOneFac[j]->bStatus)//不存在该面片
				{
					Ph->vec_FOneFac[j]->bStatus = 1;
				}
			}
		}
	}
	if (BordRing.size()==0)
	{
		return FALSE;
	}
	PPOLYPOLYGON vec_poly = new POLYPOLYGON;
	Kit->closedBoundary(BordRing, vec_poly);//给所有的边界顶点排序
	vector<vec_PHEDGE> BordRingTemp;//存储多环的容器
	//对边界半边进行排序连接
	for (int i = 0; i < vec_poly->m_vecPPolygons.size(); i++)
	{
		PPOLYGON ploy = vec_poly->m_vecPPolygons[i];
		edgepoint = ploy->m_vecPnts;
		vec_PHEDGE BordRingempty;
		for (int j = 0; j <ploy->m_vecPnts.size() ; j++)
		{
			for (int k = 0; k < BordRing.size(); k++)
			{
				if (BordRing[k]->pVertEnd->x == ploy->m_vecPnts[j].x&&BordRing[k]->pVertEnd->y == ploy->m_vecPnts[j].y
					&&BordRing[k]->pVertEnd->z == ploy->m_vecPnts[j].z)
				{
					BordRingempty.push_back(BordRing[k]);
					BordRing.erase(BordRing.begin() + k);
					break;
				}
			}
		}
		BordRingTemp.push_back(BordRingempty);
	}
	//////////////*重点调试对象*/////////////////////////
	/*对边界顶点进行边折叠操作*/
	for (int i = 0; i < vec_poly->m_vecPPolygons.size();i++)
	{
		bool ifSimple=false;
		PPOLYGON ploy = vec_poly->m_vecPPolygons[i];
		vec_PHEDGE BordRingempty = BordRingTemp[i];
		do
		{
			ifSimple = false;
			for (int j = ploy->m_vecPnts.size()-1; j >=0; j--)
			{
				
				if (j != ploy->m_vecPnts.size() - 1)
				{
					j = j - 2;
				}
				int n1, n2,n3;
				n1 = j -3; n2 = j - 2; n3 = j - 1;
				if (n1<0)
				{
					break;
				}
				if (Judgelegal(ploy->m_vecPnts[n2], ploy->m_vecPnts[n3], ploy->m_vecPnts[j])
					&& Judgelegal(ploy->m_vecPnts[n1], ploy->m_vecPnts[n2], ploy->m_vecPnts[n3]))
				{
					POINT3D p0;
					p0.x = 0.5*(ploy->m_vecPnts[n2].x + ploy->m_vecPnts[n3].x);
					p0.y = 0.5*(ploy->m_vecPnts[n2].y + ploy->m_vecPnts[n3].y);
					p0.z = 0.5*(ploy->m_vecPnts[n2].z + ploy->m_vecPnts[n3].z);
					//更新边界区域的三角形信息
					vec_PFACETTRI vecper = BordRingempty[n2]->vec_FOneFac;
					//从面表中删除边界三角形
					for (int k = 0; k < vecper.size(); k++)
					{
						bool Markface = false;
						for (int m = 0; m < 3; m++)
						{
							int  nct,ncnt;
							nct = (m + 1) % 3; ncnt = (m + 2) % 3;
							//将待折叠面片从n1,n2,n3一阶邻域中删除
							if (vecper[k]->m_PVerts[m]->x == ploy->m_vecPnts[n2].x&&vecper[k]->m_PVerts[m]->y == ploy->m_vecPnts[n2].y
								&&vecper[k]->m_PVerts[m]->z == ploy->m_vecPnts[n2].z)
							{
								vector<int> Snumber; Snumber.push_back(nct); Snumber.push_back(ncnt);
								for (int n = 0; n < 2;n++)
								{
									if (vecper[k]->m_PVerts[Snumber[n]]->x == ploy->m_vecPnts[n3].x&&vecper[k]->m_PVerts[Snumber[n]]->y == ploy->m_vecPnts[n3].y
										&&vecper[k]->m_PVerts[Snumber[n]]->z == ploy->m_vecPnts[n3].z)
									{
										Markface = true;
										//删除半边n1,n3的一邻域面片
										for (int r = 0; r < BordRingempty[n3]->vec_FOneFac.size(); r++)
										{
											if (BordRingempty[n3]->vec_FOneFac[r] == vecper[k])
											{
												BordRingempty[n3]->vec_FOneFac.erase(BordRingempty[n3]->vec_FOneFac.begin() + r);
												break;
											}
										}
										for (int r = 0; r < BordRingempty[n1]->vec_FOneFac.size(); r++)
										{
											if (BordRingempty[n1]->vec_FOneFac[r] == vecper[k])
											{
												BordRingempty[n1]->vec_FOneFac.erase(BordRingempty[n1]->vec_FOneFac.begin() + r);
												break;
											}
										}
										vecper.erase(vecper.begin() + k);
										break;
									}
								}
							}
							if (Markface)
							{
								break;
							}
						}
						if (Markface)
						{
							break;
						}
					}
					//从面表中删除顶点边界三角形
					for (int k = 0; k < vecper.size();k++)
					{
						for (int m = 0; m < 3; m++)
						{
							PVERT pten = new VERT;
							pten->x = p0.x; pten->y = p0.y; pten->z = p0.z;
							if (vecper[k]->m_PVerts[m]->x == ploy->m_vecPnts[n2].x&&vecper[k]->m_PVerts[m]->y == ploy->m_vecPnts[n2].y
								&&vecper[k]->m_PVerts[m]->z == ploy->m_vecPnts[n2].z)
							{
								vecper[k]->m_PVerts[m] = pten;
								BordRingempty[n3]->vec_FOneFac.push_back(vecper[k]);
								break; 
							}
							if (vecper[k]->m_PVerts[m]->x == ploy->m_vecPnts[n3].x&&vecper[k]->m_PVerts[m]->y == ploy->m_vecPnts[n3].y
								&&vecper[k]->m_PVerts[m]->z == ploy->m_vecPnts[n3].z)
							{
								
								vecper[k]->m_PVerts[m] = pten;
								BordRingempty[n1]->vec_FOneFac.push_back(vecper[k]);
								break;
							}
						}
					}
					BordRingempty.erase(BordRingempty.begin() + n2);
					vec_POINT3D::iterator it;
					it = ploy->m_vecPnts.begin() + n2;
					ploy->m_vecPnts.erase(it);
					ploy->m_vecPnts[n2] = p0;
					ifSimple = true;
				}
			}
			if (!ploy->m_vecPnts.empty() && ifSimple == false)
			{
				break;
			}
		} while (ifSimple);
		BordRingTemp[i] = BordRingempty;
		edgepoint = ploy->m_vecPnts;
	}

	/*更新所有三角面片信息*/
	//存入非边界三角形面片
	int VetxID = 0;
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	for (int i = 0; i < pMeshTri->m_vecPFacetTri.size(); i++)//将三角面片放入容器中
	{
		if (!pMeshTri->m_vecPFacetTri[i]->bStatus)
		{
			PFACETTRI PFat; PFat = new FACETTRI; PVERT p0;
			for (int j = 0; j < 3; j++)
			{
				p0 = new VERT;
				p0->x = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->x;
				p0->y = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->y;
				p0->z = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->z;
				iter = map_Vert.find(*p0);
				if (iter != map_Vert.end())//已存在该点
				{
					int Num = iter->second;
					pMeshDate->m_vecPVert[Num]->vec_OneFac.push_back(PFat);
					PFat->m_PVerts[j] = pMeshDate->m_vecPVert[Num];
				}
				else
				{
					p0->VertexID = VetxID;
					p0->vec_OneFac.push_back(PFat);
					map_Vert.insert(pair<VERT, int>(*p0, pMeshDate->m_vecPVert.size()));
					pMeshDate->m_vecPVert.push_back(p0);
					PFat->m_PVerts[j] = p0;
					VetxID++;
				}
			}
			PFat->m_PFacetNorm = Kit->CalcuPfacNormal(PFat->m_PVerts[0], PFat->m_PVerts[1], PFat->m_PVerts[2]);///求面片的法矢
			pMeshDate->m_vecPFacetTri.push_back(PFat);
		}
	}
	//存储处理后边界的面片
	map<PFACETTRI, int> map_fact; map_fact.clear();
	map<PFACETTRI, int>::iterator iter_fact;
	for (int i = 0; i <BordRingTemp[i].size(); i++)//将三角面片放入容器中
	{
		vec_PHEDGE BordRingempty = BordRingTemp[i];
		for (int j = 0; j < BordRingempty.size(); j++)
		{
			for (int k = 0; k < BordRingempty[j]->vec_FOneFac.size();k++)
			{
				iter_fact = map_fact.find(BordRingempty[j]->vec_FOneFac[k]);
				if (iter_fact == map_fact.end())//不存在该面片
				{
					map_fact.insert(pair<PFACETTRI, int>(BordRingempty[j]->vec_FOneFac[k], pMeshDate->m_vecPFacetTri.size()));
					PFACETTRI PFat; PFat = new FACETTRI; PVERT p0;
					for (int m = 0; m < 3; m++)
					{
						p0 = new VERT;
						p0->x = BordRingempty[j]->vec_FOneFac[k]->m_PVerts[m]->x;
						p0->y = BordRingempty[j]->vec_FOneFac[k]->m_PVerts[m]->y;
						p0->z = BordRingempty[j]->vec_FOneFac[k]->m_PVerts[m]->z;
						iter = map_Vert.find(*p0);
						if (iter != map_Vert.end())//已存在该点
						{
							int Num = iter->second;
							pMeshDate->m_vecPVert[Num]->vec_OneFac.push_back(PFat);
							PFat->m_PVerts[m] = pMeshDate->m_vecPVert[Num];
						}
						else
						{
							p0->VertexID = VetxID;
							p0->vec_OneFac.push_back(PFat);
							map_Vert.insert(pair<VERT, int>(*p0, pMeshDate->m_vecPVert.size()));
							pMeshDate->m_vecPVert.push_back(p0);
							PFat->m_PVerts[m] = p0;
							VetxID++;
						}
					}
					PFat->beboard = 1;
					PFat->m_PFacetNorm = Kit->CalcuPfacNormal(PFat->m_PVerts[0], PFat->m_PVerts[1], PFat->m_PVerts[2]);///求面片的法矢
					pMeshDate->m_vecPFacetTri.push_back(PFat);
				}
			}
		}
	}
	return TRUE;
}

BOOL CSimplify::Judgelegal(POINT3D p0, POINT3D p1, POINT3D p2)
{
	//加入两边界边夹角条件，限制边界边过度简化
	VECTOR3D v1, v2;
	v1 = p0 - p1;
	v2 = p2 - p1;
	v1.Normalize(); v2.Normalize();
	double angel = v1|v2;
	if (angel>-0.9)
	{
		return false;
	}
	else
	{
		return true;
	}
	return true;
}
MatrixXd  CSimplify::Planeeuate(PFACETTRI PFc0)
{
	MatrixXd A(4, 1); 
	POINT3D p1, p2, p3;
	double a, b, c, d;

	p1 = *PFc0->pHEdge->pVertEnd;
	p2 = *PFc0->pHEdge->pHEdgePre->pVertEnd;
	p3 = *PFc0->pHEdge->pHEdgeNext->pVertEnd;
	a = ((p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y));
	b = ((p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z));
	c = ((p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x));
	VECTOR3D v1 = VECTOR3D(a, b, c);	v1.Normalize();
	a = v1.dx;	b = v1.dy;	c = v1.dz;
	d = (0 - (a*p1.x + b*p1.y + c*p1.z));
	int nt = a*a + b*b + c*c;
	A << a, b, c, d;
	return A;
}

void CSimplify::EdgesimpleResult(STLModell *pMeshTri, STLModell *pSimple, float R)
{
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;

	//double t0 = (double)clock();
	vector<PEDGE>  vec_Temp;
	vec_Temp = pMeshTri->m_vecPEdge;
	CTimer t;
	
	AssortTriangle(vec_Temp);//计算所有边的折叠代价
	Kit->RankAlledge(vec_Temp);//按照折叠误差大小对边表进行排序
	int n = int(R / 100 * vec_Temp.size()), VetxID = 0; BOOL  legal;
	TriangleTimeTemp = t.elapsed_micro();
	//t.reset();
	//double t1 = (double)clock(); TriangleTimeTemp = (t1 - t0) / 1000;
	int twe = 0;
	for (int i = 0; i < n; i++)
	{
		if (vec_Temp[i]->pHEdgeAdj->bStatus == 1 )
		{
			vector<PFACETTRI> vecper;
 			legal = Judgelegal(vec_Temp[i],vecper);//合法性检查，防止折叠后面片法矢发生过大翻转
			if (!legal)   { continue; }

			PFACETTRI(vec_Temp[i]->pHEdgeAdj->pFacetAdj)->beused = 1;  //对折叠边的邻接三角形进行标记
			PFACETTRI(vec_Temp[i]->pHEdgeAdj->pHEdgePair->pFacetAdj)->beused = 1; 
			//////////////////对三角形进行折叠操作////////////////////
			PVERT p2 = new VERT;
			p2->x = vec_Temp[i]->pHEdgeAdj->Pvet->x; p2->y = vec_Temp[i]->pHEdgeAdj->Pvet->y; p2->z = vec_Temp[i]->pHEdgeAdj->Pvet->z;
			delete vec_Temp[i]->pHEdgeAdj->Pvet;
			for (int j = 0; j < vecper.size(); j++)
			{
				if (vecper[j]->beused == 0)
				{
					PFACETTRI PFact; PFact = new FACETTRI;
					PVERT Pvert[2]; Pvert[0] = vec_Temp[i]->pVertStart; Pvert[1] = vec_Temp[i]->pVertEnd;
					for (int k = 0; k < 2; k++)
					{
						for (int r = 0; r < 3; r++)
						{
							int ncnt = (r + 1) % 3, nct = (r + 2) % 3;
							if (Pvert[k] == vecper[j]->m_PVerts[r])
							{
								PVERT p0, p1; p0 = new VERT; p1 = new VERT; 
								p0->x = vecper[j]->m_PVerts[ncnt]->x; p0->y = vecper[j]->m_PVerts[ncnt]->y; p0->z = vecper[j]->m_PVerts[ncnt]->z;
								p1->x = vecper[j]->m_PVerts[nct]->x; p1->y = vecper[j]->m_PVerts[nct]->y; p1->z = vecper[j]->m_PVerts[nct]->z;
								PVERT POint[3];  POint[0] = p0; POint[1] = p1; POint[2] = p2;
								for (int nt = 0; nt < 3; nt++)
								{
									iter = map_Vert.find(*POint[nt]);
									if (iter != map_Vert.end())//已存在该点
									{
										int Num = iter->second;
										pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFact);//构建一阶邻域
										PFact->m_PVerts[nt] = pSimple->m_vecPVert[Num];
									}
									else
									{
										POint[nt]->VertexID = VetxID;
										POint[nt]->vec_OneFac.push_back(PFact);//构建一阶邻域
										map_Vert.insert(pair<VERT, int>(*POint[nt], pSimple->m_vecPVert.size()));
										pSimple->m_vecPVert.push_back(POint[nt]);
										PFact->m_PVerts[nt] = POint[nt];
										VetxID++;
									}
								}
							}
						}
					}
					vecper[j]->beused = 1;
					pSimple->m_vecPFacetTri.push_back(PFact);
				}
			}

		}
	}
	int onetime=0;
	PushUntreatedFace(pMeshTri->m_vecPFacetTri, pSimple, map_Vert, iter, VetxID,onetime);//处理所有未经折叠的面片
	vec_Temp.clear();
}

void CSimplify::PushUntreatedFace(vector<PFACETTRI>  vec_Temp, STLModell *pSimple, map<VERT, int>map_Vert, map<VERT, int>::iterator iter, int VetxID, int Onetime)
{
	for (int i = 0; i < vec_Temp.size(); i++)//将不用折叠的三角面片放入容器中
	{
		if (vec_Temp[i]->beused == 0)
		{
			PFACETTRI PFat; PFat = new FACETTRI;
			for (int j = 0; j < 3; j++)
			{
				PVERT p0; p0 = new VERT;
				p0->x = vec_Temp[i]->m_PVerts[j]->x;  p0->y = vec_Temp[i]->m_PVerts[j]->y;
				p0->z = vec_Temp[i]->m_PVerts[j]->z;
				iter = map_Vert.find(*p0);
				if (iter != map_Vert.end())//已存在该点
				{
					int Num = iter->second;
					pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFat);
					PFat->m_PVerts[j] = pSimple->m_vecPVert[Num];
				}
				else
				{
					p0->VertexID = VetxID;
					p0->vec_OneFac.push_back(PFat);
					map_Vert.insert(pair<VERT, int>(*p0, pSimple->m_vecPVert.size()));
					pSimple->m_vecPVert.push_back(p0);
					PFat->m_PVerts[j] = p0;
					VetxID++;
				}
			}
			if (vec_Temp[i]->befrost&&Onetime) PFat->befrost = 1;
			pSimple->m_vecPFacetTri.push_back(PFat); 
		}
	}
}


BOOL CSimplify::Judgelegal(PEDGE Pedge,vector<PFACETTRI> &vecper)
{
	PHEDGE Ph0 = Pedge->pHEdgeAdj;

	Kit->FindedgeOnering(Ph0, vecper);
	PVERT pvert[2];
	pvert[0] = Pedge->pVertStart; pvert[1] = Pedge->pVertEnd;
	//////////////判断折叠后边的一阶邻域面片法矢是否发生反转////////////
	for (int i = 0; i < vecper.size(); i++)
	{
		 VECTOR3D V0;
		if (vecper[i]->beused )
		{
			return FALSE;
		}
		for (int j = 0; j < 2; j++)
		{
			////////////////////判断面片的一阶邻域面片折叠前后法矢有无明显翻转///////////////
			for (int k = 0; k < 3; k++)
			{
				int nct = (k + 1) % 3;    int ncnt = (k + 2) % 3;

				if (pvert[j] == vecper[i]->m_PVerts[k])
				{
					V0 = *Kit->CalcuPfacNormal(vecper[i]->m_PVerts[nct], vecper[i]->m_PVerts[ncnt], Ph0->Pvet);
				}
			}
		}
		double angel = acos(V0 | (*vecper[i]->m_PFacetNorm));
		if (angel>(PI / 5))
		{
			return FALSE;
		}
	}
	return TRUE;
	//////////////////SOD搜寻算法寻找特征边///////////////////////
	double  C0=0 , C1=0 ;
	Kit->Gettriperimeter(Ph0, C0);
	Kit->Gettriperimeter(Ph0->pHEdgePair, C1);
	Kit->Ranktwodigit(C0, C1);

	VECTOR3D m_vNorm1, m_vNorm2;
	m_vNorm1 = *Ph0->pFacetAdj->m_PFacetNorm;
	m_vNorm2 = *Ph0->pHEdgePair->pFacetAdj->m_PFacetNorm;
	double angel = acos(m_vNorm1 | m_vNorm2);
	if (angel > (PI / 3))//之前是90度时效果没现在60度时效果好
	{
		return false;
	}
	return true;
}
void CSimplify::AssortTriangle(vector<PEDGE>  vecPedge)
{
	for (int i = 0; i < vecPedge.size();i++)
	{
		//对内部三角形（碟形域）求折叠后的新点///////////////////////
		PHEDGE Ph0 = vecPedge[i]->pHEdgeAdj;
		if (Ph0->pHEdgePair != NULL)
		{
			PHEDGE Phedge[5];
			Phedge[0] = Ph0; Phedge[1] = Ph0->pHEdgePre;  Phedge[2] = Ph0->pHEdgeNext;
			Phedge[3] = Ph0->pHEdgePair->pHEdgeNext; Phedge[4] = Ph0->pHEdgePair->pHEdgePre;
			vec_VERT pVert; pVert.clear();
			map<VERT, int> map_Vert; map_Vert.clear();
			map<VERT, int>::iterator iter;
			for (int j = 0; j < 4; j++)
			{
				map_Vert.insert(pair<VERT, int>(*Phedge[j]->pVertEnd, pVert.size()));
				pVert.push_back(*Phedge[j]->pVertEnd);
			}
			bool RMark = false;
			for (int j = 1; j < 5; j++)
			{
				if (Phedge[j]->pHEdgePair == NULL)
				{
					RMark = true;break;
				}

				PFACETTRI Pftemp = PFACETTRI(Phedge[j]->pHEdgePair->pFacetAdj);
				for (int k = 0; k < 3; k++)
				{
					iter = map_Vert.find(*Pftemp->m_PVerts[k]);
					if (iter == map_Vert.end())//不存在该点
					{
						map_Vert.insert(pair<VERT, int>(*Pftemp->m_PVerts[k], pVert.size()));
						pVert.push_back(*Pftemp->m_PVerts[k]);
					}
				}
			}
			if (!RMark)
			{
				//////////////////计算新的顶点值//////////////////////////////////
				double Serx = 0, Sery = 0, Serz = 0;
				for (int j = 4; j < pVert.size(); j++)
				{
					Serx += pVert[j].x;
					Sery += pVert[j].y;
					Serz += pVert[j].z;
				}
				Ph0->bStatus = 1;
				Ph0->Pvet = new VERT;
				Ph0->Pvet->x = 0.5 * (pVert[0].x + pVert[1].x) + 0.125 * (pVert[2].x + pVert[3].x) - 0.0625 * Serx;
				Ph0->Pvet->y = 0.5 * (pVert[0].y + pVert[1].y) + 0.125 * (pVert[2].y + pVert[3].y) - 0.0625 * Sery;
				Ph0->Pvet->z = 0.5 * (pVert[0].z + pVert[1].z) + 0.125 * (pVert[2].z + pVert[3].z) - 0.0625 * Serz;
				CalerrorLittleValue(Ph0, Ph0->Pvet);
				//vec_PFACETTRI vecper;
				//Kit->FindedgeOnering(Ph0, vecper);
				//CalValue_pointplane(Ph0,vecper);
			}
		}
	}
}
void CSimplify::CalValue_pointplane(PHEDGE Ph0, vec_PFACETTRI vecper)
{
	vector<double> dista;
	for (int i = 0; i < vecper.size(); i++)
	{
		VECTOR3D u;
		POINT3D p00, p01;
		p00 = POINT3D(vecper[i]->m_PVerts[0]->x, vecper[i]->m_PVerts[0]->y, vecper[i]->m_PVerts[0]->z);
		p01 = POINT3D(Ph0->Pvet->x, Ph0->Pvet->y, Ph0->Pvet->z);
		u = Kit->PlanePointDistance(p00, p01, *vecper[i]->m_PFacetNorm);
		dista.push_back(u.GetLength());
	}

	Ph0->value = dista[0];
	for (int j = 0; j < vecper.size(); j++)
	{
		Kit->Ranktwodigit(dista[j], Ph0->value);
	}
}
void CSimplify::CalerrorLittleValue(PHEDGE Ph0, PVERT p0)
{
	vector<PFACETTRI> VecPfact;
	Kit->FindedgeOnering(Ph0, VecPfact);
	vector<double> equ(16, int(0));
	for (int i = 0; i < VecPfact.size(); i++)
	{
		vector<double> dis;
		Planeeuate(VecPfact[i], dis);
		equ[0] += dis[0] * dis[0]; equ[1] += dis[0] * dis[1]; equ[2] += dis[0] * dis[2]; equ[3] += dis[0] * dis[3];
		equ[4] += dis[1] * dis[0]; equ[5] += dis[1] * dis[1]; equ[6] += dis[1] * dis[2]; equ[7] += dis[1] * dis[3];
		equ[8] += dis[2] * dis[0]; equ[9] += dis[2] * dis[1]; equ[10] += dis[2] * dis[2]; equ[11] += dis[2] * dis[3];
		equ[12] += dis[3] * dis[0]; equ[13] += dis[3] * dis[1]; equ[14] += dis[3] * dis[2]; equ[15] += dis[3] * dis[3];
	}
	vector<double> tew(4, int(0));
	tew[0] = p0->x*equ[0] + p0->y*equ[4] + p0->z*equ[8] + equ[12];
	tew[1] = p0->x*equ[1] + p0->y*equ[5] + p0->z*equ[9] + equ[13];
	tew[2] = p0->x*equ[2] + p0->y*equ[6] + p0->z*equ[10] + equ[14];
	tew[3] = p0->x*equ[3] + p0->y*equ[7] + p0->z*equ[11] + equ[15];
	Ph0->value = p0->x*tew[0] + p0->y*tew[1] + p0->z*tew[2] + tew[3];
}
void CSimplify::TrianglesimplesmoothResult(STLModell *pMeshTri, STLModell *pSimple, float R)
{
	map<VERT, int>map_Vert; 
	map<VERT, int>::iterator iter;
	//double t0 = (double)clock();
	CTimer t;
	
	vector<PFACETTRI>  vec_Temp;
	for (int i = 0; i < pMeshTri->m_vecPVert.size(); i++)
	{
		vec_PVERT OneRingvert;
		Kit->FindOneRing(pMeshTri->m_vecPVert[i], OneRingvert);
		CalSmoothDegree(pMeshTri->m_vecPVert[i], OneRingvert);
	}
	vec_Temp = pMeshTri->m_vecPFacetTri;
	CalerrorLittleValue(vec_Temp);//计算所有矩阵的特征值及其新顶点
	sort(vec_Temp.begin(), vec_Temp.end(), Kit->RankPfact1);//按照折叠误差大小对面片进行排序
	int n = int(R / 100.0 * vec_Temp.size()), VetxID = 0; BOOL  legal;
	for (int i = 0; i < n; i++)
	{
		if (!vec_Temp[i]->beused)
		{
			vector<PFACETTRI> vecper; PHEDGE Phedg[3];
			Phedg[0] = vec_Temp[i]->pHEdge;
			Phedg[1] = vec_Temp[i]->pHEdge->pHEdgePre; 
			Phedg[2] = vec_Temp[i]->pHEdge->pHEdgeNext;
			legal = Judgelegalsmooth(vec_Temp[i], vecper);//合法性检查，防止折叠后面片法矢发生过大翻转
			if (!legal)   { continue; }
			
			for (int r = 0; r < 3; r++)//对折叠三角面片的邻接三角形进行标记
			{
				if (Phedg[r]->pHEdgePair != NULL)
				{
					PFACETTRI(Phedg[r]->pHEdgePair->pFacetAdj)->beused = 1;
				}
			}
			//////////////////对三角形进行折叠操作////////////////////
			PVERT p2 = new VERT;
			p2->x = vec_Temp[i]->Pvet->x;
			p2->y = vec_Temp[i]->Pvet->y;
			p2->z = vec_Temp[i]->Pvet->z;
			p2->Sdegree = vec_Temp[i]->Pvet->Sdegree;
			for (int j = 0; j < vecper.size(); j++)
			{
				PFACETTRI PFact; PFact = new FACETTRI;
				for (int k = 0; k < 3; k++)
				{
					for (int r = 0; r < 3; r++)
					{
						int ncnt = (r + 1) % 3, nct = (r + 2) % 3;
						if (vec_Temp[i]->m_PVerts[k] == vecper[j]->m_PVerts[r])
						{
							PVERT p0, p1; p0 = new VERT; p1 = new VERT;
							p0->x = vecper[j]->m_PVerts[ncnt]->x; p0->y = vecper[j]->m_PVerts[ncnt]->y; p0->z = vecper[j]->m_PVerts[ncnt]->z;
							p1->x = vecper[j]->m_PVerts[nct]->x; p1->y = vecper[j]->m_PVerts[nct]->y; p1->z = vecper[j]->m_PVerts[nct]->z;

							PVERT POint[3];  POint[0] = p0; POint[1] = p1; POint[2] = p2;
							for (int nt = 0; nt < 3; nt++)
							{
								iter = map_Vert.find(*POint[nt]);
								if (iter != map_Vert.end())//已存在该点
								{
									int Num = iter->second;
									pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFact);//构建一阶邻域
									PFact->m_PVerts[nt] = pSimple->m_vecPVert[Num];
								}
								else
								{
									POint[nt]->VertexID = VetxID;
									POint[nt]->vec_OneFac.push_back(PFact);//构建一阶邻域
									map_Vert.insert(pair<VERT, int>(*POint[nt], pSimple->m_vecPVert.size()));
									pSimple->m_vecPVert.push_back(POint[nt]);
									PFact->m_PVerts[nt] = POint[nt];
									VetxID++;
								}
							}
						}
					}
				}
				vecper[j]->beused = 1;
				pSimple->m_vecPFacetTri.push_back(PFact);
			}
			vec_Temp[i]->beused = 1;
		}
	}
	int onetime = 0;
	PushUntreatedFace(vec_Temp, pSimple, map_Vert, iter, VetxID,onetime);//处理没有被简化的面片
	vec_Temp.clear();
	//double t1 = (double)clock(); TriangleTimeTemp = (t1 - t0) / 1000;
	TriangleTimeTemp = t.elapsed_micro();
}

BOOL CSimplify::EliminatetheReapetedFace(PFACETTRI Pf0, vector<PFACETTRI> vecper)
{
	for (int i = 0; i < vecper.size(); i++)
	{
		PFACETTRI PFact; PFact = new FACETTRI;
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				int ncnt = (k + 1) % 3, nct = (k + 2) % 3;
				PVERT p0, p1, p2;
				p0 = vecper[i]->m_PVerts[ncnt];
				p1 = vecper[i]->m_PVerts[nct];
				p2 = vecper[i]->m_PVerts[k];
				if (Pf0->m_PVerts[j] == p2)
				{
					for (int m = 0; m < i; m++)
					{
						for (int n = 0; n < 3; n++)
						{
							int nqt = (n + 1) % 3, nqnt = (n + 2) % 3;
							if (p0->x == vecper[m]->m_PVerts[n]->x&&p0->y == vecper[m]->m_PVerts[n]->y&&p0->z == vecper[m]->m_PVerts[n]->z)
							{
								if (p1->x == vecper[m]->m_PVerts[nqt]->x&&p1->y == vecper[m]->m_PVerts[nqt]->y&&p1->z == vecper[m]->m_PVerts[nqt]->z)
								{
									return FALSE;
								}
								if (p1->x == vecper[m]->m_PVerts[nqnt]->x&&p1->y == vecper[m]->m_PVerts[nqnt]->y&&p1->z == vecper[m]->m_PVerts[nqnt]->z)
								{
									return FALSE;
								}
							}
						}
					}
				}
			}
		}
	}
	return TRUE;
}
BOOL CSimplify::Judgelegalsmooth(PFACETTRI PFact, vector<PFACETTRI> &vecper)
{
	vector<PFACETTRI> vec1, vec2;//一阶领域面片

	Kit->FindPOneRFAC(PFact, vec1);
	Kit->FindPOneRFAC_NEW(PFact, vec2);
	/////////////////////求一阶邻域面片//////////////////////
	for (int i = 0; i < vec1.size(); i++)
	{
		int t = 0;
		if (vec1[i]->beused)
		{
			return FALSE;
		}
		for (int j = 0; j < vec2.size(); j++)
		{
			if (vec1[i] != vec2[j])
			{
				t++;
			}
		}
		if (t == vec2.size())
		{
			vecper.push_back(vec1[i]);
		}
	}
	////////////三角面片最外围的点////////////////
	map<VERT, int> map_Temp; map_Temp.clear();
	map<VERT, int>::iterator iter_Temp;

	vec_PVERT Pverts;
	for (int k = 0; k < 3;k++)
	{
		map_Temp.insert(pair<VERT, int>(*PFact->m_PVerts[k], Pverts.size()));
	}
	for (int i = 0; i < vec2.size(); i++)
	{
		for (int j = 0; j < 3;j++)
		{
			iter_Temp = map_Temp.find(*vec2[i]->m_PVerts[j]);
			if (iter_Temp == map_Temp.end())
			{
				Pverts.push_back(vec2[i]->m_PVerts[j]);
				map_Temp.insert(pair<VERT, int>(*vec2[i]->m_PVerts[j], Pverts.size()));
			}
		}
	}
	if (Pverts.size()!=3)
	{
		return FALSE;
	}
	for (int i = 0; i < vec1.size(); i++)
	{
		for (int j = 0; j < 3; j++)
		{
			iter_Temp = map_Temp.find(*vec1[i]->m_PVerts[j]);
			if (iter_Temp == map_Temp.end())
			{
				Pverts.push_back(vec1[i]->m_PVerts[j]);
				map_Temp.insert(pair<VERT, int>(*vec1[i]->m_PVerts[j], Pverts.size()));
			}
		}
	}
	vec1.clear(); vec2.clear();
	///////////////排除折叠后重复的面片/////////////
	for (int i = 0; i < Pverts.size();i++)
	{
		int Net=0;
		for (int j = 0; j < vecper.size();j++)
		{
			for (int k = 0; k < 3;k++)
			{
				if (Pverts[i]==vecper[j]->m_PVerts[k])
				{
					Net++;
				}
			}
			if (Net>2)
			{
				return FALSE;
			}
		}
	}
	///////////////去除折叠后重复的面片/////////////
	if (!EliminatetheReapetedFace(PFact, vecper))
	{
		return FALSE;
	}

	//将不平滑度进行累积传递/////////////////
	vec_PVERT OneRingvert;
	map<VERT, int> map_Vert; map_Vert.clear();
	map<VERT, int>::iterator iter;
	map_Vert.insert(pair<VERT, int>(*PFact->m_PVerts[0], OneRingvert.size()));
	map_Vert.insert(pair<VERT, int>(*PFact->m_PVerts[1], OneRingvert.size()));
	map_Vert.insert(pair<VERT, int>(*PFact->m_PVerts[2], OneRingvert.size()));

	for (int i = 0; i < vecper.size(); i++)
	{
		for (int j = 0; j < 3;j++)
		{
			iter = map_Vert.find(*vecper[i]->m_PVerts[j]);
			if (iter == map_Vert.end())//不存在该点
			{
				map_Vert.insert(pair<VERT, int>(*vecper[i]->m_PVerts[j], OneRingvert.size()));
				OneRingvert.push_back(vecper[i]->m_PVerts[j]);
			}
		}
	}
	double Lsmooth=0;
	for (int k = 0; k < 3;k++)
	{
		Lsmooth += PFact->m_PVerts[k]->Sdegree;
	}
	Lsmooth = Lsmooth / 3;
	CalSmoothDegree(PFact->Pvet, OneRingvert);
	PFact->Pvet->Sdegree += Lsmooth;
	return TRUE;
}
void CSimplify::CalSmoothDegree(PVERT p0, vec_PVERT OneRingvert)
{
	if (!p0->Sdegree)
	{
		vector<VECTOR3D> VoneRing;
		for (int i = 0; i < OneRingvert.size(); i++)
		{
			VECTOR3D v1 = *OneRingvert[i] - *p0;
			v1.Normalize();
			VoneRing.push_back(v1);
		}
		VECTOR3D vTemp;
		int nt = VoneRing.size();
		for (int i = 0; i < nt; i++)
		{
			vTemp.dx += VoneRing[i].dx;
			vTemp.dy += VoneRing[i].dy;
			vTemp.dz += VoneRing[i].dz;
		}
		vTemp = vTemp / nt;
		vTemp.Normalize();
		for (int i = 0; i < nt; i++)
		{
			p0->Sdegree += (PI / 2 - acos(abs(VoneRing[i] | vTemp)));
		}
		p0->Sdegree = 180 * p0->Sdegree / (PI*nt);
	}
}
void CSimplify::CalerrorLittleValue(vector<PFACETTRI>  vecPfact)
{
	for (int i = 0; i < vecPfact.size();i++)
	{
		vector<double> vlong; double Lcir=0;
		for (int j = 0; j < 3;j++)
		{
			int nct=(j+1)%3;
			VECTOR3D v1 = *vecPfact[i]->m_PVerts[j] - *vecPfact[i]->m_PVerts[nct];
			Lcir += v1.GetLength();
			vlong.push_back(v1.GetLength());
			vecPfact[i]->value += vecPfact[i]->m_PVerts[j]->Sdegree;
		}
		vecPfact[i]->Pvet = new VERT;
		vecPfact[i]->value = vecPfact[i]->value / 3 * pow(Lcir / 2 * (Lcir / 2 - vlong[0])*(Lcir / 2 - vlong[1])*(Lcir / 2 - vlong[2]), 1.0 / 4);
		VERT p0, p1, p2;
		p0 = *vecPfact[i]->m_PVerts[0]; p1 = *vecPfact[i]->m_PVerts[1]; p2 = *vecPfact[i]->m_PVerts[2];
		double sdrgee = p0.Sdegree + p1.Sdegree + p2.Sdegree;
		vecPfact[i]->Pvet->x = p0.Sdegree / sdrgee*p0.x + p1.Sdegree / sdrgee*p1.x + p2.Sdegree / sdrgee*p2.x;
		vecPfact[i]->Pvet->y = p0.Sdegree / sdrgee*p0.y + p1.Sdegree / sdrgee*p1.y + p2.Sdegree / sdrgee*p2.y;
		vecPfact[i]->Pvet->z = p0.Sdegree / sdrgee*p0.z + p1.Sdegree / sdrgee*p1.z + p2.Sdegree / sdrgee*p2.z;
	}
}


void CSimplify::TrianglesimpleResult(STLModell *pMeshTri, STLModell *pSimple, float R)
{
	vector<PFACETTRI>  vec_Temp;
	double sharpMin, sharpMax;
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	//double t0 = (double)clock();
	CTimer t;
	
	vec_Temp = pMeshTri->m_vecPFacetTri;
	AssortTriangle(vec_Temp, sharpMin, sharpMax);//计算所有三角面片的特征值和折叠后的新顶点
	sort(vec_Temp.begin(), vec_Temp.end(), Kit->RankPfact1);//按照折叠误差大小对面片进行排序
	int n = int(R / 100.0 * (vec_Temp.size())), VetxID = 0; BOOL  legal;
	for (int i = 0; i < n; i++)
	{
		if (!vec_Temp[i]->beused &&!vec_Temp[i]->befrost&&vec_Temp[i]->Pvet)
		{
			Checkparticular(vec_Temp[i], 0.5*(sharpMax + sharpMin));//检查一个三角形是不是特征三角形

			vector<PFACETTRI> vecper; PHEDGE Phedg[3];
			Phedg[0] = vec_Temp[i]->pHEdge; Phedg[1] = vec_Temp[i]->pHEdge->pHEdgePre; Phedg[2] = vec_Temp[i]->pHEdge->pHEdgeNext;
			legal = Judgelegal(vec_Temp[i], vecper);//合法性检查，防止折叠后面片法矢发生过大翻转
			if (!legal)   { continue; }

			for (int r = 0; r < 3; r++)//对折叠三角面片的邻接三角形进行标记
			{
				if (Phedg[r]->pHEdgePair != NULL)
				{
					PFACETTRI(Phedg[r]->pHEdgePair->pFacetAdj)->beused = 1;
				}
			}
			//////////////////对三角形进行折叠操作////////////////////
			PVERT p2 = new VERT;
			p2->x = vec_Temp[i]->Pvet->x; p2->y = vec_Temp[i]->Pvet->y; p2->z = vec_Temp[i]->Pvet->z;
			for (int j = 0; j < vecper.size(); j++)
			{
				if (!vecper[j]->beused )
				{
					PFACETTRI PFact; PFact = new FACETTRI;
					for (int k = 0; k < 3; k++)
					{
						for (int r = 0; r < 3; r++)
						{
							int ncnt = (r + 1) % 3, nct = (r + 2) % 3;
							if (vec_Temp[i]->m_PVerts[k] == vecper[j]->m_PVerts[r])
							{
								if (!EliminatetheReapetedFace(vecper[j]->m_PVerts[ncnt], vecper[j]->m_PVerts[nct], j, vecper))
								{
									continue;
								}
								PVERT p0, p1; p0 = new VERT; p1 = new VERT;
								p0->x = vecper[j]->m_PVerts[ncnt]->x; p0->y = vecper[j]->m_PVerts[ncnt]->y; p0->z = vecper[j]->m_PVerts[ncnt]->z;
								p1->x = vecper[j]->m_PVerts[nct]->x; p1->y = vecper[j]->m_PVerts[nct]->y; p1->z = vecper[j]->m_PVerts[nct]->z;

								PVERT POint[3];  POint[0] = p0; POint[1] = p1; POint[2] = p2;
								for (int nt = 0; nt < 3; nt++)
								{
									iter = map_Vert.find(*POint[nt]);
									if (iter != map_Vert.end())//已存在该点
									{
										int Num = iter->second;
										pSimple->m_vecPVert[Num]->vec_OneFac.push_back(PFact);//构建一阶邻域
										PFact->m_PVerts[nt] = pSimple->m_vecPVert[Num];
									}
									else
									{
										POint[nt]->VertexID = VetxID;
										POint[nt]->vec_OneFac.push_back(PFact);//构建一阶邻域
										map_Vert.insert(pair<VERT, int>(*POint[nt], pSimple->m_vecPVert.size()));
										pSimple->m_vecPVert.push_back(POint[nt]);
										PFact->m_PVerts[nt] = POint[nt];
										VetxID++;
									}
								}
							}
						}
					}
					vecper[j]->beused = 1;
					if (PFact->m_PVerts[0] != NULL&& PFact->m_PVerts[1] != NULL && PFact->m_PVerts[2] != NULL)
					{
						pSimple->m_vecPFacetTri.push_back(PFact);
					}
				}
			}
			vec_Temp[i]->beused = 1;
		}
	}
	int onetime=0;
	PushUntreatedFace(vec_Temp, pSimple, map_Vert, iter, VetxID,onetime);//处理没有被简化的面片
	vec_Temp.clear();
	TriangleTimeTemp = t.elapsed_micro();
	//double t1 = (double)clock(); TriangleTimeTemp = (t1 - t0) / 1000;
}
BOOL CSimplify::EliminatetheReapetedFace(PVERT p0, PVERT p1, int r, vector<PFACETTRI> vecper)
{
	for (int i = 0; i < r; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			int nqt = (j + 1) % 3, nqnt = (j + 2) % 3;
			if (p0->x == vecper[i]->m_PVerts[j]->x&&p0->y == vecper[i]->m_PVerts[j]->y&&p0->z == vecper[i]->m_PVerts[j]->z)
			{
				if (p1->x == vecper[i]->m_PVerts[nqt]->x&&p1->y == vecper[i]->m_PVerts[nqt]->y&&p1->z == vecper[i]->m_PVerts[nqt]->z)
				{
					return FALSE;
				}
				if (p1->x == vecper[i]->m_PVerts[nqnt]->x&&p1->y == vecper[i]->m_PVerts[nqnt]->y&&p1->z == vecper[i]->m_PVerts[nqnt]->z)
				{
					return FALSE;
				}
			}
		}
	}
	return TRUE;
}
void CSimplify::AssortTriangle(vector<PFACETTRI> vecPfact, double &Lsmin, double &Lsmax)
{
	for (int i = 0; i < vecPfact.size(); i++)
	{
		PFACETTRI Pfact = vecPfact[i];
		if (Pfact->beused == 0)
		{
			PHEDGE phedge[3];
			phedge[0] = Pfact->pHEdge; phedge[1] = Pfact->pHEdge->pHEdgeNext; phedge[2] = Pfact->pHEdge->pHEdgePre;
			if (!phedge[0]->pHEdgePair || !phedge[1]->pHEdgePair || !phedge[2]->pHEdgePair)
			{
				if (!Pfact->befrost)
				{
					Pfact->befrost = 1;
				}
			}
			PFACETTRI Pf0;
			if (phedge[0]->pHEdgePair != NULL)
			{
				if (phedge[2]->pHEdgePair == NULL)
				{
					if (phedge[1]->pHEdgePair != NULL)
					{
						//////////////求出顶点边界三角形的新顶点及折叠误差////////////////////////
						if (phedge[1]->pHEdgePair->pHEdgeNext->pHEdgePair != NULL&&phedge[1]->pHEdgePair->pHEdgePre->pHEdgePair != NULL)
						{
							PVERT p1 = new VERT;
							p1->x = phedge[1]->pVertEnd->x;
							p1->y = phedge[1]->pVertEnd->y;
							p1->z = phedge[1]->pVertEnd->z;
							CalerrorLittleValue(PFACETTRI(phedge[1]->pHEdgePair->pFacetAdj), p1);
						}
					}
					else
					{
						if (PFACETTRI(phedge[0]->pHEdgePair->pFacetAdj)->beused == 0)
						{
							PVERT p1 = new VERT;
							p1->x = phedge[2]->pVertEnd->x;
							p1->y = phedge[2]->pVertEnd->y;
							p1->z = phedge[2]->pVertEnd->z;
							CalerrorLittleValue(PFACETTRI(phedge[0]->pHEdgePair->pFacetAdj), p1);
						}
					}
				}
				else
				{
					if (phedge[1]->pHEdgePair == NULL)
					{
						//////////////求出顶点边界三角形的新顶点及折叠误差////////////////////////
						if (phedge[2]->pHEdgePair->pHEdgeNext->pHEdgePair != NULL&&phedge[2]->pHEdgePair->pHEdgePre->pHEdgePair != NULL&&PFACETTRI(phedge[2]->pHEdgePair->pFacetAdj)->beused == 0)
						{
							PVERT p1 = new VERT;
							p1->x = phedge[2]->pHEdgePre->pVertEnd->x;
							p1->y = phedge[2]->pHEdgePre->pVertEnd->y;
							p1->z = phedge[2]->pHEdgePre->pVertEnd->z;
							CalerrorLittleValue(PFACETTRI(phedge[2]->pHEdgePair->pFacetAdj), p1);
						}

						if (phedge[0]->pHEdgePair->pHEdgePre->pHEdgePair != NULL&&phedge[0]->pHEdgePair->pHEdgeNext->pHEdgePair != NULL&&PFACETTRI(phedge[0]->pHEdgePair)->beused == 0)
						{
							PVERT p1 = new VERT;
							p1->x = phedge[0]->pVertEnd->x;
							p1->y = phedge[0]->pVertEnd->y;
							p1->z = phedge[0]->pVertEnd->z;
							CalerrorLittleValue(PFACETTRI(phedge[0]->pHEdgePair->pFacetAdj), p1);
						}
					}
					else
					{
						//////////////求解第三类情况即内部三角形的情况下的新顶点和折叠误差/////////////////////////////
						AcqnewVertex(Pfact, Pfact->value);
					}
				}
			}
			else
			{
				//////////////求出顶点边界三角形的新顶点及折叠误差////////////////////////
				if (phedge[2]->pHEdgePair != NULL&&PFACETTRI(phedge[2]->pHEdgePair)->beused == 0)
				{
					if (phedge[2]->pHEdgePair->pHEdgePre->pHEdgePair != NULL&&phedge[2]->pHEdgePair->pHEdgeNext->pHEdgePair != NULL)
					{
						PVERT p1 = new VERT;
						p1->x = phedge[2]->pVertEnd->x;
						p1->y = phedge[2]->pVertEnd->y;
						p1->z = phedge[2]->pVertEnd->z;
						CalerrorLittleValue(PFACETTRI(phedge[2]->pHEdgePair->pFacetAdj), p1);
					}

				}
				if (phedge[1]->pHEdgePair != NULL&&PFACETTRI(phedge[1]->pHEdgePair)->beused == 0)
				{
					if (phedge[1]->pHEdgePair->pHEdgePre->pHEdgePair != NULL&&phedge[1]->pHEdgePair->pHEdgeNext->pHEdgePair != NULL)
					{
						PVERT p1 = new VERT;
						p1->x = phedge[0]->pVertEnd->x;
						p1->y = phedge[0]->pVertEnd->y;
						p1->z = phedge[0]->pVertEnd->z;
						CalerrorLittleValue(PFACETTRI(phedge[1]->pHEdgePair->pFacetAdj), p1);
					}
				}
			}
		}
	}
	///////////////////////////////计算最大和最小尖锐度//////////////////
	Lsmin = Lsmax = Lshapedegree(vecPfact[0]);
	for (int i = 0; i < vecPfact.size(); i++)
	{
		vecPfact[i]->beused = 0;
		double t = Lshapedegree(vecPfact[i]);
		Kit->Ranktwodigit(Lsmin, t);
		Kit->Ranktwodigit(t, Lsmax);
	}
}
BOOL CSimplify::Judgelegal(PFACETTRI PFc0, vector<PFACETTRI> &vecper)
{
	VECTOR3D V0; double angel; 
	vecper = PFc0->vec_FOneFac; 
	for (int i = 0; i < vecper.size(); i++)
	{
		if (vecper[i]->beused || vecper[i]->befrost)
		{
			return FALSE;
		}
	}
	for (int i = 0; i < vecper.size(); i++)
	{
		PHEDGE pHedge[3];
		pHedge[0] = vecper[i]->pHEdge; pHedge[1] = vecper[i]->pHEdge->pHEdgePre;
		pHedge[2] = vecper[i]->pHEdge->pHEdgeNext;
		for (int j = 0; j < 3; j++)
		{
			////////////////////判断面片的一阶邻域面片折叠前后法矢有无明显翻转///////////////
			for (int k = 0; k < 3; k++)
			{
				int nct = (k + 1) % 3;    int ncnt = (k + 2) % 3;

				if (PFc0->m_PVerts[j] == vecper[i]->m_PVerts[k])
				{
					V0 = *Kit->CalcuPfacNormal(vecper[i]->m_PVerts[nct], vecper[i]->m_PVerts[ncnt], PFc0->Pvet);
				}
			}
		}
		angel = acos(V0 | (*vecper[i]->m_PFacetNorm));
		if (angel>(PI / 12))
		{
			return FALSE;
		}
	}
	return TRUE;
}
void CSimplify::CalerrorLittleValue(PFACETTRI Pf0, PVERT p0)
{
	vector<PFACETTRI> vec1, vec2;//一阶领域面片
	Kit->FindPOneRFAC(Pf0, vec1);//所有一阶领域面片
	Kit->FindPOneRFAC_NEW(Pf0, vec2);
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
			Pf0->vec_FOneFac.push_back(vec1[m]);
		}
	}
	double L = Longdegree(Pf0);
	double LRA = SumRingPfacArea(Pf0);
	double LRS = Lshapedegree(Pf0);
	double t = (L*LRS) / LRA;
	for (int j = 0; j < Pf0->vec_FOneFac.size(); j++)
	{
		vector<double> equ(16, int(0));
		vector<double> dis;
		Planeeuate(Pf0->vec_FOneFac[j], dis);
		equ[0] = t*dis[0] * dis[0]; equ[1] = t*dis[0] * dis[1]; equ[2] = t*dis[0] * dis[2]; equ[3] = t*dis[0] * dis[3];
		equ[4] = t*dis[1] * dis[0]; equ[5] = t*dis[1] * dis[1]; equ[6] = t*dis[1] * dis[2]; equ[7] = t*dis[1] * dis[3];
		equ[8] = t*dis[2] * dis[0]; equ[9] = t*dis[2] * dis[1]; equ[10] = t*dis[2] * dis[2]; equ[11] = t*dis[2] * dis[3];
		equ[12] = t*dis[3] * dis[0]; equ[13] = t*dis[3] * dis[1]; equ[14] = t*dis[3] * dis[2]; equ[15] = t*dis[3] * dis[3];
		vector<double> tew(4, int(0));
		tew[0] = p0->x*equ[0] + p0->y*equ[4] + p0->z*equ[8] + equ[12];
		tew[1] = p0->x*equ[1] + p0->y*equ[5] + p0->z*equ[9] + equ[13];
		tew[2] = p0->x*equ[2] + p0->y*equ[6] + p0->z*equ[10] + equ[14];
		tew[3] = p0->x*equ[3] + p0->y*equ[7] + p0->z*equ[11] + equ[15];
		double value = p0->x*tew[0] + p0->y*tew[1] + p0->z*tew[2] + tew[3];
		if (j == 0)  Pf0->value = p0->x*tew[0] + p0->y*tew[1] + p0->z*tew[2] + tew[3];
		Kit->Ranktwodigit(value, Pf0->value);
	}
	Pf0->beused = 1;
}
void CSimplify::Checkparticular(PFACETTRI &PFc0, double t0)
{
	double L0 = Lshapedegree(PFc0);
	if (L0 > t0)
	{
		vector<double> re; re.clear();
		for (int i = 0; i < 3; i++)
		{
			re.push_back(Kit->CalGaussCurvature(PFc0->m_PVerts[i]));
		}
		Kit->Rankthreedigit(PFc0, fabs(re[0]), fabs(re[1]), fabs(re[2]));
	}
}

double CSimplify::Longdegree(PFACETTRI PFact)
{
	VECTOR3D V1, V2;
	double L = 0.5;
	PVERT PVert[3];

	PVert[0] = PFact->pHEdge->pVertEnd;
	PVert[1] = PFact->pHEdge->pHEdgeNext->pVertEnd;
	PVert[2] = PFact->pHEdge->pHEdgePre->pVertEnd;
	for (int nct = 0; nct < 3; nct++)
	{
		int ncnt = (nct + 1) % 3, ncrt = (nct + 2) % 3;
		V1.dx = PVert[nct]->x - PVert[ncrt]->x;
		V1.dy = PVert[nct]->y - PVert[ncrt]->y;
		V1.dz = PVert[nct]->z - PVert[ncrt]->z;

		V2.dx = PVert[nct]->x - PVert[ncnt]->x;
		V2.dy = PVert[nct]->y - PVert[ncnt]->y;
		V2.dz = PVert[nct]->z - PVert[ncnt]->z;
		V1.Normalize(); V2.Normalize();
		double t = V1 | V2;
		if (t >= L&&t < 1)
		{
			L = t;
		}
	}
	L = 2 * (1 - L);
	return L;
}
double CSimplify::SumRingArea(PVERT P0)
{
	double S = 0;
	vec_PFACETTRI vec_Pfact = P0->vec_OneFac;

	for (int i = 0; i < vec_Pfact.size(); i++)
	{
		PVERT PVert[3];
		PVert[0] = vec_Pfact[i]->pHEdge->pVertEnd;
		PVert[1] = vec_Pfact[i]->pHEdge->pHEdgeNext->pVertEnd;
		PVert[2] = vec_Pfact[i]->pHEdge->pHEdgePre->pVertEnd;
		auto stemp = [](PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
		S += stemp(PVert[0], PVert[1], PVert[2]);
		//S += Kit->AreaTri(PVert[0], PVert[1], PVert[2]);
	}
	vec_Pfact.clear();
	return S;
}
double CSimplify::Lshapedegree(PFACETTRI PFact)
{
	double LR = 0; VECTOR3D V1;
	vec_PFACETTRI vec_Pfact;

	Kit->FindPOneRFAC(PFact, vec_Pfact);
	VECTOR3D Vnormal = *PFact->m_PFacetNorm;
	Vnormal.Normalize();
	for (int i = 0; i < vec_Pfact.size(); i++)
	{
		V1 = *vec_Pfact[i]->m_PFacetNorm;
		V1.Normalize();
		LR += 1 - (Vnormal | V1);
	}
	LR = LR / (vec_Pfact.size());
	vec_Pfact.clear();
	return LR;
}
double CSimplify::SumRingPfacArea(PFACETTRI PFact)
{
	double S = 0;
	vec_PFACETTRI vec_Pfact;

	Kit->FindPOneRFAC(PFact, vec_Pfact);
	for (int i = 0; i < vec_Pfact.size(); i++)
	{
		PVERT PVert[3];
		PVert[0] = vec_Pfact[i]->pHEdge->pVertEnd;
		PVert[1] = vec_Pfact[i]->pHEdge->pHEdgeNext->pVertEnd;
		PVert[2] = vec_Pfact[i]->pHEdge->pHEdgePre->pVertEnd;
		auto stemp = [](PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3){return 0.5*((*pPoint2 - *pPoint1)*(*pPoint3 - *pPoint1)).GetLength(); };
		S += stemp(PVert[0], PVert[1], PVert[2]);
		//S += Kit->AreaTri(PVert[0], PVert[1], PVert[2]);
	}
	vec_Pfact.clear();
	return S;
}
void CSimplify::AcqnewVertex(PFACETTRI PFact, double &value)
{

	PVERT P0, P1, P2;
	P0 = PFact->pHEdge->pVertEnd;
	P1 = PFact->pHEdge->pHEdgePre->pVertEnd;
	P2 = PFact->pHEdge->pHEdgeNext->pVertEnd;
	double L1, L2, L3, LA1, LA2, LA3;
	LA1 = SumRingArea(P0);
	LA2 = SumRingArea(P1);
	LA3 = SumRingArea(P2);
	L1 = LA1 / (LA1 + LA2 + LA3);
	L2 = LA2 / (LA1 + LA2 + LA3);
	L3 = LA3 / (LA1 + LA2 + LA3);
	PFact->Pvet = new VERT;
	PFact->Pvet->x = L1*P0->x + L2*P1->x + L3*P2->x;
	PFact->Pvet->y = L1*P0->y + L2*P1->y + L3*P2->y;
	PFact->Pvet->z = L1*P0->z + L2*P1->z + L3*P2->z;
	CalerrorLittleValue(PFact, PFact->Pvet);
}
void CSimplify::AcqnewVertex_curdegree(PFACETTRI Pfact)
{
	//////////////曲度求内部三角形新折叠点////////
	VERT p0, p1, p2;
	Pfact->Pvet = new VERT;
	double cdrgee = 0;
	for (int j = 0; j < 3; j++)
	{
		if (!Pfact->m_PVerts[j]->cdrgee)
		{
			Pfact->m_PVerts[j]->cdrgee = Kit->CalcuPointCurve(Pfact->m_PVerts[j]);
		}
		cdrgee += Pfact->m_PVerts[j]->cdrgee;
	}
	p0 = *Pfact->m_PVerts[0]; p1 = *Pfact->m_PVerts[1]; p2 = *Pfact->m_PVerts[2];
	Pfact->Pvet->x = p0.cdrgee / cdrgee*p0.x + p1.cdrgee / cdrgee*p1.x + p2.cdrgee / cdrgee*p2.x;
	Pfact->Pvet->y = p0.cdrgee / cdrgee*p0.y + p1.cdrgee / cdrgee*p1.y + p2.cdrgee / cdrgee*p2.y;
	Pfact->Pvet->z = p0.cdrgee / cdrgee*p0.z + p1.cdrgee / cdrgee*p1.z + p2.cdrgee / cdrgee*p2.z;
}
void CSimplify::CalValue_pointplane(PFACETTRI Pfact, vec_PFACETTRI vecper)
{
	vector<double> dista;
	for (int i = 0; i < vecper.size(); i++)
	{
		VECTOR3D u;
		POINT3D p00, p01;
		p00 = POINT3D(vecper[i]->m_PVerts[0]->x, vecper[i]->m_PVerts[0]->y, vecper[i]->m_PVerts[0]->z);
		p01 = POINT3D(Pfact->Pvet->x, Pfact->Pvet->y, Pfact->Pvet->z);
		u = Kit->PlanePointDistance(p00, p01, *vecper[i]->m_PFacetNorm);
		dista.push_back(u.GetLength());
	}

	Pfact->value = dista[0];
	for (int j = 0; j < vecper.size(); j++)
	{
		Kit->Ranktwodigit(dista[j], Pfact->value);
	}
}
void CSimplify::CalValue_Wmsa(PFACETTRI pFac)
{
	POINT3D pvert;//三角面片的重心
	pvert.x = (pFac->m_PVerts[0]->x + pFac->m_PVerts[1]->x + pFac->m_PVerts[2]->x)*1.0 / 3;
	pvert.y = (pFac->m_PVerts[0]->y + pFac->m_PVerts[1]->y + pFac->m_PVerts[2]->y)*1.0 / 3;
	pvert.z = (pFac->m_PVerts[0]->z + pFac->m_PVerts[1]->z + pFac->m_PVerts[2]->z)*1.0 / 3;
	PVERT pVer = new VERT; pVer->x = pvert.x; pVer->y = pvert.y; pVer->z = pvert.z;
	
	VECTOR3D Certer_Normal = Kit->CalcuVerNormal(pVer, pFac);//估算三角形重心的法矢

	POINT3D p00 = POINT3D(pFac->Pvet->x, pFac->Pvet->y, pFac->Pvet->z);
	VECTOR3D vTemp = Kit->PlanePointDistance(pvert, p00, Certer_Normal);
	pFac->value = vTemp.GetLength();
 }
void CSimplify::Planeeuate(PFACETTRI PFc0, vector<double> &facepare)
{
	POINT3D p1, p2, p3;
	double a, b, c, d;

	p1 = *PFc0->pHEdge->pVertEnd;
	p2 = *PFc0->pHEdge->pHEdgePre->pVertEnd;
	p3 = *PFc0->pHEdge->pHEdgeNext->pVertEnd;
	a = ((p2.y - p1.y)*(p3.z - p1.z) - (p2.z - p1.z)*(p3.y - p1.y));
	b = ((p2.z - p1.z)*(p3.x - p1.x) - (p2.x - p1.x)*(p3.z - p1.z));
	c = ((p2.x - p1.x)*(p3.y - p1.y) - (p2.y - p1.y)*(p3.x - p1.x));
	VECTOR3D v1 = VECTOR3D(a, b, c);	v1.Normalize();
	a = v1.dx;	b = v1.dy;	c = v1.dz;
	d = (0 - (a*p1.x + b*p1.y + c*p1.z));
	facepare.push_back(a);
	facepare.push_back(b);
	facepare.push_back(c);
	facepare.push_back(d);

//	Matrix<double, 3, 3> equ;    // 矩阵中元素值初始为0
//	equ = Matrix<double, 3, 3>::Zero();
//	Matrix<double, 3, 1>  B;
//	B = Matrix<double, 3, 1>::Zero();
//#pragma omp parallel for
//	for (int i = 0; i < 3; i++)
//	{
//		PVERT pPt = PFc0->m_PVerts[i];
//		float u = pPt->x; float v = pPt->y; float h = pPt->z;             //邻点坐标值u,v,h
//
//		//利用邻点的坐标通过最小二乘法拟合平面
//		//解线性方程组
//		equ(0, 0) += u*u; equ(0, 1) += u*v; equ(0, 2) += u;
//
//		equ(1, 0) += u*v; equ(1, 1) += v*v; equ(1, 2) += v;
//
//		equ(2, 0) += u; equ(2, 1) += v; equ(2, 2) += 1;
//
//
//		B(0, 0) += u *h;  B(1, 0) += v * h;    B(2, 0) += h;
//	}
//
//	Matrix<double, 3, 1>  solu;
//	solu = Matrix<double, 3, 1>::Zero();
//	solu = equ.lu().solve(B);
//	double a, b, d;
//	a = solu(0, 0); b = solu(1, 0); d = solu(2, 0);
//
//	facepare.push_back(a);
//	facepare.push_back(b);
//	facepare.push_back(-1);
//	facepare.push_back(d);

}
void CSimplify::CalValue_Fitcurve(PFACETTRI pFac)
{
	POINT3D pvert;//三角面片的重心
	pvert.x = (pFac->m_PVerts[0]->x + pFac->m_PVerts[1]->x + pFac->m_PVerts[2]->x)*1.0 / 3;
	pvert.y = (pFac->m_PVerts[0]->y + pFac->m_PVerts[1]->y + pFac->m_PVerts[2]->y)*1.0 / 3;
	pvert.z = (pFac->m_PVerts[0]->z + pFac->m_PVerts[1]->z + pFac->m_PVerts[2]->z)*1.0 / 3;

	Matrix<double, 6, 6> equ;    // 矩阵中元素值初始为0
	equ = Matrix<double, 6, 6>::Zero();
	Matrix<double, 6, 1>  B;
	B = Matrix<double, 6, 1>::Zero();
	vec_PVERT     vecpVer;
	Kit->FindPOneRVetex(pFac, vecpVer);
#pragma omp parallel for
	for (int i = 0; i < vecpVer.size(); i++)
	{
		PVERT pPt = vecpVer[i];
		float u = pPt->x; float v = pPt->y; float h = pPt->z;             //邻点坐标值u,v,h

		//利用邻点的坐标通过最小二乘法拟合二次曲面
		//解线性方程组
		equ(0, 0) += u*u*u*u; equ(0, 1) += u*u*u*v; equ(0, 2) += u*u*v*v;
		equ(0, 3) += u*u*u;     equ(0, 4) += u*u*v;     equ(0, 5) += u*u;

		equ(1, 0) += u*u*u*v; equ(1, 1) += u*u*v*v; equ(1, 2) += u*v*v*v;
		equ(1, 3) += u*u*v;     equ(1, 4) += u*v*v;     equ(1, 5) += u*v;

		equ(2, 0) += u*u*v*v; equ(2, 1) += u*v*v*v; equ(2, 2) += v*v*v*v;
		equ(2, 3) += u*v*v;     equ(2, 4) += v*v*v;     equ(2, 5) += v*v;

		equ(3, 0) += u*u*u;     equ(3, 1) += u*u*v;     equ(3, 2) += u*v*v;
		equ(3, 3) += u*u;         equ(3, 4) += u*v;         equ(3, 5) += u;

		equ(4, 0) += u*u*v;     equ(4, 1) += u*v*v;     equ(4, 2) += v*v*v;
		equ(4, 3) += u*v;         equ(4, 4) += v*v;         equ(4, 5) += v;

		equ(5, 0) += u*u;         equ(5, 1) += u*v;         equ(5, 2) += v*v;
		equ(5, 3) += u;             equ(5, 4) += v;             equ(5, 5) += 1;

		B(0, 0) += u * u * h;  B(1, 0) += u * v * h;    B(2, 0) += v * v * h;
		B(3, 0) += u * h;        B(4, 0) += v*h;            B(5, 0) += h;
	}

	Matrix<double, 6, 1>  solu;
	solu = Matrix<double, 6, 1>::Zero();
	solu = equ.lu().solve(B);
	double a, b, c, d, e, f;
	a = solu(0, 0); b = solu(1, 0); c = solu(2, 0); d = solu(3, 0); e = solu(4, 0); f = solu(5, 0);
	// ec_KDPoint.clear();
	VECTOR3D Certer_Normal;
	Certer_Normal.dx = 2 * a*pvert.x + b*pvert.y + d; Certer_Normal.dy = b*pvert.x + 2 * c*pvert.y + e; Certer_Normal.dz = -1;
	Certer_Normal = (Certer_Normal.GetNormal())*(-1);//求得搜索中心的的法矢
	POINT3D p00 = POINT3D(pFac->Pvet->x, pFac->Pvet->y, pFac->Pvet->z);
	VECTOR3D vTemp = Kit->PlanePointDistance(pvert, p00, Certer_Normal);
	pFac->value = vTemp.GetLength();
}
void CSimplify::CalValue_Fitplane(PFACETTRI pFac)
{
	POINT3D pvert;//三角面片的重心
	pvert.x = (pFac->m_PVerts[0]->x + pFac->m_PVerts[1]->x + pFac->m_PVerts[2]->x)*1.0 / 3;
	pvert.y = (pFac->m_PVerts[0]->y + pFac->m_PVerts[1]->y + pFac->m_PVerts[2]->y)*1.0 / 3;
	pvert.z = (pFac->m_PVerts[0]->z + pFac->m_PVerts[1]->z + pFac->m_PVerts[2]->z)*1.0 / 3;

	Matrix<double, 3, 3> equ;    // 矩阵中元素值初始为0
	equ = Matrix<double, 3, 3>::Zero();
	Matrix<double, 3, 1>  B;
	B = Matrix<double, 3, 1>::Zero();
	vec_PVERT     vecpVer;
	Kit->FindPOneRVetex(pFac, vecpVer);
#pragma omp parallel for
	for (int i = 0; i < vecpVer.size(); i++)
	{
		PVERT pPt = vecpVer[i];
		float u = pPt->x; float v = pPt->y; float h = pPt->z;             //邻点坐标值u,v,h

		//利用邻点的坐标通过最小二乘法拟合二次曲面
		//解线性方程组
		equ(0, 0) += u*u; equ(0, 1) += u*v; equ(0, 2) += u;

		equ(1, 0) += u*v; equ(1, 1) += v*v; equ(1, 2) += v;

		equ(2, 0) += u; equ(2, 1) += v; equ(2, 2) += 1;


		B(0, 0) += u *h;  B(1, 0) += v * h;    B(2, 0) += h;
	}

	Matrix<double, 3, 1>  solu;
	solu = Matrix<double, 3, 1>::Zero();
	solu = equ.lu().solve(B);
	double a, b, c;
	a = solu(0, 0); b = solu(1, 0); c = solu(2, 0);
	// ec_KDPoint.clear();
	VECTOR3D Certer_Normal;
	Certer_Normal.dx = a; Certer_Normal.dy = b; Certer_Normal.dz = -1;
	Certer_Normal = (Certer_Normal.GetNormal())*(-1);//求得搜索中心的的法矢
	POINT3D p00 = POINT3D(pFac->Pvet->x, pFac->Pvet->y, pFac->Pvet->z);
	VECTOR3D vTemp = Kit->PlanePointDistance(pvert, p00, Certer_Normal);
	pFac->value = vTemp.GetLength();
}
void CSimplify::CalValue_QEM(PFACETTRI pFac, vec_PFACETTRI vecper)
{
	//MatrixXd V(4, 1), R(1, 1);
	//V << pFac->Pvet->x, pFac->Pvet->y,
	//	pFac->Pvet->z, 1;
	//vector<MatrixXd> MR;
	//for (int i = 0; i < vecper.size(); i++)
	//{
	//	MatrixXd A(4, 1), M(4, 4);
	//	A = Planeeuate(vecper[i]);
	//	M << A(0, 0)*A(0, 0), A(0, 0)*A(1, 0), A(0, 0)*A(2, 0), A(0, 0)*A(3, 0),
	//		A(0, 0)*A(1, 0), A(1, 0)*A(1, 0), A(1, 0)*A(2, 0), A(1, 0)*A(3, 0),
	//		A(0, 0)*A(2, 0), A(1, 0)*A(2, 0), A(2, 0)*A(2, 0), A(2, 0)*A(3, 0),
	//		A(0, 0)*A(3, 0), A(1, 0)*A(3, 0), A(2, 0)*A(3, 0), A(3, 0)*A(3, 0);
	//	MR.push_back(M);
	//}
	//R = V.transpose()*MR[0] * V;
	//pFac->value = R(0, 0);
	//for (int j = 0; j < vecper.size(); j++)
	//{
	//	R = V.transpose()*MR[j] * V;
	//	Kit->Ranktwodigit(R(0, 0), pFac->value);
	//}

	vector<double> equ(16, int(0));
	for (int i = 0; i < vecper.size(); i++)
	{
		vector<double> dis;
		Planeeuate(vecper[i], dis);
		equ[0] += dis[0] * dis[0]; equ[1] += dis[0] * dis[1]; equ[2] += dis[0] * dis[2]; equ[3] += dis[0] * dis[3];
		equ[4] += dis[1] * dis[0]; equ[5] += dis[1] * dis[1]; equ[6] += dis[1] * dis[2]; equ[7] += dis[1] * dis[3];
		equ[8] += dis[2] * dis[0]; equ[9] += dis[2] * dis[1]; equ[10] += dis[2] * dis[2]; equ[11] += dis[2] * dis[3];
		equ[12] += dis[3] * dis[0]; equ[13] += dis[3] * dis[1]; equ[14] += dis[3] * dis[2]; equ[15] += dis[3] * dis[3];
	}
	vector<double> tew(4, int(0));
	tew[0] = pFac->Pvet->x*equ[0] + pFac->Pvet->y*equ[4] + pFac->Pvet->z*equ[8] + equ[12];
	tew[1] = pFac->Pvet->x*equ[1] + pFac->Pvet->y*equ[5] + pFac->Pvet->z*equ[9] + equ[13];
	tew[2] = pFac->Pvet->x*equ[2] + pFac->Pvet->y*equ[6] + pFac->Pvet->z*equ[10] + equ[14];
	tew[3] = pFac->Pvet->x*equ[3] + pFac->Pvet->y*equ[7] + pFac->Pvet->z*equ[11] + equ[15];
	pFac->value = pFac->Pvet->x*tew[0] + pFac->Pvet->y*tew[1] + pFac->Pvet->z*tew[2] + tew[3];
}

void CSimplify::AcqnewVertex_Oneofthree(PFACETTRI Pfact, vec_PFACETTRI vecper)
{
	vector<double> dis_0;
	PVERT p0 = new VERT;
	for (int i = 0; i < 3;i++)
	{
		Pfact->Pvet = Pfact->m_PVerts[i];
		CalValue_pointplane(Pfact,vecper);
		dis_0.push_back(Pfact->value);
	}
	if (dis_0[0] < dis_0[1])
	{
		if (dis_0[0] < dis_0[2])
		{
			Pfact->Pvet = Pfact->m_PVerts[0];
			//Pfact->value = dis_0[0];
		}
		else
		{
			Pfact->Pvet = Pfact->m_PVerts[2];
			//Pfact->value = dis_0[2];
		}
	}
	else
	{
		if (dis_0[1]< dis_0[2])
		{
			Pfact->Pvet = Pfact->m_PVerts[1];
			//Pfact->value = dis_0[1];
		}
		else
		{
			Pfact->Pvet = Pfact->m_PVerts[2];
			//Pfact->value = dis_0[2];
		}
	}
	Pfact->value = 0;
}
void CSimplify::AcqnewVertex_Gravity(PFACETTRI Pfact)
{
	Pfact->Pvet = new VERT;
	Pfact->Pvet->x = (Pfact->m_PVerts[0]->x + Pfact->m_PVerts[1]->x + Pfact->m_PVerts[2]->x)*1.0 / 3;
	Pfact->Pvet->y = (Pfact->m_PVerts[0]->y + Pfact->m_PVerts[1]->y + Pfact->m_PVerts[2]->y)*1.0 / 3;
	Pfact->Pvet->z = (Pfact->m_PVerts[0]->z + Pfact->m_PVerts[1]->z + Pfact->m_PVerts[2]->z)*1.0 / 3;
}

