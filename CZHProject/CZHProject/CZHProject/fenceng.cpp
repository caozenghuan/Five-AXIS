
#include "fenceng.h"

fenceng::fenceng()
{
	
}


fenceng::~fenceng()
{
	delete kit;
}
void fenceng::GetBlockFillResult(vector<vec_PPOLYPOLYGON> Blocklunkuo, vector<VECTOR3D> All_VecDir, vector<vec_PPOLYPOLYGON> &BlockFill, QProgressBar  *progressBar, double FillDia, int Fillway)
{
	if (Blocklunkuo.size() < 1) return;

	vec_PPOLYPOLYGON lunkuo;
	PPOLYPOLYGON PloyPloyTemp;
	progressBar->setRange(0, Blocklunkuo.size() - 1);
	progressBar->setValue(-1);
	for (int i = 0; i < Blocklunkuo.size(); i++)
	{
		lunkuo.clear();
		progressBar->setValue(i);
		switch (Fillway)
		{
		case OFFSETFILL:
		{
			Coffset::GetOffset(Blocklunkuo[i], All_VecDir[i], lunkuo, FillDia);
			break;
		}
		case LINFILL:
		{
			CLine::GetLine(Blocklunkuo[i], All_VecDir[i], lunkuo, FillDia);
			break;
		}
		
		case MIXFILL:
		{
			CMix::GetMixFill(Blocklunkuo[i], All_VecDir[i], lunkuo, FillDia);
			break;
		}
		case SPINFILL:
		{
			Coffset::GetOffset(Blocklunkuo[i], All_VecDir[i], lunkuo, FillDia);
			ChelicalPath::GetheilcalPath(lunkuo);
			break;
		}
		case UNICURSALFILL:
		{
			Cunicursal::Getunicursal(Blocklunkuo[i], All_VecDir[i], lunkuo, FillDia);
		}
		}
		KitTool::RotatePolygon(Blocklunkuo[i], VECTOR3D(0, 0, 1), All_VecDir[i]);//将分层轮廓旋转回来
		BlockFill.push_back(lunkuo);
	}
}
void fenceng::GetFillResult(vec_PPOLYPOLYGON lunkuo, VECTOR3D Vec_Dir, vec_PPOLYPOLYGON &FillResult, QProgressBar  *progressBar, double FillDia, int Fillway)
{
	if (lunkuo.size() < 1) return;
	switch (Fillway)
	{
	case OFFSETFILL://
	{
		Coffset::GetOffset(lunkuo, Vec_Dir, FillResult, FillDia);
		break;
	}
	case LINFILL://
	{
		CLine::GetLine(lunkuo, Vec_Dir, FillResult, FillDia);
		break;
	}
	case MIXFILL:
	{
		CMix::GetMixFill(lunkuo, Vec_Dir, FillResult, FillDia);
		break;
	}
	case SPINFILL://
	{
		Coffset::GetOffset(lunkuo, Vec_Dir, FillResult, FillDia);
		ChelicalPath::GetheilcalPath(FillResult);
		break;
	}
	case UNICURSALFILL:
	{
		Cunicursal::Getunicursal(lunkuo, Vec_Dir, FillResult, FillDia);
	}
	}
}
void fenceng::GetEvenslice(STLModell* pSTL, vec_PPOLYPOLYGON &lunkuo, VECTOR3D DirV, float Even_Thickness, QProgressBar  *progressBar)
{
	if (!pSTL)
		return;
	if (!lunkuo.empty())
	{
		clear(lunkuo);
	}
	STLModell::CreateEdgeTopByFctVert(pSTL);
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	RotateModel(pSTL, DirV, DirZ);

	PPOLYPOLYGON ppolypolygon;
	pSTL->UpdateBox();
	pSTL->GetBox();
	int layer = 0;
	int n = (int)(pSTL->ONE.z1 - pSTL->ONE.z0) / Even_Thickness;
	progressBar->setRange(0, n - 1);
	progressBar->setValue(-1);
	float thickness_h = Even_Thickness;
	while (pSTL->ONE.z0 + thickness_h < pSTL->ONE.z1)
	{
		progressBar->setValue(layer);
		ppolypolygon = GetHighSlice(pSTL, thickness_h);
		ppolypolygon->UpdateBox();
		lunkuo.push_back(ppolypolygon);
		thickness_h += Even_Thickness;
		layer++;
	}
	//Caleachlayerboundarypoint(pSTL,lunkuo,Even_Thickness,progressBar);

	RotateModel(pSTL, DirZ, DirV);
	RotatesliceModel(lunkuo, DirZ, DirV);
}
void fenceng::GetBlockEvenslice(vector<MyBlock> &Rbox, vector<vec_PPOLYPOLYGON> &lunkuo, vector<VECTOR3D> &All_VecDir, float Even_Thickness, QProgressBar  *progressBar)
{
	if (All_VecDir.size() == 0) return;
	vector<Blockpath> AllBlock;
	bool ifMb = false;
	for (int i = 0; i < Rbox.size();i++)
	{
		(i == 1) ? ifMb = false : ifMb = true;
		Blockpath pMeshTri; 
		CreateEdgeTopByFctVert(pMeshTri, Rbox[i].OneBlock);
		FindBlockHole(pMeshTri, Rbox[i].OneBlock,All_VecDir[i],ifMb);///块补洞/////////
		Rbox[i].OneBlock = pMeshTri.m_vecPFacetTri;
		AllBlock.push_back(pMeshTri);
	}
	////块分层1////////////
	//int n = AllBlock.size();
	//progressBar->setRange(0, n - 1);
	//progressBar->setValue(-1);
	//VECTOR3D DirS;
	//BOOL IfFir = true;
	//for (int i = 0; i < n; i++)
	//{
	//	progressBar->setValue(i);
	//	vec_PPOLYPOLYGON lunkuoTemp;
	//	DirS = All_VecDir[i];
	//	//if (FindSupportBlock(AllBlock[i]) && Rbox[i].vec_PairBlock.size()<3)  DirS = DirZ;//根据实际情况，优化最优方向
	//	if (i != 0) IfFir = false;
	//	Caleachlayerboundarypoint(AllBlock[i], lunkuoTemp, IfFir, DirS, Even_Thickness, progressBar);

	//	WipeErrorLayer(lunkuoTemp);
	//	lunkuo.push_back(lunkuoTemp);
	//}


	//块分层2////////////
	int n = AllBlock.size();
	progressBar->setRange(0, n - 1);
	progressBar->setValue(-1);
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	VECTOR3D DirS;
	for (int i = 0; i < n;i++)
	{
		progressBar->setValue(i);
		vec_PPOLYPOLYGON lunkuoTemp;
		DirS = All_VecDir[i];
		RotateBlockModel(AllBlock[i], DirS, DirZ);
		//if (FindSupportBlock(AllBlock[i]) && Rbox[i].vec_PairBlock.size()<3)  DirS = DirZ;//根据实际情况，优化最优方向
		
		PPOLYPOLYGON ppolypolygon;
		BOX3D ONE;
		UpdateBlockBox(AllBlock[i], ONE);

		float thickness_h = Even_Thickness;
		while (ONE.z0 + thickness_h < ONE.z1)
		{

			ppolypolygon = GetHighSlice(AllBlock[i], thickness_h, ONE.z0);
			ppolypolygon->UpdateBox();
			lunkuoTemp.push_back(ppolypolygon);
			thickness_h += Even_Thickness;
		}
		//CaleachBlockboundarypoint(AllBlock[i], lunkuoTemp, Even_Thickness);
		RotatesliceModel(lunkuoTemp, DirZ, DirS);
		RotateBlockModel(AllBlock[i], DirZ, DirS);
		WipeErrorLayer(lunkuoTemp);
		lunkuo.push_back(lunkuoTemp);
	}
	///////////////清空链表/////////////////
	for (int i = 0;i<AllBlock.size();i++)
	{
		for (int nct = 0; nct < AllBlock[i].m_vecPEdge.size();nct++)
		{
			delete AllBlock[i].m_vecPEdge[nct];
		}
		AllBlock[i].m_vecPEdge.clear();

		for (int nct = 0; nct < AllBlock[i].m_vecPHEdge.size(); nct++)
		{
			delete AllBlock[i].m_vecPHEdge[nct];
		}
		AllBlock[i].m_vecPHEdge.clear();
		/*for (int nct = 0; nct < AllBlock[i].m_vecPFacetTri.size(); nct++)
		{
		delete AllBlock[i].m_vecPFacetTri[nct];
		}
		AllBlock[i].m_vecPFacetTri.clear();
		for (int nct = 0; nct < AllBlock[i].m_vecPVert.size(); nct++)
		{
		delete AllBlock[i].m_vecPVert[nct];
		}
		AllBlock[i].m_vecPVert.clear();*/
	}
}
bool fenceng::IfAcuteAngle(VECTOR3D Dir, VECTOR3D Diz)
{
	double angel = Dir | Diz;
	//angel = abs(abs(angel) - 1);
	if (angel<0)
	{
		Dir.dx = -Dir.dx;
		Dir.dy = -Dir.dy;
		Dir.dz = -Dir.dz;
		return true;
	}
	return false;
}
bool fenceng::FindSupportBlock(Blockpath OneBlock)
{
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	VECTOR3D F_Normal; int Nub = 0;
	double OneZero = 0.0038;
	for (int i = 0; i < OneBlock.m_vecPFacetTri.size();i++)//
	{
		F_Normal = *OneBlock.m_vecPFacetTri[i]->m_PFacetNorm;

		double angel = F_Normal | DirZ;
		angel = abs(abs(angel) - 1);
		if (angel<OneZero)
		{
			Nub++;
		}
	}
	if (Nub>10)
	{
		return true;
	}
	return false;
}
void fenceng::FindBlockHole(Blockpath &OneBlock, vector<PFACETTRI> &vec_Pface, VECTOR3D vec_Dir, bool IfMb)
{
	POINT3D Pceshi;
	vec_PHEDGE BordRing;//存储所有的边界边，查找边界边的一阶邻域面片
	for (int i = 0; i < OneBlock.m_vecPEdge.size(); i++)
	{
		PHEDGE Ph = OneBlock.m_vecPEdge[i]->pHEdgeAdj;
		if (!Ph->pHEdgePair)
		{
			BordRing.push_back(Ph);
		}
	}
	if (BordRing.size() == 0) return;
	PPOLYPOLYGON vec_poly; vec_poly = new POLYPOLYGON;
	kit->closedBoundary(BordRing, vec_poly);//给所有的边界顶点排序
	BordRing.clear();
	vector<vector<vector<POINT3D>>> vec_Face;
	vector<vector<POINT3D>> vec_FaceTemp;
	for (int i = 0; i < vec_poly->m_vecPPolygons.size();i++)//判断环是否需补洞
	{
		PPOLYGON ploy = vec_poly->m_vecPPolygons[i];
		int nct = ploy->m_vecPnts.size();
		int nct0 = int(0.3*nct); int nct1 = int(0.6*nct);
		VECTOR3D v0, v1, v2;
		v0 = ploy->m_vecPnts[nct0] - ploy->m_vecPnts[0];
		v1 = ploy->m_vecPnts[nct1] - ploy->m_vecPnts[0];
		bool RoDir = KitTool::judeorientation(vec_poly->m_vecPPolygons[i]);
		if (RoDir)
		{
			v2 = v0*v1;
		}
		else
		{
			v2 = v1*v0;
			KitTool::RankCircle(ploy);
		}
		 v2.Normalize(); vec_Dir.Normalize();
		double angel = v2 | vec_Dir;
		double OneZero = 0.000004;
		angel = abs(abs(angel) - 1);
		if (angel>OneZero)//查看是否平行最优方向
		{
			//HoleRepair(vec_FaceTemp, v2, ploy);//孔洞细分
			//LoopOnehole(v2,ploy,vec_FaceTemp);//偏置方法补洞,成功
			if (IfMb)
			{
				LoopOnehole1(v2, ploy, vec_FaceTemp,Pceshi);//偏置方法补洞，成功
			}
			else
			{
				HoleRepair(vec_FaceTemp, v2, ploy);//孔洞细分
			}
			//LoopOnehole1(v2, ploy, vec_FaceTemp);//偏置方法补洞，成功
			vec_Face.push_back(vec_FaceTemp);
		}
	}
	vec_FaceTemp.clear();
	//delete vec_poly;
	//去除错误面片(三个顶点中有两点坐标相同)
	for (int i = 0; i < vec_Face.size();i++)
	{
		vec_FaceTemp = vec_Face[i];
		bool iffind = false;
		do 
		{
			iffind = false;
			for (int j = 0; j < vec_FaceTemp.size(); j++)
			{
				vec_POINT3D nFace = vec_FaceTemp[j];
				for (int k = 0; k < 3; k++)
				{
					int nrt = (k + 1) % 3;
					if (nFace[k].x == nFace[nrt].x&&nFace[k].y == nFace[nrt].y&&nFace[k].z == nFace[nrt].z)
					{
						vec_FaceTemp.erase(vec_FaceTemp.begin() + j);
						iffind = true; break;
					}
				}
				if (iffind) break;
			}
			vec_Face[i].clear();
			vec_Face[i] = vec_FaceTemp;
		} while (iffind);
		vec_FaceTemp.clear();
	}

	//更新块面片信息
	if (vec_Face.size()!=0)
	{
		Blockpath BlockTemp;
		map<VERT, int>map_Vert; int VetxID = 0;
		map<VERT, int>::iterator iter;
		PVERT p0;
		for (int i = 0; i < OneBlock.m_vecPFacetTri.size();i++)//存入之前的面片
		{
			PFACETTRI PF0; PF0 = new FACETTRI;
			for (int j = 0; j < 3;j++)
			{
				p0 = new VERT;
				p0->x = OneBlock.m_vecPFacetTri[i]->m_PVerts[j]->x;
				p0->y = OneBlock.m_vecPFacetTri[i]->m_PVerts[j]->y;
				p0->z = OneBlock.m_vecPFacetTri[i]->m_PVerts[j]->z;
				iter = map_Vert.find(*p0);
				if (iter != map_Vert.end())//已存在该点
				{
					int Num = iter->second;
					BlockTemp.m_vecPVert[Num]->vec_OneFac.push_back(PF0);
					PF0->m_PVerts[j] = BlockTemp.m_vecPVert[Num];
				}
				else
				{
					p0->VertexID = VetxID;
					p0->vec_OneFac.push_back(PF0);
					map_Vert.insert(pair<VERT, int>(*p0, BlockTemp.m_vecPVert.size()));
					BlockTemp.m_vecPVert.push_back(p0);
					PF0->m_PVerts[j] = p0;
					VetxID++;
				}
			}
			PF0->m_PFacetNorm = kit->CalcuPfacNormal(PF0->m_PVerts[0], PF0->m_PVerts[1], PF0->m_PVerts[2]);///求面片的法矢
			BlockTemp.m_vecPFacetTri.push_back(PF0);
		}
		//存入新细分的面片
		for (int i = 0; i < vec_Face.size(); i++)//存入之前的面片
		{
			vec_FaceTemp = vec_Face[i];
			for (int j = 0; j < vec_FaceTemp.size();j++)
			{
				PFACETTRI PF0; PF0 = new FACETTRI;
				for (int k = 0; k < vec_FaceTemp[j].size(); k++)
				{
					p0 = new VERT;
					p0->x = vec_FaceTemp[j][k].x; p0->y = vec_FaceTemp[j][k].y; p0->z = vec_FaceTemp[j][k].z;
					iter = map_Vert.find(*p0);
					if (iter != map_Vert.end())//已存在该点
					{
						int Num = iter->second;
						BlockTemp.m_vecPVert[Num]->vec_OneFac.push_back(PF0);
						PF0->m_PVerts[k] = BlockTemp.m_vecPVert[Num];
					}
					else
					{
						p0->VertexID = VetxID;
						p0->vec_OneFac.push_back(PF0);
						map_Vert.insert(pair<VERT, int>(*p0, BlockTemp.m_vecPVert.size()));
						BlockTemp.m_vecPVert.push_back(p0);
						PF0->m_PVerts[k] = p0;
						VetxID++;
					}
				}
				PF0->m_PFacetNorm = kit->CalcuPfacNormal(PF0->m_PVerts[0], PF0->m_PVerts[1], PF0->m_PVerts[2]);///求面片的法矢
				BlockTemp.m_vecPFacetTri.push_back(PF0);
			}
		}
		KitTool::ClearTheList(OneBlock); vec_Face.clear(); vec_FaceTemp.clear();//清空容器
		OneBlock.m_vecPFacetTri = BlockTemp.m_vecPFacetTri;
		OneBlock.m_vecPVert = BlockTemp.m_vecPVert;

		BlockTemp.m_vecPFacetTri.clear(); BlockTemp.m_vecPVert.clear();

		///////重新创建拓扑信息
		kit->CreateEdgeTopByFctVert(OneBlock);

		////去除子块中的流行三角形
		vec_PHEDGE OneRedge;
		for (int i = 0; i < OneBlock.m_vecPHEdge.size(); i++)
		{
			PHEDGE PH0 = OneBlock.m_vecPHEdge[i];
			bool ifind = true;
			if (!PH0->pHEdgePair )
			{
				PHEDGE temp;
				if (PH0->pHEdgePre->pHEdgePair != NULL)
				{
					PH0 = PH0->pHEdgePre->pHEdgePair;
					temp = PH0;
					while (1)
					{
						if (temp->pHEdgePre->pHEdgePair != NULL)
						{
							temp = temp->pHEdgePre->pHEdgePair;
							if (temp == PH0)
							{
								break;
							}
						}
						else
						{
							ifind = false; break;
						}

					}
				}
				else
				{
					ifind = false;
				}
				if (ifind)
				{
					OneRedge.push_back(OneBlock.m_vecPHEdge[i]);//找到流行三角形的半边
				}
			}
		}
		//从面表中去除流行面片
		for (int i = 0; i < OneRedge.size(); i++)
		{
			//查找面表,删除对应面片
			for (int j = 0; j < OneBlock.m_vecPFacetTri.size(); j++)
			{
				PFACETTRI prFac = PFACETTRI(OneRedge[i]->pFacetAdj);
				if (prFac == OneBlock.m_vecPFacetTri[j])
				{
					OneBlock.m_vecPFacetTri.erase(OneBlock.m_vecPFacetTri.begin() + j);
					break;
				}
			}
		}
		//重新创建拓扑结构
		if (OneRedge.size()!=0)
		{
			StoreTheBlock(OneBlock.m_vecPFacetTri, BlockTemp);
			KitTool::ClearTheList(OneBlock);
			OneBlock.m_vecPFacetTri = BlockTemp.m_vecPFacetTri;
			OneBlock.m_vecPVert = BlockTemp.m_vecPVert;
			BlockTemp.m_vecPFacetTri.clear(); BlockTemp.m_vecPVert.clear();

			kit->CreateEdgeTopByFctVert(OneBlock);
			vec_Pface.clear();  vec_Pface = OneBlock.m_vecPFacetTri;
		}
	}
}
void fenceng::StoreTheBlock(vec_PFACETTRI m_vecPFact, Blockpath  &OneBlock)
{
	map<VERT, int>map_Vert; int VetxID = 0;
	map<VERT, int>::iterator iter;
	PVERT p0;
	for (int i = 0; i < m_vecPFact.size(); i++)//存入之前的面片
	{
		PFACETTRI PF0; PF0 = new FACETTRI;
		for (int j = 0; j < 3; j++)
		{
			p0 = new VERT;
			p0->x = m_vecPFact[i]->m_PVerts[j]->x;
			p0->y = m_vecPFact[i]->m_PVerts[j]->y;
			p0->z = m_vecPFact[i]->m_PVerts[j]->z;
			iter = map_Vert.find(*p0);
			if (iter != map_Vert.end())//已存在该点
			{
				int Num = iter->second;
				//OneBlock.m_vecPVert[Num]->vec_OneFac.push_back(PF0);
				PF0->m_PVerts[j] = OneBlock.m_vecPVert[Num];
			}
			else
			{
				p0->VertexID = VetxID;
				//p0->vec_OneFac.push_back(PF0);
				map_Vert.insert(pair<VERT, int>(*p0, OneBlock.m_vecPVert.size()));
				OneBlock.m_vecPVert.push_back(p0);
				PF0->m_PVerts[j] = p0;
				VetxID++;
			}
		}
		PF0->m_PFacetNorm = kit->CalcuPfacNormal(PF0->m_PVerts[0], PF0->m_PVerts[1], PF0->m_PVerts[2]);///求面片的法矢
		OneBlock.m_vecPFacetTri.push_back(PF0);
	}
}
void fenceng::HoleRepair(vector<vector<POINT3D>> &vec_Face, VECTOR3D FaceDir, PPOLYGON ploy)
{
	POINT3D Midpoint;
	int nct = ploy->m_vecPnts.size();
	for (int i = 0; i < nct;i++)
	{
		Midpoint.x += ploy->m_vecPnts[i].x;
		Midpoint.y += ploy->m_vecPnts[i].y;
		Midpoint.z += ploy->m_vecPnts[i].z;
	}
	Midpoint.x /= nct; Midpoint.y /= nct; Midpoint.z /= nct;

	vector<POINT3D> Face0;
	bool RightDir;
	for (int i = 0; i < nct;i++)
	{
		int nrt = (i + 1) % nct; Face0.clear();
		/////判别面片方向
		RightDir = KitTool::JudgeFaceDir(ploy->m_vecPnts[i], ploy->m_vecPnts[nrt], Midpoint, FaceDir);
		if (RightDir)
		{
			Face0.push_back(ploy->m_vecPnts[i]);
			Face0.push_back(ploy->m_vecPnts[nrt]);
		}
		else
		{
			Face0.push_back(ploy->m_vecPnts[nrt]);
			Face0.push_back(ploy->m_vecPnts[i]);
		}
		Face0.push_back(Midpoint);
		vec_Face.push_back(Face0);
	}

	//bool IfBound;
	//int ntime = 0; double s1, s2,st;
	//vector<vector<POINT3D>> vec_TempFace, vec_Face1;
	//do //迭代细分三角形
	//{
	//	vec_TempFace.clear();
	//	for (int i = 0; i < vec_Face.size();i++)
	//	{
	//		vec_Face1.clear(); s2 = 1000;
	//		s1 = kit->AreaTri(vec_Face[i][0], vec_Face[i][1], vec_Face[i][2]);
	//		IfBound = IfBoundFace(vec_Face[i], ploy->m_vecPnts);
	//		LoopOneFace(IfBound, FaceDir, vec_Face[i], vec_Face1);
	//		for (int j = 0; j < vec_Face1.size();j++)
	//		{
	//			st = kit->AreaTri(vec_Face1[j][0], vec_Face1[j][1], vec_Face1[j][2]);
	//			if (st<s2)
	//			{
	//				s2 = st;
	//			}
	//			vec_TempFace.push_back(vec_Face1[j]);
	//		}
	//		if (s2 / s1 < 0.4&&ntime>0)//满足面积比条件退出
	//		{
	//			if (i != vec_Face.size()-1)//存储未细分面片
	//			{
	//				for (int j = i; j<vec_Face.size(); j++)
	//				{
	//					vec_TempFace.push_back(vec_Face[j]);
	//				}
	//			}
	//			break;
	//		}
	//	}
	//	vec_Face.clear();
	//	for (int i = 0; i < vec_TempFace.size(); i++)
	//	{
	//		vec_Face.push_back(vec_TempFace[i]);
	//	}
	//	ntime++;
	//} while (ntime<2);
}
void fenceng::LoopOnehole(VECTOR3D vec_Dir, PPOLYGON ploy, vector<vector<POINT3D>> &vec_Face)
{

	PPOLYPOLYGON vecP; vecP = new POLYPOLYGON;
	Coffset::OffsetCircle(2, vec_Dir, ploy, vecP);//偏置环
	//Coffset::OffsetCircle1(2, vec_Dir, ploy, vecP);//偏置环

	POINT3D Midpoint;//环的中点
	int nct = ploy->m_vecPnts.size();
	for (int i = 0; i < nct; i++)
	{
		Midpoint.x += ploy->m_vecPnts[i].x;
		Midpoint.y += ploy->m_vecPnts[i].y;
		Midpoint.z += ploy->m_vecPnts[i].z;
	}
	Midpoint.x /= nct; Midpoint.y /= nct; Midpoint.z /= nct;
	PPOLYGON ployEnd = new POLYGON;
	ployEnd->m_vecPnts.push_back(Midpoint);
	vecP->m_vecPPolygons.push_back(ployEnd);

	POINT3D  p0, p1, p2, p3, p4, p5, pa, pb, pc;
	int nn = 0;
	for (int i = 0; i < vecP->m_vecPPolygons.size() - 1; i++)
	{
		vector<POINT3D> nFace;
		PPOLYGON ploy0, ploy1;
		ploy0 = vecP->m_vecPPolygons[i];
		for (int j = 0; j < ploy0->m_vecPnts.size(); j++)
		{
			if (j == ploy0->m_vecPnts.size() - 1)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p0 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 1];
				p1 = ploy0->m_vecPnts[0];

				KitTool::FindCircleNearPoint(p0, p3, ploy1);
				KitTool::FindCircleNearPoint(p1, p2, ploy1);
				KitTool::FindCircleNearPoint1(p0, nn, ploy1);

				//第一个面片 
				nFace.push_back(p3);  nFace.push_back(p0);  nFace.push_back(p2);
				vec_Face.push_back(nFace);    nFace.clear();

				//第二个面片 
				nFace.push_back(p2);  nFace.push_back(p0);  nFace.push_back(p1);
				vec_Face.push_back(nFace);    nFace.clear();
			}

			if (j == vecP->m_vecPPolygons[i]->m_vecPnts.size() - 2)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p0 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 2];
				p1 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 1];

				KitTool::FindCircleNearPoint(p0, p3, ploy1);
				KitTool::FindCircleNearPoint(p1, p2, ploy1);
				KitTool::FindCircleNearPoint1(p0, nn, ploy1);

				//第一个面片 
				nFace.push_back(p3);  nFace.push_back(p0);  nFace.push_back(p2);
				vec_Face.push_back(nFace);    nFace.clear();

				//第二个面片 
				nFace.push_back(p2);  nFace.push_back(p0);  nFace.push_back(p1);
				vec_Face.push_back(nFace);    nFace.clear();


			}
			if (j == 0)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p0 = ploy0->m_vecPnts[0];
				p1 = ploy0->m_vecPnts[1];

				KitTool::FindCircleNearPoint(p0, p3, ploy1);
				KitTool::FindCircleNearPoint(p1, p2, ploy1);
				KitTool::FindCircleNearPoint1(p0, nn, ploy1);

				//第一个面片 
				nFace.push_back(p3);  nFace.push_back(p0);  nFace.push_back(p2);
				vec_Face.push_back(nFace);    nFace.clear();

				//第二个面片 
				nFace.push_back(p2);  nFace.push_back(p0);  nFace.push_back(p1);
				vec_Face.push_back(nFace);    nFace.clear();
			}
			else   if (0 < j&&j < ploy0->m_vecPnts.size() - 2)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p0 = ploy0->m_vecPnts[j - 1];

				p1 = ploy0->m_vecPnts[j];

				p2 = ploy0->m_vecPnts[j + 1];

				pb = ploy0->m_vecPnts[j + 2];

				int nn1, nn2, nn3, nn4;
				KitTool::FindCircleNearPoint(p0, p5, ploy1);
				KitTool::FindCircleNearPoint1(p1, nn1, ploy1);

				KitTool::FindCircleNearPoint(p1, p4, ploy1);
				KitTool::FindCircleNearPoint1(p1, nn2, ploy1);

				KitTool::FindCircleNearPoint(p2, p3, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn3, ploy1);

				KitTool::FindCircleNearPoint(pb, pa, ploy1);
				KitTool::FindCircleNearPoint1(pb, nn4, ploy1);


				if (nn3 - nn2 == 3)//
				{
					pc = vecP->m_vecPPolygons[i + 1]->m_vecPnts[nn3 - 1];
					p0= vecP->m_vecPPolygons[i + 1]->m_vecPnts[nn2 + 1];

					//第一个面片 
					nFace.push_back(pc);  nFace.push_back(p1);  nFace.push_back(p2);
					vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(pc);  nFace.push_back(p0);  nFace.push_back(p1);
					vec_Face.push_back(nFace);    nFace.clear();

					//第三个面片 
					nFace.push_back(p0);  nFace.push_back(p1);  nFace.push_back(p4);
					vec_Face.push_back(nFace);    nFace.clear();

					//第四个面片 
					nFace.push_back(pc);  nFace.push_back(p3);  nFace.push_back(p2);
					vec_Face.push_back(nFace);    nFace.clear();
				}
				if (nn3 - nn2 == 2)//gaiguo
				{
					p0 = ploy1->m_vecPnts[nn3 - 1];

					//这种情况也要写出来；
					//第一个面片 
					nFace.push_back(p0);  nFace.push_back(p2);  nFace.push_back(p3);
					vec_Face.push_back(nFace);    nFace.clear();

				}
				if (nn3 - nn2 == 4)
				{
					//也存在这种情况，也要写出来；
					pa = ploy1->m_vecPnts[nn3 - 1];
					pb = ploy1->m_vecPnts[nn3 - 2];
					pc = ploy1->m_vecPnts[nn3 - 3];

					//第一个面片 
					nFace.push_back(p2);  nFace.push_back(pc);  nFace.push_back(pb);
					vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(p2);  nFace.push_back(pb);  nFace.push_back(pa);
					vec_Face.push_back(nFace);    nFace.clear();

					//第三个面片 
					nFace.push_back(p3);  nFace.push_back(pa);  nFace.push_back(p2);
					vec_Face.push_back(nFace);    nFace.clear();

				}
				if ((p5 != p4) && (p4 == p3) && (p3 != pa))
				{
					//第一个面片 
					nFace.push_back(p4);  nFace.push_back(p1);  nFace.push_back(p2);
					vec_Face.push_back(nFace);    nFace.clear();
				}
				else if ((nn1 == nn2) && (nn2 == nn3))
				{
					//第一个面片 
					/*nFace.push_back(p4);  nFace.push_back(p0);  nFace.push_back(p1);//去掉，不用
					vec_Face.push_back(nFace);    nFace.clear();*/

					//第二个面片 
					nFace.push_back(p1);  nFace.push_back(p2);  nFace.push_back(p4);
					vec_Face.push_back(nFace);    nFace.clear();

				}
				else
				{
					pb = ploy1->m_vecPnts[nn2 + 1];

					//第一个面片 
					nFace.push_back(p4);  nFace.push_back(p1);  nFace.push_back(pb);
					vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(pb);  nFace.push_back(p1);  nFace.push_back(p2);
					vec_Face.push_back(nFace);    nFace.clear();
				}
			}

		}
	}
	delete vecP;
}
void fenceng::LoopOnehole1(VECTOR3D vec_Dir, PPOLYGON ploy, vector<vector<POINT3D>> &vec_Face, POINT3D &Pceshi)
{
	PPOLYPOLYGON vecP; vecP = new POLYPOLYGON;
	//Coffset::OffsetCircle(2, vec_Dir, ploy, vecP);//偏置环
	vec_Face.clear();
	vec_PPOLYPOLYGON m_vec;
	vec_PPOLYPOLYGON m_vecFillP;

	PPOLYPOLYGON vre = new POLYPOLYGON;
	vre->m_vecPPolygons.push_back(ploy);
	m_vec.push_back(vre);
	Coffset::GetOffset1(m_vec, vec_Dir, m_vecFillP,2 );//偏置环
	vecP = m_vecFillP[0];

	POINT3D Midpoint;//环的中点
	int nrt = vecP->m_vecPPolygons.size()-1;
	int nct = vecP->m_vecPPolygons[nrt]->m_vecPnts.size();
	for (int i = 0; i < nct; i++)
	{
		Midpoint.x += vecP->m_vecPPolygons[nrt]->m_vecPnts[i].x;
		Midpoint.y += vecP->m_vecPPolygons[nrt]->m_vecPnts[i].y;
		Midpoint.z += vecP->m_vecPPolygons[nrt]->m_vecPnts[i].z;
	}
	Midpoint.x /= nct; Midpoint.y /= nct; Midpoint.z /= nct;
	PPOLYGON ployEnd = new POLYGON;
	ployEnd->m_vecPnts.push_back(Midpoint);
	vecP->m_vecPPolygons.push_back(ployEnd);
	POINT3D Prt0;//剔除可疑点
	if (vecP->m_vecPPolygons[0]->m_vecPnts.size() > 62)
	{
		Pceshi = vecP->m_vecPPolygons[0]->m_vecPnts[62];
	}
	
	POINT3D  p0, p1, p2, p3, p4, p5, p6, p7, p8, p9, pa, pb, pc;
	int nn = 0; bool RightDir;

	int rt0=0, rt1=0, rt2=0, rt3=0, rt4=0, rt5=0, rt6=0, rt7=0, rt8=0, rt9=0;
	for (int i = 0; i < vecP->m_vecPPolygons.size() - 1; i++)
	{
		vector<POINT3D> nFace;
		PPOLYGON ploy0, ploy1;
		ploy0 = vecP->m_vecPPolygons[i];
		for (int n = 0; n < ploy0->m_vecPnts.size(); n++)
		{
			if (n == ploy0->m_vecPnts.size() - 1)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p2 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 1];
				p3 = ploy0->m_vecPnts[0];

				KitTool::FindCircleNearPoint(p2, p5, ploy1);
				KitTool::FindCircleNearPoint(p3, p4, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn, ploy1);
				rt0++;//测试程序
				//第一个面片 
				nFace.push_back(p5);  nFace.push_back(p2); 
				  nFace.push_back(p4);   vec_Face.push_back(nFace);   nFace.clear();

				//第二个面片 
				  bool ceshi1 = false;
				  vec_POINT3D nFace1;
				  nFace1.push_back(p4);  nFace1.push_back(p2);
				  nFace1.push_back(p3);
				  /*for (int m = 0; m < 3; m++)
				  {
				  if (nFace1[m].x == Prt0.x&&nFace1[m].y == Prt0.y&&nFace1[m].z == Prt0.z)
				  {
				  for (int t = 0; t < 3; t++)
				  {
				  if ((t != m) && (nFace1[t].x == Prt0.x&&nFace1[t].y == Prt0.y&&nFace1[t].z == Prt0.z))
				  {
				  ceshi1 = true;
				  break;
				  }
				  }
				  }
				  if (ceshi1) break;
				  }
				  if (!ceshi1)
				  {
				  nFace.push_back(p4);  nFace.push_back(p2);
				  nFace.push_back(p3); vec_Face.push_back(nFace);
				  }*/
				  nFace.push_back(p4);  nFace.push_back(p2);
				  nFace.push_back(p3); vec_Face.push_back(nFace);
				  nFace.clear();
			}

			if (n == vecP->m_vecPPolygons[i]->m_vecPnts.size() - 2)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p2 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 2];
				p3 = ploy0->m_vecPnts[ploy0->m_vecPnts.size() - 1];

				KitTool::FindCircleNearPoint(p2, p5, ploy1);
				KitTool::FindCircleNearPoint(p3, p4, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn, ploy1);
				rt1++;//测试程序
				//第一个面片 
				bool Iffind = true;
				if ((p5.x == p4.x&&p5.y == p4.y&&p5.z == p4.z))
				{
					Iffind = false;
				}
				if (Iffind)
				{
					nFace.push_back(p5);  nFace.push_back(p2);
					nFace.push_back(p4); vec_Face.push_back(nFace);
				}
				nFace.clear();

				//第二个面片 
				nFace.push_back(p4);  nFace.push_back(p2);  
				nFace.push_back(p3); vec_Face.push_back(nFace);    nFace.clear();


			}
			if (n == 0)//没毛病
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p2 = ploy0->m_vecPnts[0];
				p3 = ploy0->m_vecPnts[1];

				KitTool::FindCircleNearPoint(p2, p5, ploy1);
				KitTool::FindCircleNearPoint(p3, p4, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn, ploy1);
				rt2++;//测试程序
				//第一个面片 
				nFace.push_back(p5);  nFace.push_back(p2);
				nFace.push_back(p4); vec_Face.push_back(nFace);    nFace.clear();

				//第二个面片 
				nFace.push_back(p4);  nFace.push_back(p2);  
				nFace.push_back(p3); vec_Face.push_back(nFace);    nFace.clear();
			}
			else   if (0 < n&&n < ploy0->m_vecPnts.size() - 2)
			{
				ploy1 = vecP->m_vecPPolygons[i + 1];
				p1 = ploy0->m_vecPnts[n - 1];

				p2 = ploy0->m_vecPnts[n];

				p3 = ploy0->m_vecPnts[n + 1];

				pb = ploy0->m_vecPnts[n + 2];

				int nn1, nn2, nn3, nn4;
				KitTool::FindCircleNearPoint(p1, p6, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn1, ploy1);

				KitTool::FindCircleNearPoint(p2, p5, ploy1);
				KitTool::FindCircleNearPoint1(p2, nn2, ploy1);

				KitTool::FindCircleNearPoint(p3, p4, ploy1);
				KitTool::FindCircleNearPoint1(p3, nn3, ploy1);

				KitTool::FindCircleNearPoint(pb, p7, ploy1);
				KitTool::FindCircleNearPoint1(pb, nn4, ploy1);


				if (nn3 - nn2 == 3)//没毛病
				{
					p9 = vecP->m_vecPPolygons[i + 1]->m_vecPnts[nn3 - 1];
					p0 = vecP->m_vecPPolygons[i + 1]->m_vecPnts[nn2 + 1];
					rt3++;//测试程序
					nFace.push_back(p9);  nFace.push_back(p2);
					nFace.push_back(p3); vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(p9);  nFace.push_back(p0);  //p0和p2反了一下导致面片反向了
					nFace.push_back(p2); vec_Face.push_back(nFace);    nFace.clear();

					//第三个面片 
					nFace.push_back(p0);  nFace.push_back(p2);
					nFace.push_back(p5); vec_Face.push_back(nFace);    nFace.clear();//不影响,有没有该面片无所谓

					//第四个面片 
					
					nFace.push_back(p9);  nFace.push_back(p4);
					nFace.push_back(p3); vec_Face.push_back(nFace);    nFace.clear();
				}
				if (nn3 - nn2 == 2)//没毛病
				{
					p0 = ploy1->m_vecPnts[nn3 - 1];
					//这种情况也要写出来；
					//第一个面片 
					rt4++;//测试程序
					nFace.push_back(p0);  nFace.push_back(p3);
					nFace.push_back(p4);  vec_Face.push_back(nFace);    nFace.clear();

				}
				if (nn3 - nn2 == 4)//不涉及这种情况
				{
					//也存在这种情况，也要写出来；
					pa = ploy1->m_vecPnts[nn3 - 1];
					pb = ploy1->m_vecPnts[nn3 - 2];
					pc = ploy1->m_vecPnts[nn3 - 3];
					rt5++;//测试程序
					//第一个面片 
					nFace.push_back(p3);  nFace.push_back(pc);
					nFace.push_back(pb); vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(p3);  nFace.push_back(pb);
					nFace.push_back(pa);  vec_Face.push_back(nFace);    nFace.clear();

					//第三个面片 
					nFace.push_back(p4);  nFace.push_back(pa); 
					nFace.push_back(p3);  vec_Face.push_back(nFace);    nFace.clear();

				}
				if ((p6 != p5) && (p5 == p4) && (p4 != p7))//没毛病
				{
					//第一个面片 
					rt6++;//测试程序
					nFace.push_back(p5);  nFace.push_back(p2);
					nFace.push_back(p3);  vec_Face.push_back(nFace);    nFace.clear();
				}
				else if ((nn1 == nn2) && (nn2 == nn3))//没毛病，有点影响
				{
					//第一个面片 
					rt7++;//测试程序
					/*nFace.push_back(p5);  nFace.push_back(p1);//去掉，不需要
					nFace.push_back(p2);  vec_Face.push_back(nFace);    nFace.clear();*/

					//第二个面片 
					nFace.push_back(p2);  nFace.push_back(p3);
					nFace.push_back(p5); vec_Face.push_back(nFace);    nFace.clear();

				}
				else//没毛病
				{
					p8 = ploy1->m_vecPnts[nn2 + 1];

					//第一个面片 
					rt8++;//测试程序
					nFace.push_back(p5);  nFace.push_back(p2);
					nFace.push_back(p8);  vec_Face.push_back(nFace);    nFace.clear();

					//第二个面片 
					nFace.push_back(p8);  nFace.push_back(p2);
					nFace.push_back(p3); vec_Face.push_back(nFace);    nFace.clear();
				}

			}
		}
	}
}
void fenceng::WipeErrorLayer(vec_PPOLYPOLYGON &lunkuo)
{
	if (lunkuo.size() < 1) return;
	bool IfMis = false;
	for (int i = lunkuo.size()-1; i > -1; i--)
	{
		IfMis = false;
		PPOLYPOLYGON PloyPloy = lunkuo[i];
		if (PloyPloy->m_vecPPolygons.size()>15 || PloyPloy->m_vecPPolygons.size()<1)//一层大于15个环则视为错误
		{
			delete PloyPloy;
			lunkuo.erase(lunkuo.begin() + i);
		}
		
	}
}
bool fenceng::IfBoundFace(vector<POINT3D> Face, vector<POINT3D> vec_Point)
{
	int nct, nrt;
	POINT3D p0, p1,p2,p00,p01;
	for (int i = 0; i < 3;i++)
	{
		nct = (i + 1) % 3; nrt = (i + 2) % 3;
		p0 = Face[i]; p1 = Face[nct]; p2 = Face[nrt];
		for (int j = 0; j < vec_Point.size();j++)
		{
			p00 = vec_Point[j];
			if (p0.x == p00.x&&p0.y == p00.y&&p0.z == p00.z)
			{
				for (int k = 0; k < vec_Point.size(); k++)
				{
					p01 = vec_Point[k];
					if (p1.x == p01.x&&p1.y == p01.y&&p1.z == p01.z)
					{
						Face.clear();
						Face.push_back(p00); Face.push_back(p01); Face.push_back(p2); 
						return true;
					}

				}
				return false;
			}
		}
	}
	return false;
}
void fenceng::LoopOneFace(bool IfThree, VECTOR3D FaceDir, vector<POINT3D> Face, vector<vector<POINT3D>> &vec_Face)
{
	bool RightDir;
	vector<POINT3D> vec_point;
	if (IfThree)
	{
		for (int i = 1; i < Face.size(); i++)
		{
			int nct = (i + 1) % 3;
			POINT3D p0;
			p0.x = 0.5*(Face[i].x + Face[nct].x);
			p0.y = 0.5*(Face[i].y + Face[nct].y);
			p0.z = 0.5*(Face[i].z + Face[nct].z);
			vec_point.push_back(p0);
		}
		vector<POINT3D> OneFace; OneFace.clear();
		/////判别面片方向
		////存入面片0
		RightDir=KitTool::JudgeFaceDir(Face[0], Face[1], vec_point[0],FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[0]); OneFace.push_back(Face[1]);
		}
		else
		{
			OneFace.push_back(Face[1]); OneFace.push_back(Face[0]);
		}
		OneFace.push_back(vec_point[0]); vec_Face.push_back(OneFace); OneFace.clear();
		////存入面片1
		RightDir = KitTool::JudgeFaceDir(Face[0],vec_point[0], vec_point[1], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[0]); OneFace.push_back(vec_point[0]);
		}
		else
		{
			OneFace.push_back(vec_point[0]); OneFace.push_back(Face[0]);
		}
		OneFace.push_back(vec_point[1]); vec_Face.push_back(OneFace); OneFace.clear();
		////存入面片2
		RightDir = KitTool::JudgeFaceDir(Face[2], vec_point[1], vec_point[0], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[2]); OneFace.push_back(vec_point[1]);
		}
		else
		{
			OneFace.push_back(vec_point[1]); OneFace.push_back(Face[2]);
		}
		OneFace.push_back(vec_point[0]); vec_Face.push_back(OneFace); OneFace.clear();
		
	}
	else
	{
		for (int i = 0; i < Face.size(); i++)
		{
			int nct = (i + 1) % 3;
			POINT3D p0;
			p0.x = 0.5*(Face[i].x + Face[nct].x);
			p0.y = 0.5*(Face[i].y + Face[nct].y);
			p0.z = 0.5*(Face[i].z + Face[nct].z);
			vec_point.push_back(p0);
		}
		vector<POINT3D> OneFace; OneFace.clear();
		////存入面片0
		RightDir = KitTool::JudgeFaceDir(Face[0], vec_point[0], vec_point[2], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[0]); 
			OneFace.push_back(vec_point[0]);
		}
		else
		{
			OneFace.push_back(vec_point[0]);
			OneFace.push_back(Face[0]);
		}
		OneFace.push_back(vec_point[2]); 
		vec_Face.push_back(OneFace); OneFace.clear();
		////存入面片1
		RightDir = KitTool::JudgeFaceDir(Face[1], vec_point[1], vec_point[0], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[1]); 
			OneFace.push_back(vec_point[1]);
		}
		{
			OneFace.push_back(vec_point[1]); 
			OneFace.push_back(Face[1]);
		}
		OneFace.push_back(vec_point[0]); 
		vec_Face.push_back(OneFace); OneFace.clear();
		////存入面片2
		RightDir = KitTool::JudgeFaceDir(Face[2], vec_point[2], vec_point[1], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(Face[2]); 
			OneFace.push_back(vec_point[2]);
		}
		else
		{
			OneFace.push_back(vec_point[2]); 
			OneFace.push_back(Face[2]);
		}
		OneFace.push_back(vec_point[1]); 
		vec_Face.push_back(OneFace); OneFace.clear();
		
		////存入面片3
		RightDir = KitTool::JudgeFaceDir(vec_point[0], vec_point[1], vec_point[2], FaceDir);
		if (RightDir)
		{
			OneFace.push_back(vec_point[0]); 
			OneFace.push_back(vec_point[1]);
		}
		else
		{
			OneFace.push_back(vec_point[1]); 
			OneFace.push_back(vec_point[0]);
		}
		OneFace.push_back(vec_point[2]); 
		vec_Face.push_back(OneFace); OneFace.clear();
	}

}
void  fenceng::RotateBlockModel(Blockpath pMeshTri, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta;
	theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();

	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < pMeshTri.m_vecPVert.size(); i++)
	{
		//不修改地址对指针进行赋值
		POINT3D Temp;
		Temp.x = pMeshTri.m_vecPVert[i]->x;
		Temp.y = pMeshTri.m_vecPVert[i]->y;
		Temp.z = pMeshTri.m_vecPVert[i]->z;
		Temp = Temp*TM;
		pMeshTri.m_vecPVert[i]->x = Temp.x;
		pMeshTri.m_vecPVert[i]->y = Temp.y;
		pMeshTri.m_vecPVert[i]->z = Temp.z;
	}
}
void fenceng::CreateEdgeTopByFctVert(Blockpath &pMeshTri,vec_PFACETTRI m_vecpFact)
{
	int VetxID = 0;
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;

	for (int i = 0; i < m_vecpFact.size(); i++)//将三角面片放入容器中
	{
		PFACETTRI PFat; PFat = new FACETTRI; PVERT p0;
		for (int j = 0; j < 3; j++)
		{
			p0 = new VERT;
			p0->x = m_vecpFact[i]->m_PVerts[j]->x;
			p0->y = m_vecpFact[i]->m_PVerts[j]->y;
			p0->z = m_vecpFact[i]->m_PVerts[j]->z;
			iter = map_Vert.find(*p0);
			if (iter != map_Vert.end())//已存在该点
			{
				int Num = iter->second;
				pMeshTri.m_vecPVert[Num]->vec_OneFac.push_back(PFat);
				PFat->m_PVerts[j] = pMeshTri.m_vecPVert[Num];
			}
			else
			{
				p0->VertexID = VetxID;
				p0->vec_OneFac.push_back(PFat);
				map_Vert.insert(pair<VERT, int>(*p0, pMeshTri.m_vecPVert.size()));
				pMeshTri.m_vecPVert.push_back(p0);
				PFat->m_PVerts[j] = p0;
				VetxID++;
			}
		}
		PFat->m_PFacetNorm = kit->CalcuPfacNormal(PFat->m_PVerts[0], PFat->m_PVerts[1], PFat->m_PVerts[2]);///求面片的法矢
		pMeshTri.m_vecPFacetTri.push_back(PFat);
	}

	int	nFct, nFCnt;
	PFACETTRI pFct;

	int nCntTmp;

	set<PEDGE, CompPEDGE> setEdge;
	pair<set<PEDGE, CompPEDGE>::iterator, bool> pair_setEdge_it_b;

	nFct = pMeshTri.m_vecPFacetTri.size();
	for (nFCnt = 0; nFCnt < nFct; nFCnt++)
	{
		pFct = pMeshTri.m_vecPFacetTri[nFCnt];

		PHEDGE pHEdge[3];
		pHEdge[0] = new HEDGE; pHEdge[1] = new HEDGE; pHEdge[2] = new HEDGE;

		//pHEdge[0]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri.m_vecPHEdge.push_back(pHEdge[0]);//FOR Lin JunFeng
		//pHEdge[1]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri.m_vecPHEdge.push_back(pHEdge[1]);
		//pHEdge[2]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri.m_vecPHEdge.push_back(pHEdge[2]);//FOR Lin JunFeng

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
				pMeshTri.m_vecPEdge.push_back(pEdge);
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
void fenceng::CaleachBlockboundarypoint(Blockpath pMeshTri, vec_PPOLYPOLYGON &lunkuo, float dept)
{
	int n; float h; float Zt; int  nfacts = 0;
	BOX3D ONE;
	UpdateBlockBox(pMeshTri, ONE);
	h = ONE.z1 - ONE.z0;
	n = h / dept;
	PHEDGE PHE, PHedeg, PHedeg_0;
	
	for (int i = 1; i < n; i++)
	{
		PPOLYPOLYGON Polypoly = new POLYPOLYGON;
		Zt = ONE.z0 + i*dept;
		int nFct, nFcnt, t;
		PFACETTRI pFct;
		
		if (i==5)//60
		{
			int ceshi = 0;
		}
		nFcnt = pMeshTri.m_vecPFacetTri.size();
		bool IFfind = false;
		for (nFct = 0; nFct < nFcnt; nFct++)
		{
			PPOLYGON Poly = new POLYGON;
			
			pFct = pMeshTri.m_vecPFacetTri[nFct];
			POINT3D p0, p1; POINT3D face_point;
			if (!(pFct->bStatus) && Checkphedgegood(pFct, PHE, PHedeg_0, face_point, Zt))
			{
				///////////////////找出平面与模型的第一个交点///////////////////
				Poly->m_vecPnts.push_back(face_point);
				PHedeg = PHE; t = 0;
				//////////////找出模型与平面的第一个交点后，利用伙伴半边和拓扑关系求下一个相交面片/////////
				for (int j = 1;; j++)
				{
					p0 = POINT3D(PHE->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pVertEnd->z);
					p1 = POINT3D(PHE->pHEdgePre->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pHEdgePre->pVertEnd->z);
					if (kit->CalPlanePointIntPoint(p0, p1, Zt, face_point))
					{
						PHE->pFacetAdj->bStatus = TRUE;
						if (Poly->m_vecPnts[0]== face_point)//2017.11.08新加入
						{
							Polypoly->m_vecPPolygons.push_back(Poly);
							IFfind = true;
							break;
						}
						if (Poly->m_vecPnts.back() != face_point)
						{
							if (t == 0)
							{
								Poly->m_vecPnts.push_back(face_point);
							}
							else
								if (Poly->m_vecPnts.front() != face_point)
								{
								Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
								}
						}
						if (PHE->pHEdgePre->pHEdgePair == NULL)
						{
							t++;
							PHE = PHedeg_0;
						}
						else
							PHE = PHE->pHEdgePre->pHEdgePair;
					}
					else
					{
						p0 = POINT3D(PHE->pVertEnd->x, PHE->pVertEnd->y, PHE->pVertEnd->z);
						p1 = POINT3D(PHE->pHEdgeNext->pVertEnd->x, PHE->pHEdgeNext->pVertEnd->y, PHE->pHEdgeNext->pVertEnd->z);
						if (kit->CalPlanePointIntPoint(p0, p1, Zt, face_point))
						{
							//去除冗余的点
							PHE->pFacetAdj->bStatus = TRUE;
							if (Poly->m_vecPnts[0] == face_point)//2017.11.08新加入
							{
								Polypoly->m_vecPPolygons.push_back(Poly);
								IFfind = true;
								break;
							}
							if (Poly->m_vecPnts.back() != face_point)
							{
								if (t == 0)
								{
									Poly->m_vecPnts.push_back(face_point);
								}
								else
									if (Poly->m_vecPnts.front() != face_point)
									{
									Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
									}
							}
							if (PHE->pHEdgeNext->pHEdgePair == NULL)
							{
								t = t + 1;
								PHE = PHedeg_0;
								/*PHE = PHedeg->pHEdgePair;*/
							}
							else
								PHE = PHE->pHEdgeNext->pHEdgePair;
						}
					}
					if (PHedeg == PHE || t >= 2)
					{
						Polypoly->m_vecPPolygons.push_back(Poly);
						break;
					}
				}
			}
			//if (IFfind) break;//2017.11.08新加入
		}
		for (nFct = 0; nFct < nFcnt; nFct++)
		{
			pFct = pMeshTri.m_vecPFacetTri[nFct];
			if (pFct->bStatus)
			{
				pFct->bStatus = false;
			}
		}
		lunkuo.push_back(Polypoly);
		nfacts++;
	}
}
void fenceng::UpdateBlockBox(Blockpath pMeshTri, BOX3D &ONE)
{
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = pMeshTri.m_vecPFacetTri[1]->m_PVerts[0]->x;
	y0 = y1 = pMeshTri.m_vecPFacetTri[1]->m_PVerts[0]->y;
	z0 = z1 = pMeshTri.m_vecPFacetTri[1]->m_PVerts[0]->z;

	FACETTRI* ptri;
	for (int i = 0; i < pMeshTri.m_vecPFacetTri.size(); i++)
	{
		ptri = pMeshTri.m_vecPFacetTri[i];
		for (int n = 0; n < 3; n++)
		{
			if (ptri->m_PVerts[n]->x < x0)
				x0 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->x > x1)
				x1 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->y < y0)
				y0 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->y > y1)
				y1 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->z < z0)
				z0 = ptri->m_PVerts[n]->z;
			if (ptri->m_PVerts[n]->z > z1)
				z1 = ptri->m_PVerts[n]->z;
		}
	}
	ONE.x0 = x0;
	ONE.y0 = y0;
	ONE.z0 = z0;
	ONE.x1 = x1;
	ONE.y1 = y1;
	ONE.z1 = z1;
}
void  fenceng::UpdateBlockBox(MyBlock pMeshTri, BOX3D &ONE)
{
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = pMeshTri.OneBlock[1]->m_PVerts[0]->x;
	y0 = y1 = pMeshTri.OneBlock[1]->m_PVerts[0]->y;
	z0 = z1 = pMeshTri.OneBlock[1]->m_PVerts[0]->z;

	FACETTRI* ptri;
	for (int i = 0; i < pMeshTri.OneBlock.size(); i++)
	{
		ptri = pMeshTri.OneBlock[i];
		for (int n = 0; n < 3; n++)
		{
			if (ptri->m_PVerts[n]->x < x0)
				x0 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->x > x1)
				x1 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->y < y0)
				y0 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->y > y1)
				y1 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->z < z0)
				z0 = ptri->m_PVerts[n]->z;
			if (ptri->m_PVerts[n]->z > z1)
				z1 = ptri->m_PVerts[n]->z;
		}
	}
	ONE.x0 = x0;
	ONE.y0 = y0;
	ONE.z0 = z0;
	ONE.x1 = x1;
	ONE.y1 = y1;
	ONE.z1 = z1;
}
void fenceng::Calthecenterpoint(Blockpath OneBlock, VECTOR3D Current_Dir,POINT3D &p0D)
{
	//当前模型初始加工平面没有进行孔洞修补

	vec_PHEDGE BordRing;//存储所有的边界边，查找边界边的一阶邻域面片
	for (int i = 0; i < OneBlock.m_vecPEdge.size(); i++)
	{
		PHEDGE Ph = OneBlock.m_vecPEdge[i]->pHEdgeAdj;
		if (!Ph->pHEdgePair)
		{
			BordRing.push_back(Ph);
		}
	}
	if (BordRing.size() == 0) return;
	PPOLYPOLYGON vec_poly; vec_poly = new POLYPOLYGON;
	kit->closedBoundary(BordRing, vec_poly);//给所有的边界顶点排序
	BordRing.clear();

	VECTOR3D v0, v1,v2;
	for (int i = 0; i < vec_poly->m_vecPPolygons.size(); i++)
	{
		PPOLYGON OneCircle = vec_poly->m_vecPPolygons[i];
		int nsize = OneCircle->m_vecPnts.size();
		v0 = OneCircle->m_vecPnts[nsize / 3] - OneCircle->m_vecPnts[0];
		v1 = OneCircle->m_vecPnts[nsize * 2 / 3] - OneCircle->m_vecPnts[nsize / 3];
		v2 = v0*v1; v2.Normalize();

		double dir = v2 | Current_Dir;
		if (dir == 1 || dir == -1)//面片法矢方向与最优成型方向平行
		{
			//求取平面上的中心点
			for (int j = 0; j < OneCircle->m_vecPnts.size(); j++)
			{
				p0D.x += OneCircle->m_vecPnts[j].x;
				p0D.y += OneCircle->m_vecPnts[j].y;
				p0D.z += OneCircle->m_vecPnts[j].z;
			}
			p0D.x /= nsize;
			p0D.y /= nsize;
			p0D.z /= nsize;
			return;
		}
	}
	
}
void fenceng::CaltheFarpoint(Blockpath OneBlock, VECTOR3D Current_Dir,POINT3D p0D, POINT3D &p1D)
{
	double LDis = 0;
	int AtemNub = 0;
	for (int i = 0; i < OneBlock.m_vecPVert.size(); i++)
	{
		PVERT P0 = OneBlock.m_vecPVert[i];
		//计算当前顶点到平面的距离
		VECTOR3D  d1 = kit->PlanePointDistance(*P0, p0D, Current_Dir);
		if (LDis < d1.GetLength())
		{
			LDis = d1.GetLength();
			AtemNub = i;//标记当前点为最远点
		}
	}
	p1D = *OneBlock.m_vecPVert[AtemNub];

	POINT3D pt, p2D;
	pt.x = Current_Dir.dx + p0D.x;
	pt.y = Current_Dir.dy + p0D.y;
	pt.z = Current_Dir.dz + p0D.z;

	double t = 0;
	t = ((p1D.x - pt.x)*Current_Dir.dx + (p1D.y - pt.y)*Current_Dir.dy + (p1D.z - pt.z)*Current_Dir.dz) / (
		(p0D.x - pt.x)*Current_Dir.dx + (p0D.y - pt.y)*Current_Dir.dy + (p0D.z - pt.z)*Current_Dir.dz);
	p2D.x = t*(p0D.x - pt.x) + pt.x;
	p2D.y = t*(p0D.y - pt.y) + pt.y;
	p2D.z = t*(p0D.z - pt.z) + pt.z;

	p1D = p2D;
}
void fenceng::Caleachlayerboundarypoint(Blockpath OneBlock, vec_PPOLYPOLYGON &lunkuo, bool IfFir, VECTOR3D Curr_Dir, float dept, QProgressBar  *progressBar)
{
	float h;  
	if (IfFir)
	{
		BOX3D ONE;
		UpdateBlockBox(OneBlock, ONE);
		h = ONE.z1 - ONE.z0;
		float thickness_h = dept;
		while (ONE.z0 + thickness_h < ONE.z1)
		{
			PPOLYPOLYGON ppolypolygon;
			ppolypolygon = GetHighSlice(OneBlock, thickness_h, ONE.z0);
			ppolypolygon->UpdateBox();
			lunkuo.push_back(ppolypolygon);
			thickness_h += dept;
		}
	}
	else
	{
		POINT3D planeVer;
		POINT3D P0A, P0B;
		Calthecenterpoint(OneBlock, Curr_Dir,P0A);
		CaltheFarpoint(OneBlock, Curr_Dir,P0A, P0B);

		int n;
		int  nfacts = 0;
		VECTOR3D V1 = P0B - P0A;
		h = V1.GetLength();
		n = h / dept;
		PHEDGE PHE, PHedeg, PHedeg_0;

		progressBar->setRange(0, n - 1);
		for (int i = 1; i < n; i++)
		{
			progressBar->setValue(nfacts);
			PPOLYPOLYGON Polypoly = new POLYPOLYGON;
			int nFct, nFcnt, t;
			PFACETTRI pFct;

			double Scal = i*dept / h;
			planeVer.x = Scal*P0A.x + (1 - Scal)*P0B.x;
			planeVer.y = Scal*P0A.y + (1 - Scal)*P0B.y;
			planeVer.z = Scal*P0A.z + (1 - Scal)*P0B.z;
			nFcnt = OneBlock.m_vecPFacetTri.size();
			for (nFct = 0; nFct < nFcnt; nFct++)
			{
				PPOLYGON Poly = new POLYGON;

				pFct = OneBlock.m_vecPFacetTri[nFct];
				POINT3D p0, p1; POINT3D face_point;
				if (!(pFct->bStatus) && Checkphedgegood(pFct, PHE, PHedeg_0, planeVer, face_point, Curr_Dir))
				{
					///////////////////找出平面与模型的第一个交点///////////////////
					Poly->m_vecPnts.push_back(face_point);
					PHedeg = PHE; t = 0;
					//////////////找出模型与平面的第一个交点后，利用伙伴半边和拓扑关系求下一个相交面片/////////
					for (int j = 1;; j++)
					{
						p0 = POINT3D(PHE->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pVertEnd->z);
						p1 = POINT3D(PHE->pHEdgePre->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pHEdgePre->pVertEnd->z);
						if (kit->CalPlaneLineIntPoint(p0, p1, planeVer, Curr_Dir, face_point))
						{
							PHE->pFacetAdj->bStatus = TRUE;
							if (Poly->m_vecPnts.back() != face_point)
							{
								if (t == 0)
								{
									Poly->m_vecPnts.push_back(face_point);
								}
								else
									if (Poly->m_vecPnts.front() != face_point)
									{
									Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
									}
							}
							if (PHE->pHEdgePre->pHEdgePair == NULL)
							{
								t++;
								PHE = PHedeg_0;
							}
							else
								PHE = PHE->pHEdgePre->pHEdgePair;
						}
						else
						{
							p0 = POINT3D(PHE->pVertEnd->x, PHE->pVertEnd->y, PHE->pVertEnd->z);
							p1 = POINT3D(PHE->pHEdgeNext->pVertEnd->x, PHE->pHEdgeNext->pVertEnd->y, PHE->pHEdgeNext->pVertEnd->z);
							if (kit->CalPlaneLineIntPoint(p0, p1, planeVer, Curr_Dir, face_point))
							{
								//去除冗余的点
								PHE->pFacetAdj->bStatus = TRUE;
								if (Poly->m_vecPnts.back() != face_point)
								{
									if (t == 0)
									{
										Poly->m_vecPnts.push_back(face_point);
									}
									else
										if (Poly->m_vecPnts.front() != face_point)
										{
										Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
										}
								}
								if (PHE->pHEdgeNext->pHEdgePair == NULL)
								{
									t = t + 1;
									PHE = PHedeg_0;
									/*PHE = PHedeg->pHEdgePair;*/
								}
								else
									PHE = PHE->pHEdgeNext->pHEdgePair;
							}
						}
						if (PHedeg == PHE || t >= 2)
						{
							Polypoly->m_vecPPolygons.push_back(Poly);
							break;
						}
					}
				}
			}
			for (nFct = 0; nFct < nFcnt; nFct++)
			{
				pFct = OneBlock.m_vecPFacetTri[nFct];
				if (pFct->bStatus)
				{
					pFct->bStatus = false;
				}
			}
			lunkuo.push_back(Polypoly);
			nfacts++;
		}
	}
	
}
bool fenceng::IntersectLinePlane(POINT3D LinePt, VECTOR3D LineNor, PFACETTRI pFacTri, POINT3D &InterP)
{
	POINT3D p00,p10;
	p00 = LinePt;
	p10.x = LineNor.dx*1000 + p00.x;
	p10.y = LineNor.dy* 1000 + p00.y;
	p10.z = LineNor.dz* 1000 + p00.z;

	POINT3D plane_point;
	VECTOR3D dir;
	plane_point = *pFacTri->m_PVerts[0];
	dir = *pFacTri->m_PFacetNorm;
	if (p00.z > p10.z)
	{
		POINT3D p03 = p00;
		p00 = p10;
		p10 = p03;
	}
	VECTOR3D  d1, d2, u0, u1; 
	//求直线上顶点1的欧氏距离
	dir.Normalize();
	u0 = p00 - plane_point;
	double stt = u0 | dir;
	d1 = dir*stt;
	//求直线上顶点2的欧氏距离
	u0 = p10 - plane_point;
	stt = u0 | dir;
	d2 = dir*stt;

	//求直线与三角面片所在平面的交点
	double s, t;
	s = d1 | d2;
	if (s > 0)
	{
		return false;//无交点
	}
	if (fabs(s) == 0)
	{
		if ((d1.GetLength() == d2.GetLength() == 0) && (d1.dx == d2.dx == 0) && (d1.dy == d2.dy == 0) && (d1.dz == d2.dz == 0))
		{
			return false;//无交点
		}
		else
		{
			if ((d1.dx == 0) && (d1.dy == 0) && (d1.dz == 0)){ InterP = p00; }
			else { InterP = p10; }
			return false;//无交点
		}
	}
	else
	{
		t = d2.GetLength() / (d2.GetLength() + d1.GetLength());
		u1 = p10 - p00;
		InterP = p10 - u1*t;
	}
	
	//判断所求得的交点是否在三角面片内
	double Area;
	VECTOR3D vEdge1, vEdge2, vArea;
	//求总的面积
	vEdge1 = VECTOR3D(pFacTri->m_PVerts[0]->x - pFacTri->m_PVerts[1]->x, 
		pFacTri->m_PVerts[0]->y - pFacTri->m_PVerts[1]->y, pFacTri->m_PVerts[0]->z - pFacTri->m_PVerts[1]->z);
	vEdge2 = VECTOR3D(pFacTri->m_PVerts[2]->x - pFacTri->m_PVerts[1]->x,
		pFacTri->m_PVerts[2]->y - pFacTri->m_PVerts[1]->y, pFacTri->m_PVerts[2]->z - pFacTri->m_PVerts[1]->z);
	vArea = vEdge1*vEdge2;
	Area = 0.5*vArea.GetLength();

	vector<double> vec_Area;
	for (int i = 0; i < 3; i++)
	{
		double area1;
		int nct = (i + 1) % 3;
		vEdge1 = VECTOR3D(pFacTri->m_PVerts[i]->x - InterP.x,
			pFacTri->m_PVerts[i]->y - InterP.y, pFacTri->m_PVerts[i]->z - InterP.z);
		vEdge2 = VECTOR3D(pFacTri->m_PVerts[nct]->x - InterP.x,
			pFacTri->m_PVerts[nct]->y - InterP.y, pFacTri->m_PVerts[nct]->z - InterP.z);
		vArea = vEdge1*vEdge2;
		area1 = 0.5*vArea.GetLength();
		vec_Area.push_back(area1);
	}
	if (Area ==vec_Area[0] + vec_Area[1] + vec_Area[2])
	{
		return true;
	}
	else
	{
		return false;
	}
}
bool fenceng::Checkphedgegood(PFACETTRI Pfact, PHEDGE &PHE, PHEDGE &PHE_0, POINT3D planeVer, POINT3D &face_point, VECTOR3D Curr_Dir)
{
	int nCnt, t = 0;
	POINT3D p0; POINT3D p1;
	PHEDGE PHEdge[3];
	PHEdge[0] = Pfact->pHEdge;
	PHEdge[1] = Pfact->pHEdge->pHEdgePre;
	PHEdge[2] = Pfact->pHEdge->pHEdgeNext;
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		p0 = POINT3D(PHEdge[nCnt]->pVertEnd->x, PHEdge[nCnt]->pVertEnd->y, PHEdge[nCnt]->pVertEnd->z);
		p1 = POINT3D(PHEdge[nCnt]->pHEdgePre->pVertEnd->x, PHEdge[nCnt]->pHEdgePre->pVertEnd->y, PHEdge[nCnt]->pHEdgePre->pVertEnd->z);
		if (kit->CalPlaneLineIntPoint(p0, p1, planeVer, Curr_Dir, face_point))
		{
			t = t + 1;
			PHEdge[nCnt]->pFacetAdj->bStatus = TRUE;
			if (t == 1)
			{
				PHE_0 = PHEdge[nCnt];
			}
			else
			{
				PHE = PHEdge[nCnt];
				return true;
			}
		}
	}
	if (t == 0)
	{
		return false;
	}
	return true;
}
void fenceng::Caleachlayerboundarypoint(STLModell* pMeshTri, vec_PPOLYPOLYGON &lunkuo, float dept, QProgressBar  *progressBar)
{
	int n; float h; float Zt; int  nfacts=0;
	pMeshTri->GetBox();
	h = pMeshTri->box.z1 - pMeshTri->box.z0;
	n = h / dept;
	PHEDGE PHE, PHedeg, PHedeg_0;

	progressBar->setRange(0, n-1);
	for (int i = 1; i <n; i++)
	{
		progressBar->setValue(nfacts);
		PPOLYPOLYGON Polypoly = new POLYPOLYGON;
		Zt = pMeshTri->box.z0 + i*dept;
		int nFct, nFcnt, t;
		PFACETTRI pFct;

		nFcnt = pMeshTri->m_vecPFacetTri.size();
		for (nFct = 0; nFct < nFcnt; nFct++)
		{
			PPOLYGON Poly = new POLYGON;

			pFct = pMeshTri->m_vecPFacetTri[nFct];
			POINT3D p0, p1; POINT3D face_point;
			if (!(pFct->bStatus) && Checkphedgegood(pFct, PHE, PHedeg_0, face_point, Zt))
			{
				///////////////////找出平面与模型的第一个交点///////////////////
				Poly->m_vecPnts.push_back(face_point);
				PHedeg = PHE; t = 0;
				//////////////找出模型与平面的第一个交点后，利用伙伴半边和拓扑关系求下一个相交面片/////////
				for (int j = 1;; j++)
				{
					p0 = POINT3D(PHE->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pVertEnd->z);
					p1 = POINT3D(PHE->pHEdgePre->pHEdgePre->pVertEnd->x, PHE->pHEdgePre->pHEdgePre->pVertEnd->y, PHE->pHEdgePre->pHEdgePre->pVertEnd->z);
					if (kit->CalPlanePointIntPoint(p0, p1, Zt, face_point))
					{
						PHE->pFacetAdj->bStatus = TRUE;
						if (Poly->m_vecPnts.back() != face_point)
						{
							if (t == 0)
							{
								Poly->m_vecPnts.push_back(face_point);
							}
							else
								if (Poly->m_vecPnts.front() != face_point)
								{
								Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
								}
						}
						if (PHE->pHEdgePre->pHEdgePair == NULL)
						{
							t++;
							PHE = PHedeg_0;
						}
						else
							PHE = PHE->pHEdgePre->pHEdgePair;
					}
					else
					{
						p0 = POINT3D(PHE->pVertEnd->x, PHE->pVertEnd->y, PHE->pVertEnd->z);
						p1 = POINT3D(PHE->pHEdgeNext->pVertEnd->x, PHE->pHEdgeNext->pVertEnd->y, PHE->pHEdgeNext->pVertEnd->z);
						if (kit->CalPlanePointIntPoint(p0, p1, Zt, face_point))
						{
							//去除冗余的点
							PHE->pFacetAdj->bStatus = TRUE;
							if (Poly->m_vecPnts.back() != face_point)
							{
								if (t == 0)
								{
									Poly->m_vecPnts.push_back(face_point);
								}
								else
									if (Poly->m_vecPnts.front() != face_point)
									{
									Poly->m_vecPnts.insert(Poly->m_vecPnts.begin(), face_point);
									}
							}
							if (PHE->pHEdgeNext->pHEdgePair == NULL)
							{
								t = t + 1;
								PHE = PHedeg_0;
								/*PHE = PHedeg->pHEdgePair;*/
							}
							else
								PHE = PHE->pHEdgeNext->pHEdgePair;
						}
					}
					if (PHedeg == PHE || t >= 2)
					{
						Polypoly->m_vecPPolygons.push_back(Poly);
						break;
					}
				}
			}
		}
		for (nFct = 0; nFct < nFcnt; nFct++)
		{
			pFct = pMeshTri->m_vecPFacetTri[nFct];
			if (pFct->bStatus)
			{
				pFct->bStatus = false;
			}
		}
		lunkuo.push_back(Polypoly);
		nfacts++;
	}
}
PPOLYPOLYGON fenceng::GetHighSlice(STLModell *pSTL, float thickness_h)
{
	POINT3D p3D1, p3D2;
	int m = pSTL->m_vecPHEdge.size();
	double startpoint = pSTL->ONE.z0;
	PPOLYPOLYGON ppolypolygon = new POLYPOLYGON;
	int classfiy = 0;
	PPOLYGON  ppolygon;
	for (int i = 0; i < m; i++)
	{
		
		PHEDGE HEdge = pSTL->m_vecPHEdge[i]; 
		if (HEdge->pVertEnd->z < HEdge->pHEdgePre->pVertEnd->z)//控制分层线外逆里顺
		{
			if (CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D1) &&
				(HEdge->bStatus == FALSE))
			{

				if (HEdge->pHEdgePre->pVertEnd->z != startpoint + thickness_h)
				{
					POINT3D p3D;
					POINT3D p3DEX;
					ppolygon = new POLYGON;
					p3D = p3D1;
					HEdge->bStatus = TRUE;
					ppolygon->m_vecPnts.push_back(p3D);
					p3DEX = p3D;
					do
					{
						HEdge->bStatus = TRUE;
						HEdge = HEdge->pHEdgeNext;
						if (CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D2))
						{
                            //2017.11.08修改
							HEdge->bStatus = TRUE;
							if (HEdge->pHEdgePair == NULL)  break;
							else
							{
								HEdge = HEdge->pHEdgePair;
							}
							//修改结束
							//HEdge = HEdge->pHEdgePair;
						}
						else
						{
							//2017.11.08修改
							HEdge = HEdge->pHEdgeNext;
							CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D2);
							HEdge->bStatus = TRUE;
							if (HEdge->pHEdgePair == NULL)  break;
							else
							{
								HEdge = HEdge->pHEdgePair;
							}
							//修改结束
							//HEdge = HEdge->pHEdgePair;
						}
						if (p3DEX != p3D2)//去除冗余，针对刚好过顶点的情况。
						{

							int sz = ppolygon->m_vecPnts.size();
							if (sz != 0)
							{
								if (ppolygon->m_vecPnts.back() != p3D2)
								{
									if (sz > 1)
									{
										//////////去除一条直线上的多个点///////////
										VECTOR3D v1 = ppolygon->m_vecPnts[sz - 1] - ppolygon->m_vecPnts[sz - 2];
										VECTOR3D v2 = p3D2 - ppolygon->m_vecPnts[sz - 1];
										v1.Normalize();
										v2.Normalize();
										float angle = v1 | v2;
										if (abs(1 - angle) < CAD_ZERO)
										{
											ppolygon->m_vecPnts.pop_back();
										}
									}
									ppolygon->m_vecPnts.push_back(p3D2);
								}
							}
							else
							{
								ppolygon->m_vecPnts.push_back(p3D2);
							}
							p3DEX = p3D2;
						}
						
					} while (HEdge->bStatus == FALSE);
					//////为了得到有效的轮廓环
					if (ppolygon->m_vecPnts.size() > 2)
					{
						ppolygon->m_vecPnts.pop_back();
						POINT3D p3 = ppolygon->m_vecPnts[1];
						VECTOR3D v1 = p3 - ppolygon->m_vecPnts[0];
						VECTOR3D v2 = ppolygon->m_vecPnts[0] - ppolygon->m_vecPnts.back();
						v1.Normalize();
						v2.Normalize();
						float Ang = abs(v1 | v2);
						if ((1 - Ang) < CAD_ZERO)
						{
							ppolygon->m_vecPnts.erase(ppolygon->m_vecPnts.begin());
						}
						ppolygon->m_vecPnts.push_back(ppolygon->m_vecPnts[0]);
						ppolygon->m_bIsClosed = true;
						ppolygon->UpdateBox();
						classfiy = classfiy++;
						ppolygon->m_Iclassify = classfiy;
						ppolypolygon->m_vecPPolygons.push_back(ppolygon);
					}
					else delete ppolygon;
				}
			}
		}

	}
	for (int i = 0; i < m; i++)
	{

		pSTL->m_vecPHEdge[i]->bStatus = FALSE;

	}
	return ppolypolygon;
}
PPOLYPOLYGON fenceng::GetHighSlice(Blockpath pMeshTri, float thickness_h, double startpoint)
{
	POINT3D p3D1, p3D2;
	int m = pMeshTri.m_vecPHEdge.size();

	PPOLYPOLYGON ppolypolygon = new POLYPOLYGON;
	int classfiy = 0;
	PPOLYGON  ppolygon;
	for (int i = 0; i < m; i++)
	{
		PHEDGE HEdge = pMeshTri.m_vecPHEdge[i];
		if (HEdge->pVertEnd->z < HEdge->pHEdgePre->pVertEnd->z)//控制分层线外逆里顺
		{
			if (CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D1) &&
				(HEdge->bStatus == FALSE))
			{

				if (HEdge->pHEdgePre->pVertEnd->z != startpoint + thickness_h)
				{
					POINT3D p3D;
					POINT3D p3DEX;
					ppolygon = new POLYGON;
					p3D = p3D1;
					HEdge->bStatus = TRUE;
					ppolygon->m_vecPnts.push_back(p3D);
					p3DEX = p3D;
					do
					{
						HEdge->bStatus = TRUE;
						HEdge = HEdge->pHEdgeNext;
						if (CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D2))
						{
							HEdge->bStatus = TRUE;
							if (HEdge->pHEdgePair == NULL)  break;
							else
							{
								HEdge = HEdge->pHEdgePair;
							}
							//HEdge = HEdge->pHEdgePair;
						}
						else
						{
							HEdge = HEdge->pHEdgeNext;
							CrossZ(HEdge->pVertEnd, HEdge->pHEdgePre->pVertEnd, startpoint + thickness_h, p3D2);
							HEdge->bStatus = TRUE;
							if (HEdge->pHEdgePair == NULL)  break;
							else
							{
								HEdge = HEdge->pHEdgePair;
							}
							//HEdge = HEdge->pHEdgePair;
						}
						if (p3DEX != p3D2)//去除冗余，针对刚好过顶点的情况。
						{

							int sz = ppolygon->m_vecPnts.size();
							if (sz != 0)
							{
								if (ppolygon->m_vecPnts.back() != p3D2)
								{
									if (sz > 1)
									{
										//////////去除一条直线上的多个点///////////
										VECTOR3D v1 = ppolygon->m_vecPnts[sz - 1] - ppolygon->m_vecPnts[sz - 2];
										VECTOR3D v2 = p3D2 - ppolygon->m_vecPnts[sz - 1];
										v1.Normalize();
										v2.Normalize();
										float angle = v1 | v2;
										if (abs(1 - angle) < CAD_ZERO)
										{
											ppolygon->m_vecPnts.pop_back();
										}
									}
									ppolygon->m_vecPnts.push_back(p3D2);
								}
							}
							else
							{
								ppolygon->m_vecPnts.push_back(p3D2);
							}
							p3DEX = p3D2;
						}
					} while (HEdge->bStatus == FALSE );
					//////为了得到有效的轮廓环
					if (ppolygon->m_vecPnts.size() > 2)
					{
						ppolygon->m_vecPnts.pop_back();
						POINT3D p3 = ppolygon->m_vecPnts[1];
						VECTOR3D v1 = p3 - ppolygon->m_vecPnts[0];
						VECTOR3D v2 = ppolygon->m_vecPnts[0] - ppolygon->m_vecPnts.back();
						v1.Normalize();
						v2.Normalize();
						float Ang = abs(v1 | v2);
						if ((1 - Ang) < CAD_ZERO)
						{
							ppolygon->m_vecPnts.erase(ppolygon->m_vecPnts.begin());
						}
						ppolygon->m_vecPnts.push_back(ppolygon->m_vecPnts[0]);
						ppolygon->m_bIsClosed = true;
						ppolygon->UpdateBox();
						classfiy = classfiy++;
						ppolygon->m_Iclassify = classfiy;
						ppolypolygon->m_vecPPolygons.push_back(ppolygon);
					}
					else delete ppolygon;
				}
			}
		}

	}
	for (int i = 0; i < m; i++)
	{

		pMeshTri.m_vecPHEdge[i]->bStatus = FALSE;

	}
	return ppolypolygon;
}
bool fenceng::CrossZ(PVERT p1, PVERT p2, double z, POINT3D &p3D)

{
	if (p1->z == p2->z)return FALSE;//处理半边刚好在分层面上。
	double min, max;
	min = p1->z;
	max = p1->z;

	if (p2->z < min)  min = p2->z;
	if (p2->z > max)  max = p2->z;

	double k;
	k = (z - p1->z) / (p2->z - p1->z);
	if ((z - min) >= -CAD_ZERO && (max - z) >= CAD_ZERO)
		/*if(min<=z && max>=z)*/
	{
		p3D.x = k*(p2->x - p1->x) + p1->x;
		p3D.y = k*(p2->y - p1->y) + p1->y;
		p3D.z = z;
		return TRUE;
	}
	else
		return FALSE;

}
void fenceng::Draw(drawTool *pDC, vec_PPOLYPOLYGON Plunkuo)
{
	if (Plunkuo.size() < 1) return;
	for (int i = 0; i< Plunkuo.size(); i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = Plunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetColor(RGB(255, 0, 0));
			pDC->SetMaterialColor(RGB(255, 0, 0));
			
			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				if (k == ploy->m_vecPnts.size() - 1)
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[0]);
				}
				else
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[k + 1]);
				}

				//pDC->DrawPoint(ploy->m_vecPnts[k]);//绘制空间点  
			}
			pDC->SetMaterialColor(old);
			
		}
	}
}
void  fenceng::Draw(drawTool *pDC, vec_PPOLYPOLYGON Plunkuo, int nLayer)
{
	if (Plunkuo.size() < 1) return;
	for (int i = 0; i < nLayer; i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = Plunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetColor(RGB(255, 0, 0));
			pDC->SetMaterialColor(RGB(255, 0, 0));

			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				if (k == ploy->m_vecPnts.size() - 1)
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[0]);
				}
				else
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[k + 1]);
				}

				//pDC->DrawPoint(ploy->m_vecPnts[k]);//绘制空间点  
			}
			pDC->SetMaterialColor(old);

		}
	}
}
void  fenceng::DrawOneLayerFill(drawTool *pDC, PPOLYPOLYGON Polypoly)
{
	if (Polypoly->m_vecPPolygons.size() < 1) return;
	for (int i = 0; i < Polypoly->m_vecPPolygons.size(); i++)
	{
		PPOLYGON ploy;
		ploy = Polypoly->m_vecPPolygons[i];
		COLORREF old;
		pDC->GetColor(old);
		pDC->GetMaterialColor(old);
		pDC->SetColor(RGB(0, 0, 0));
		pDC->SetMaterialColor(RGB(0, 0, 0));

		for (int j = 0; j < ploy->m_vecPnts.size(); j++)
		{
			if (j == ploy->m_vecPnts.size() - 1)
			{
				pDC->DrawLine(ploy->m_vecPnts[j], ploy->m_vecPnts[0]);
			}
			else
			{
				pDC->DrawLine(ploy->m_vecPnts[j], ploy->m_vecPnts[j + 1]);
			}

			//pDC->DrawPoint(ploy->m_vecPnts[j]);//绘制空间点  
		}
		pDC->SetMaterialColor(old);

	}
}
void  fenceng::DrawThatLayerFill(drawTool *pDC, int CurrentLayer, vec_PPOLYPOLYGON Plunkuo)
{
	if (Plunkuo.size() < 1) return;
	for (int i = 0; i < CurrentLayer; i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = Plunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetColor(RGB(0, 0, 0));
			pDC->SetMaterialColor(RGB(0, 0, 0));

			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				if (k == ploy->m_vecPnts.size() - 1)
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[0]);
				}
				else
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[k + 1]);
				}

				//pDC->DrawPoint(ploy->m_vecPnts[k]);//绘制空间点  
			}
			pDC->SetMaterialColor(old);

		}
	}
}
void fenceng::DrawFill(drawTool *pDC, vec_PPOLYPOLYGON Plunkuo)
{
	if (Plunkuo.size() < 1) return;
	for (int i = 0; i < Plunkuo.size(); i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = Plunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetColor(RGB(0, 0, 0));
			pDC->SetMaterialColor(RGB(0, 0, 0));

			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				if (k == ploy->m_vecPnts.size() - 1)
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[0]);
				}
				else
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[k + 1]);
				}

				//pDC->DrawPoint(ploy->m_vecPnts[k]);//绘制空间点  
			}
			pDC->SetMaterialColor(old);

		}
	}
}
void  fenceng::DrawFill(drawTool *pDC, COLORREF newcolor, vec_PPOLYPOLYGON Plunkuo)
{
	if (Plunkuo.size() < 1) return;
	for (int i = 0; i < Plunkuo.size(); i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = Plunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			COLORREF old;
			pDC->GetColor(old);
			pDC->GetMaterialColor(old);
			pDC->SetColor(newcolor);
			pDC->SetMaterialColor(newcolor);

			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				if (k == ploy->m_vecPnts.size() - 1)
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[0]);
				}
				else
				{
					pDC->DrawLine(ploy->m_vecPnts[k], ploy->m_vecPnts[k + 1]);
				}

				//pDC->DrawPoint(ploy->m_vecPnts[k]);//绘制空间点  
			}
			pDC->SetMaterialColor(old);

		}
	}
}
void fenceng::RotateModel(STLModell* STL, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta;
	theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < STL->m_vecPVert.size(); i++)
	{
		//不修改地址对指针进行赋值
		POINT3D Temp;
		Temp.x = STL->m_vecPVert[i]->x;
		Temp.y = STL->m_vecPVert[i]->y;
		Temp.z = STL->m_vecPVert[i]->z;
		Temp = Temp*TM;
		STL->m_vecPVert[i]->x = Temp.x;
		STL->m_vecPVert[i]->y = Temp.y;
		STL->m_vecPVert[i]->z = Temp.z;
	}
}

void fenceng::RotatesliceModel(vector<PPOLYPOLYGON> &lunkuo, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < lunkuo.size(); i++)
	{
		PPOLYPOLYGON Polypoly;
		Polypoly = lunkuo[i];
		for (int j = 0; j < Polypoly->m_vecPPolygons.size(); j++)
		{
			PPOLYGON ploy;
			ploy = Polypoly->m_vecPPolygons[j];
			for (int k = 0; k < ploy->m_vecPnts.size(); k++)
			{
				POINT3D Temp;
				Temp.x = ploy->m_vecPnts[k].x;
				Temp.y = ploy->m_vecPnts[k].y;
				Temp.z = ploy->m_vecPnts[k].z;
				Temp = Temp*TM;
				ploy->m_vecPnts[k].x = Temp.x;
				ploy->m_vecPnts[k].y = Temp.y;
				ploy->m_vecPnts[k].z = Temp.z;
			}
		}
	}
}

//判断某一个面片会不会被平面所截函数
bool fenceng::Checkphedgegood(PFACETTRI Pfact, PHEDGE &PHE, PHEDGE &PHE_0, POINT3D &face_point, float Zt)
{
	int nCnt, t = 0;
	POINT3D p0; POINT3D p1;
	PHEDGE PHEdge[3];
	PHEdge[0] = Pfact->pHEdge;
	PHEdge[1] = Pfact->pHEdge->pHEdgePre;
	PHEdge[2] = Pfact->pHEdge->pHEdgeNext;
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		p0 = POINT3D(PHEdge[nCnt]->pVertEnd->x, PHEdge[nCnt]->pVertEnd->y, PHEdge[nCnt]->pVertEnd->z);
		p1 = POINT3D(PHEdge[nCnt]->pHEdgePre->pVertEnd->x, PHEdge[nCnt]->pHEdgePre->pVertEnd->y, PHEdge[nCnt]->pHEdgePre->pVertEnd->z);
		if (kit->CalPlanePointIntPoint(p0, p1, Zt, face_point))
		{
			t = t + 1;
			PHEdge[nCnt]->pFacetAdj->bStatus = TRUE;
			if (t == 1)
			{
				PHE_0 = PHEdge[nCnt];
			}
			else
			{
				PHE = PHEdge[nCnt];
				return true;
			}
		}
	}
	if (t==0)
	{  
		return false;
	}
	return true;
}
//清空三维容器，以备后续使用
void fenceng::clear(vec_PPOLYPOLYGON &lunkuo)
{
	for (int i = 0; i < lunkuo.size(); i++)
	{
		for (int j = 0; j < lunkuo[i]->m_vecPPolygons.size(); j++)
		{
			delete lunkuo[i]->m_vecPPolygons[j];
		}
		lunkuo[i]->m_vecPPolygons.clear();
	}
	lunkuo.clear();
}





