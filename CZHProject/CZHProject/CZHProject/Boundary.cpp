
#include "Boundary.h"
///////////////////////////////////////////可视锥连接
void Cboundary::Skeletonizing_link(vector<PVERT>& BLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge,double &Ang)
{
	PVERT EN,ST,temp;
	map<PVERT,PVERT> PathA,pathB;
	/*vector<pair<PVERT,double>> vec_cone;
	vector<pair<PVERT,double>>::iterator it_vec_cone,tempK;*/
	map<double,PVERT> vec_cone;
	vector<double> vec_dis;
	VECTOR3D LineNor1,LineNor2,LineNor,LineNorX,LineNorZ;
		vector<VECTOR3D> m_vecVECTOR3D;
		vec_PFACETTRI vecpFac;
		POINT3D Center;
		PVERT link,Pst,Pen;
		link=NULL;
		double geodis=1000;
		int num=(int)(BLOOP.size()/2);
		////////////////////////////////////////////////
		//Center=(POINT3D)*(BLOOP[num]);
		/*int numJS=0;
		for (int numi=0;numi<BLOOP.size();numi++)
		{
			if (BLOOP[numi]->bused==1)
			{
				Center.x=Center.x+BLOOP[numi]->x;Center.y=Center.y+BLOOP[numi]->y;Center.z=Center.z+BLOOP[numi]->z;
				numJS++;
			}
		}
		Center.x=Center.x/numJS;Center.y=Center.y/numJS;Center.z=Center.z/numJS;*/
		Center.x=(BLOOP[0]->x+BLOOP.back()->x)*0.5;Center.y=(BLOOP[0]->y+BLOOP.back()->y)*0.5;
		Center.z=(BLOOP[0]->z+BLOOP.back()->z)*0.5;
		///////////////////////////////////////
		LineNor1.dx=BLOOP.back()->x-BLOOP[num]->x;LineNor1.dy=BLOOP.back()->y-BLOOP[num]->y;
		LineNor1.dz=BLOOP.back()->z-BLOOP[num]->z;
		LineNor2.dx=BLOOP[0]->x-BLOOP[num]->x;LineNor2.dy=BLOOP[0]->y-BLOOP[num]->y;
		LineNor2.dz=BLOOP[0]->z-BLOOP[num]->z;
		LineNor1.Normalize();
		LineNor2.Normalize();
		LineNor=LineNor1+LineNor2;
		LineNor.Normalize();
		LineNorX=LineNor1*LineNor2;
		LineNorX.Normalize();
		LineNorZ=LineNor*LineNorX;
		LineNorZ.Normalize();
		/////////////////////////////////////////
		BLOOP[num]->theEND=1;
		double tempANG=(LineNor1|LineNor2);
		/*if (tempANG>(-0.5))
		{
			m_vecVECTOR3D.push_back(LineNor);
		}
		else*/
		//{
	  LineNor=BLOOP[num]->VertexNormal;
		LineNor.dx=-LineNor.dx;LineNor.dy=-LineNor.dy;LineNor.dz=-LineNor.dz;
		Kit.create_cone_new(m_vecVECTOR3D,LineNorZ,LineNor,Center,-(Ang*0.5),Ang/5);
		//}
		Kit.FindOneRFac(BLOOP[num], vecpFac);
		Kit.FindTwoRing(BLOOP[num], vecpFac);
		for (int numb=0;numb<vecpFac.size();numb++)
		{
			vecpFac[numb]->becut=1;
		}
		for (int numb=0;numb<m_vecPFacetTri.size();numb++)
		{
			POINT3D PVERT1,PVERT2,PVERT3;
			PVERT1=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[0]);
			PVERT2=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[1]);
			PVERT3=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[2]);
			for (int numC=0;numC<m_vecVECTOR3D.size();numC++)
			{
				if (Kit.IntersectTriangle(Center,m_vecVECTOR3D[numC],PVERT1,PVERT2,PVERT3)==TRUE)
				{
					double tempdis=0;
					VECTOR3D N1=(*m_vecPFacetTri[numb]->m_PFacetNorm);
					if (m_vecPFacetTri[numb]->becut==0&&(N1|LineNor)>=0)
					{
					tempdis=Kit.dis((POINT3D)(*m_vecPFacetTri[numb]->m_PVerts[0]),Center);
					tempdis=tempdis*exp(1-Kit.CalcuPOINTV(m_vecPFacetTri[numb]->m_PVerts[0]));
						geodis=tempdis;
						link=m_vecPFacetTri[numb]->m_PVerts[0];
						/////////////////////论文效果图
						/*
						link->bused=1;
						PHEDGE tempH,tempHp;
						PVERT temPP=new VERT;
						temPP->x=Center.x;temPP->y=Center.y;temPP->z=Center.z;
						tempH=new HEDGE;tempHp=new HEDGE;
						tempH->pHEdgePair=tempHp;
						tempHp->pHEdgePair=tempH;
						tempH->pVertEnd=temPP;
						tempHp->pVertEnd=link;
					    tempHp->bused=1;tempH->bused=1;
						m_vecPHEdge.push_back(tempH);m_vecPHEdge.push_back(tempHp);*/
						/////////////
						vec_cone.insert(pair<double,PVERT>(geodis,m_vecPFacetTri[numb]->m_PVerts[0]));
						vec_dis.push_back(geodis);
					}
				}
			}
		}
		Kit.cone_delel(vec_dis);
		///////////////////////////////////////
		sort(vec_dis.begin(),vec_dis.end());
		BOOL isLINK=0;
		if (!vec_dis.empty())
		{
			link=vec_cone[vec_dis[0]];
			link->theEND=1;
			link->bused=1;
			isLINK=1;
			///////////////论文效果图
			/*PHEDGE tempH,tempHp;
			PVERT temPP=new VERT;
			temPP->x=Center.x;temPP->y=Center.y;temPP->z=Center.z;
			tempH=new HEDGE;tempHp=new HEDGE;
			tempH->pHEdgePair=tempHp;
			tempHp->pHEdgePair=tempH;
			tempH->pVertEnd=temPP;
			tempHp->pVertEnd=link;
			tempHp->bused=1;tempH->bused=1;
			m_vecPHEdge.push_back(tempH);m_vecPHEdge.push_back(tempHp);*/
			///////////////////////////
		}
		////////////////////////////////////////
		for (int numb=0;numb<vecpFac.size();numb++)
		{
			vecpFac[numb]->becut=0;
		}
		//	/////////////////形成闭合圈
		if (isLINK==1);
		{
			vec_PVERT m_Pst,m_Pen;
			Pst=BLOOP[0];Pen=BLOOP.back();
			Kit.Dijkstra_Point_ST_EN_new(link,Pst,pathB,BLOOP,m_vecPVert);
			temp=Pst;
			while (temp!=link)
			{
				m_Pst.push_back(temp);
				temp=pathB[temp];
			}
			m_Pst.push_back(link);
			m_Pst.erase(m_Pst.begin());
			pathB.clear();
			Kit.Dijkstra_Point_ST_EN_new(link, Pen, pathB, BLOOP, m_vecPVert);
			temp=Pen;
			while (temp!=link)
			{
				m_Pen.push_back(temp);
				temp=pathB[temp];
			}
			m_Pen.push_back(link);
			m_Pen.erase(m_Pen.begin());
			if (m_Pst.size()>m_Pen.size())
			{
				for (int numK=0;numK<m_Pen.size();numK++)
				{
					m_Pen[numK]->bused=1;
					BLOOP.push_back(m_Pen[numK]);
				}
			}
			if (m_Pst.size()<=m_Pen.size())
			{
				for (int numK=0;numK<m_Pst.size();numK++)
				{
					m_Pst[numK]->bused=1;
					BLOOP.insert(BLOOP.begin(),m_Pst[numK]);
				}
			}
			link_cone(BLOOP,20,18,m_vecPFacetTri,m_vecPVert,m_vecPHEdge);
		}
}
void Cboundary::Skeletonizing_link_new(vector<PVERT>& BLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert)
{
	PVERT temp;
	map<PVERT,PVERT> pathB;
	vec_PVERT m_Pst;
	Kit.Dijkstra_Point_ST_EN_new(BLOOP[0], BLOOP.back(), pathB, BLOOP, m_vecPVert);
	temp=BLOOP.back();
	while (temp!=BLOOP[0])
	{
		m_Pst.push_back(temp);
		temp=pathB[temp];
	}
	m_Pst.push_back(BLOOP[0]);
	m_Pst.erase(m_Pst.begin());
	for (int i=0;i<m_Pst.size();i++)
	{
		BLOOP.push_back(m_Pst[i]);
	}
	pathB.clear();
}
void Cboundary::link_cone(vector<PVERT>& BLOOP,double angA,double angB,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge)
{
	PVERT temp;
	map<PVERT,PVERT> PathA,pathB;
	/*vector<pair<PVERT,double>> vec_cone;
	vector<pair<PVERT,double>>::iterator it_vec_cone,tempK;*/
	map<double,PVERT> vec_cone;
	vector<double> vec_dis;
	vector<double>::iterator ite;
	VECTOR3D LineNor1,LineNor2,LineNor,LineNorX,LineNorZ;
		vector<VECTOR3D> m_vecVECTOR3D;
		vec_PFACETTRI vecpFac;
		POINT3D Center;
		PVERT link,Pst,Pen;
		link=NULL;
		double geodis=1000;
		int num=(int)(BLOOP.size()/2);
		Center.x=(BLOOP[0]->x+BLOOP.back()->x)*0.5;Center.y=(BLOOP[0]->y+BLOOP.back()->y)*0.5;
		Center.z=(BLOOP[0]->z+BLOOP.back()->z)*0.5;
		/////////////////////////////////////
		LineNor1.dx=BLOOP.back()->x-BLOOP[num]->x;LineNor1.dy=BLOOP.back()->y-BLOOP[num]->y;
		LineNor1.dz=BLOOP.back()->z-BLOOP[num]->z;
		LineNor2.dx=BLOOP[0]->x-BLOOP[num]->x;LineNor2.dy=BLOOP[0]->y-BLOOP[num]->y;
		LineNor2.dz=BLOOP[0]->z-BLOOP[num]->z;
		LineNor1.Normalize();
		LineNor2.Normalize();
		LineNor=LineNor1+LineNor2;
		LineNor.Normalize();
		LineNorX=LineNor1*LineNor2;
		LineNorX.Normalize();
		LineNorZ=LineNor*LineNorX;
		LineNorZ.Normalize();
		/////////////////////////////////////////
		BLOOP[num]->theEND=1;
		double tempANG=(LineNor1|LineNor2);
		///////////////////////////////////////////////////////
		/*LineNor=Ccalcubase::CalcuVerNormal(BLOOP[num]);
		LineNor.dx=-LineNor.dx;LineNor.dy=-LineNor.dy;LineNor.dz=-LineNor.dz;*/
		Kit.create_cone_new(m_vecVECTOR3D, LineNorZ, LineNor, Center, -20, 10);
		Kit.FindOneRFac(BLOOP[num], vecpFac);
		Kit.FindTwoRing(BLOOP[num], vecpFac);
		for (int numb=0;numb<vecpFac.size();numb++)
		{
			vecpFac[numb]->becut=1;
		}
		for (int numb=0;numb<m_vecPFacetTri.size();numb++)
		{
			POINT3D PVERT1, PVERT2, PVERT3;
			PVERT1=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[0]);
			PVERT2=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[1]);
			PVERT3=(POINT3D)*(m_vecPFacetTri[numb]->m_PVerts[2]);
			for (int numC=0;numC<m_vecVECTOR3D.size();numC++)
			{
				if (Kit.IntersectTriangle(Center, m_vecVECTOR3D[numC], PVERT1, PVERT2, PVERT3) == TRUE)
				{
					double tempdis=0;
					tempdis = Kit.dis((POINT3D)(*m_vecPFacetTri[numb]->m_PVerts[0]), Center);
					if (m_vecPFacetTri[numb]->becut==0)
					{
						geodis=tempdis;
						link=m_vecPFacetTri[numb]->m_PVerts[0];
						vec_cone.insert(pair<double,PVERT>(geodis,m_vecPFacetTri[numb]->m_PVerts[0]));
						vec_dis.push_back(geodis);
						////////////////////////////////
						//link->bused=1;
					}
				}
			}
		}
		Kit.cone_delel(vec_dis);
		sort(vec_dis.begin(),vec_dis.end());
		BOOL isLINK=0;
		if (!vec_dis.empty())
		{
			link=vec_cone[vec_dis[0]];
			link->theEND=1;
			link->bused=1;
			isLINK=1;
			///////////////
			PHEDGE tempH,tempHp;
			PVERT temPP=new VERT;
			temPP->x=Center.x;temPP->y=Center.y;temPP->z=Center.z;
			tempH=new HEDGE;tempHp=new HEDGE;
			tempH->pHEdgePair=tempHp;
			tempHp->pHEdgePair=tempH;
			tempH->pVertEnd=temPP;
			tempHp->pVertEnd=link;
			tempHp->bused=1;tempH->bused=1;
			m_vecPHEdge.push_back(tempH);m_vecPHEdge.push_back(tempHp);
			///////////////////////////
		}
		for (int numb=0;numb<vecpFac.size();numb++)
		{
			vecpFac[numb]->becut=0;
		}

			/////////////////形成闭合圈
		if (isLINK==1)
		{
		vec_PVERT m_Pst,m_Pen;
		vec_PHEDGE vecpH;
		Pst=BLOOP[0];Pen=BLOOP.back();
		Kit.Dijkstra_Point_ST_EN_new(link, Pst, pathB, BLOOP, m_vecPVert);
		temp=Pst;
		while (temp!=link)
		{
			m_Pst.push_back(temp);
			temp=pathB[temp];
		}
		pathB.clear();
		Kit.Dijkstra_Point_ST_EN_new(link, Pen, pathB, BLOOP, m_vecPVert);
		temp=Pen;
		while (temp!=link)
		{
			m_Pen.push_back(temp);
			temp=pathB[temp];
		}
		m_Pen.push_back(link);
			for (int numK=0;numK<m_Pen.size();numK++)
			{
				m_Pen[numK]->bused=1;
				BLOOP.push_back(m_Pen[numK]);
			}
			for (int numK=0;numK<m_Pst.size();numK++)
			{
				m_Pst[numK]->bused=1;
				BLOOP.insert(BLOOP.begin(),m_Pst[numK]);
			}
		}
}
 ////////////////////////////通过种子生长法划分区域
void Cboundary::Skeletonizing_partlize(vector<MyBlock> &Rbox, vector<vector<PVERT>>&path, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert)
{
	//复制所有分割线上顶点
	vec_PVERT Markverts;
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		if (m_vecPVert[i]->bused)
		{
			Markverts.push_back(m_vecPVert[i]);
		}
	}
	//更新路径点
	for (int i = 0; i < path.size(); i++)
	{
		for (int j = 0; j < path[i].size(); j++)
		{
			for (int k = 0; k < Markverts.size();k++)
			{
				if (*path[i][j]== *Markverts[k])
				{
					path[i][j] = Markverts[k];
					Markverts.erase(Markverts.begin() + k);
					break;
				}
			}
		}
	}
	Markverts.clear();
	for (int i=0;i<m_vecPFacetTri.size();i++)
	{
		m_vecPFacetTri[i]->bStatus=0;
	}
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vec_PFACETTRI tempPath,vecFac;
	MyBlock BlockTemp;
	int numID=0;
	while (1)
	{
		int num=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				num++;
				break;
			}
		}
		if (num==0)
		{
			break;
		}
		while(!Q.empty())
		{
			Kit.FindOneRing(Q.front(), vecpVer);
			Kit.FindOneRFac(Q.front(), vecFac);

			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bused==0)
				{
					vecpVer[i]->bused=1;
					Q.push(vecpVer[i]);
				}
			}
			for (int i=0;i<vecFac.size();i++)
			{
				if (vecFac[i]->bStatus==0)
				{
					vecFac[i]->bStatus=1;
					tempPath.push_back(vecFac[i]);
				}
			}
			Q.front()->bused=1;
			Q.pop();
		}
		for (int i=0;i<tempPath.size();i++)
		{
			tempPath[i]->ID=numID;
		}
		BlockTemp.OneBlock.clear();
		BlockTemp.OneBlock=tempPath;
		Rbox.push_back(BlockTemp);
		numID++;
		tempPath.clear();
	}
	///////////////未包含区域并入RBOX
	vector<PFACETTRI> outbox;
	for (int i=0;i<m_vecPFacetTri.size();i++)
	{
		if (m_vecPFacetTri[i]->ID==-1)
		{
			outbox.push_back(m_vecPFacetTri[i]);
		}
	}
	while(1)
	{
		int numK=0;
		for (int i=0;i<outbox.size();i++)
		{
			if (outbox[i]->ID<0)
			{
				vector<PFACETTRI> vecpFacP;
				Kit.FindPOneRFAC_NEW(outbox[i], vecpFacP);
				if (vecpFacP.size()==0)
				{
					numK++;
				}
				for (int j=0;j<vecpFacP.size();j++)
				{
					if (vecpFacP[j]->ID>=0)
					{
						outbox[i]->ID=vecpFacP[j]->ID;
						Rbox[vecpFacP[j]->ID].OneBlock.push_back(outbox[i]);
					}
				}
			}
			else
			{
				numK++;
			}
		}
		if (numK==outbox.size())
		{
			break;
		}
	}
	for (int i = 0; i < Rbox.size(); i++)
	{
		for (int j = 0; j < Rbox[i].OneBlock.size(); j++)
		{
			Rbox[i].OneBlock[j]->ID = i;
		}
	}
	
	//建立块与块间的拓扑关系
	for (int i = 0; i<path.size(); i++)
	{
		vecFac.clear(); vecFac = path[i][0]->vec_OneFac;
		//Kit.FindOneRFac(path[i][0], vecFac);
		map<int, int>map_FaceID;
		map<int, int>::iterator iter;
		vector<int> vec_Int;
		for (int j = 0; j < vecFac.size(); j++)
		{
			iter = map_FaceID.find(vecFac[j]->ID);
			if (iter == map_FaceID.end())
			{
				map_FaceID.insert(pair<int, int>(vecFac[j]->ID, vec_Int.size()));
				vec_Int.push_back(vecFac[j]->ID);
			}
		}
		int nct = 0;
		for (int j = nct + 1; j < vec_Int.size(); j++)
		{
			int n0 = vec_Int[nct]; int n1 = vec_Int[j];
			Rbox[vec_Int[nct]].OneBlockPair.push_back(Rbox[vec_Int[j]]);
			Rbox[vec_Int[j]].OneBlockPair.push_back(Rbox[vec_Int[nct]]);
		}
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
	}
	//建立块与分割线的对应关系
	vector<MyBlock> RboxTemp; int NubK = 1;
	for (int i = 0; i < Rbox.size(); i++)//查找最大主干
	{
		if (Rbox[i].OneBlockPair.size() >NubK)
		{
			NubK = Rbox[i].OneBlockPair.size();
		}
	}
	for (int i = 0; i < Rbox.size(); i++)//将最大主干存入
	{
		if (Rbox[i].OneBlockPair.size() == NubK)
		{
			RboxTemp.push_back(Rbox[i]);
			Rbox.erase(Rbox.begin() + i);
			break;
		}
	}
	bool IfFind = true;
	for (int i = 0; i < path.size();i++)//将分支存入
	{
		vector<int> vec_int;  
		for (int j = 0; j < Rbox.size(); j++)
		{
			IfFind = false;
			vector<PFACETTRI> vec_Pface = Rbox[j].OneBlock;
			for (int m = 0; m < vec_Pface.size(); m++)
			{
				for (int k = 0; k < 3; k++)
				{
					if (path[i][0] == vec_Pface[m]->m_PVerts[k])
					{
						vec_int.push_back(j);
						IfFind = true; break;
					}
				}
				if (IfFind)   break;
			}
		}
		int Itun,NubR=100;
		for (int j = 0; j < vec_int.size();j++)
		{
			if (Rbox[vec_int[j]].OneBlockPair.size()<NubR)
			{
				NubR = Rbox[vec_int[j]].OneBlockPair.size(); Itun = vec_int[j];
			}
		}
		RboxTemp.push_back(Rbox[Itun]);
		Rbox.erase(Rbox.begin() + Itun);
	}
	for (int i = 0; i < RboxTemp.size();i++)
	{
		Rbox.push_back(RboxTemp[i]);
	}
	//建立块的伙伴块序号
	for (int i = 0; i < Rbox.size(); i++)
	{
		for (int j = 0; j < Rbox[i].OneBlockPair.size(); j++)
		{
			for (int k = 0; k < Rbox.size();k++)
			{
				if (Rbox[i].OneBlockPair[j].OneBlock==Rbox[k].OneBlock)
				{
					Rbox[i].vec_PairBlock.push_back(k); break;
				}
			}
		}
	}
	RboxTemp.clear();
}
/////////////////////////////合并小区域
void Cboundary::Skeletonizing_combine(vector<vector<PFACETTRI>>& Rbox,vec_PFACETTRI& m_vecPFacetTri)
{
	sort(Rbox.begin(),Rbox.end());
	vector<PFACETTRI> vecpFacP;
	set<int>::iterator set_it;
	for(int i=1;i<Rbox.size();i++)
	{
		set<int> set_ID;
		for (int j=0;j<Rbox[i].size();j++)
		{
			Kit.FindPOneRFAC_NEW(Rbox[i][j], vecpFacP);
			for (int k=0;k<vecpFacP.size();k++)
			{
			   if (vecpFacP[k]->ID!=Rbox[i].back()->ID&&vecpFacP[k]->ID!=0)
			   {
				   set_ID.insert(vecpFacP[k]->ID);
			   }
			}
		}
		if (set_ID.size()>1)
		{
			int tenmp=0;
			int temp_ID=0;
			for (set_it=set_ID.begin();set_it!=set_ID.end();set_it++)
			{
				if (Rbox[*set_it].size()>tenmp)
				{
					temp_ID= (*set_it);
					tenmp=Rbox[*set_it].size();
				}
		   }
			for (int j=0;j<Rbox[i].size();j++)
			{
				Rbox[i][j]->ID=temp_ID;
				//Rbox[temp_ID].push_back(Rbox[i][j]);
			}
			Rbox[i].clear();
	   }
	}
}
////////////////找到一片Skeletonizing区域的边界
void Cboundary::Skeletonizing_boundary(vec_PEDGE patchT,queue<PHEDGE>& patchB)
{
	for (int i=0;i<patchT.size();i++)
	{
		PHEDGE temp;
		int num=0;
		temp=patchT[i]->pHEdgeAdj->pHEdgeNext;
		while(temp!=patchT[i]->pHEdgeAdj)
		{
			if (temp->bStatus==0)
			{
				num++;
			}
			temp=temp->pHEdgeNext;
		}
		temp=patchT[i]->pHEdgeAdj->pHEdgePair->pHEdgeNext;
		while(temp!=patchT[i]->pHEdgeAdj->pHEdgePair)
		{
			if (temp->bStatus==0)
			{
				num++;
			}
			temp=temp->pHEdgeNext;
		}
		if (num>=1)
		{
			if (patchT[i]->pHEdgeAdj->pHEdgePair!=NULL)
			{
				patchT[i]->pHEdgeAdj->bused=1;
				patchB.push(patchT[i]->pHEdgeAdj);
				patchT[i]->pHEdgeAdj->pHEdgePair->bused=1;
				patchB.push(patchT[i]->pHEdgeAdj->pHEdgePair);
			}
		}
	}
}
void Cboundary::Skeletonizing_find_boundary(vec_PVERT ALOOP,queue<PHEDGE>& patchB,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert)
{
	vec_PVERT vecpVer;
	PHEDGE temp;
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<ALOOP.size();i++)
	{
		ALOOP[i]->bused=1;
		ALOOP[i]->bStatus=1;
	}
	for (int i=0;i<m_vecPFacetTri.size();i++)
	{
		m_vecPFacetTri[i]->bStatus=0;
		if (m_vecPFacetTri[i]->m_PVerts[0]->bStatus==1&&m_vecPFacetTri[i]->m_PVerts[1]->bStatus==1&&m_vecPFacetTri[i]->m_PVerts[2]->bStatus==1)
		{
			m_vecPFacetTri[i]->bStatus=1;
		}
	}
	for (int i=0;i<m_vecPHEdge.size();i++)
	{
		if (m_vecPHEdge[i]->pHEdgePair!=NULL)
		{
			if (m_vecPHEdge[i]->pFacetAdj->bStatus==0&&m_vecPHEdge[i]->pHEdgePair->pFacetAdj->bStatus==1)
			{
				patchB.push(m_vecPHEdge[i]);
				patchB.push(m_vecPHEdge[i]->pHEdgePair);
				temp=m_vecPHEdge[i]->pVertEnd->pHEdgeOut;
				break;
			}
		}
	}
	int test=0;
	while(1)
	{
		test++;
		temp=temp->pHEdgePre->pHEdgePair;
		if (temp->pFacetAdj->bStatus==0&&temp->pHEdgePair->pFacetAdj->bStatus==1)
		{
			if (temp==patchB.front())
			{
				break;
			}
			patchB.push(temp);
			patchB.push(temp->pHEdgePair);
			temp=temp->pVertEnd->pHEdgeOut;	
		}
	}
}
////////////////判断Skeletonizing边界是否该移除
BOOL Cboundary::Skeletonizing_delet(PHEDGE BIAN1,PHEDGE BIAN2)
{
	PHEDGE temp;
	vec_PVERT vecpVer;
	set<PVERT> set_vecpVer;
	int numC=0;
	/*int numS,numT,numC;
	numS=numT=numC=0;*/
	vec_PHEDGE PH_S,PH_T;
	temp=BIAN1;
	while(1)
	{
	   temp=temp->pHEdgePre->pHEdgePair;
		if (temp==BIAN1)
		{
			break;
		}
		if (temp->bStatus==1)
		{
			PH_S.push_back(temp);
		}
	}
	temp=BIAN2;
	while(temp!=BIAN2->pHEdgePair->pHEdgeNext)
	{
		/*if (temp->pHEdgePre->pHEdgePair!=NULL)
		{*/
		   temp=temp->pHEdgePre->pHEdgePair;
		//}
		//if (temp->pHEdgePre->pHEdgePair==NULL)
		//{
		//	temp=BIAN2;
		//	while (temp!=NULL)
		//	{
		//		if (temp->pHEdgePair!=NULL)
		//		{
		//			temp=temp->pHEdgePair->pHEdgeNext;
		//		}
		//		if(temp->bStatus==1)
		//		{
		//			//numT++;
		//			PH_T.push_back(temp);
		//		}
		//	}
		//	break;
		//}
		if (temp==BIAN2)
		{
			break;
		}
		if (temp->bStatus==1)
		{
			//numT++;
			PH_T.push_back(temp);
		}
	}
	for (int i=0;i<PH_S.size();i++)
	{
		set_vecpVer.insert(PH_S[i]->pVertEnd);
	}
	for (int i=0;i<PH_T.size();i++)
	{
		if (set_vecpVer.count(PH_T[i]->pVertEnd)>0)
		{
			numC++;
		}
	}
	if ((PH_S.size()==0&&PH_T.size()>1)||(PH_S.size()>1&&PH_T.size()==0))
	{
		return TRUE;
	}
	if ((PH_S.size()==0&&PH_T.size()==1)||(PH_S.size()==1&&PH_T.size()==0))
	{
		return FALSE;
	}
	if (PH_S.size()>=1&&PH_T.size()>=1)
	{
		if (numC>0)
		{
			return TRUE;
		}
		if (numC==0)
		{
			return FALSE;
		}
	}
}
////////////////////////////////////////////填充着色
void Cboundary::Skeletonizing_smoothing(vector<MyBlock> &Rbox, vector<vector<PVERT>>path, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert)
{
	vec_PHEDGE vecpH;
	Skeletonizing_partlize(Rbox,path,m_vecPFacetTri,m_vecPVert);
	//Skeletonizing_combine(Rbox,m_vecPFacetTri);
}
//////////////////////////////////////找到特征点域
void Cboundary::featurPOINT(vec_PFACETTRI& m_vecPFacetTri, vec_PHEDGE& m_vecPHEdge, vec_PVERT& m_vecPVert,double &NVlimit)//找到特征点域
{
	vector<double> potList;
	double theMAX,theMIN;
	theMAX=0;theMIN=999;
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		double NV=0;
		NV = Kit.CalcuPOINTV(m_vecPVert[i]);
		//////////////////
		potList.push_back(NV);
		theMAX=theMAX+NV;
	}
	double MAMI=theMAX/m_vecPVert.size();
	double SIGEMA=0;
	for (int i=0;i<m_vecPVert.size();i++)
	{
		SIGEMA=SIGEMA+(potList[i]-MAMI)*(potList[i]-MAMI)/m_vecPVert.size();
	}
	SIGEMA=sqrt(SIGEMA);
	for (int i=0;i<m_vecPVert.size();i++)
	{
		double NV=0;
		NV=potList[i];
		NV=abs((NV-MAMI)/SIGEMA);
		potList[i]=NV;
		POINT3D ptemp;
		if (NV>NVlimit)
		{
			//ptemp.x = m_vecPVert[i]->x; ptemp.y = m_vecPVert[i]->y; ptemp.z = m_vecPVert[i]->z;
			//m_vecpcut.push_back(ptemp);
			m_vecPVert[i]->bused=1;
		}
	}
	/*queue<PVERT> scendFind;
	for (int i=0;i<m_vecPVert.size();i++)
	{
		vec_PVERT vecPer;
		int num=0;
		Ccalcubase::FindOneRing(m_vecPVert[i],vecPer);
		for (int j=0;j<vecPer.size();j++)
		{
			if (vecPer[j]->bused==1)
			{
				num++;
			}
		}
		if (num>1&&potList[i]>1.6)
		{
			m_vecPVert[i]->bused=1;
			scendFind.push(m_vecPVert[i]);
		}
	}
	while(!scendFind.empty())
	{
		vec_PVERT vecPer;
		int num=0;
		Ccalcubase::FindOneRing(scendFind.back(),vecPer);
		for (int j=0;j<vecPer.size();j++)
		{
			if (vecPer[j]->bused=0&&potList[vecPer[j]->ID]>1.6)
			{
				vecPer[j]->bused=1;
				scendFind.push(vecPer[j]);
			}
		}
		scendFind.pop();
	}*/
}
void Cboundary::featurPOINT_new(vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert)
{
	///////////////////////
	for (int i=0;i<m_vecPVert.size();i++)
	{
		if (m_vecPVert[i]->bused==1)
		{
			m_vecPVert[i]->bStatus=1;
			m_vecPVert[i]->bused=0;
		}
	}
	///////////////找到几圈特征点 
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vec_PVERT vecpVerR;
	vec_PHEDGE vecpH;
	PHEDGE tempH;
	vector<PVERT> ALOOP;
	vector<PVERT> BLOOP;
	vector<PVERT> CLOOP;
	vector<vector<PVERT>> Loops,exLOOP,inLOOP;
	vector<PVERT> theEND;
	map<PVERT,PVERT> PathA,pathB;
	while(1)
	{
		int k=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bStatus==1&&m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				Q.front()->bused=1;
				k++;
				break;
			}
		}
		if (k==0)
		{
			break;
		}
		vector<PVERT> LOOP;
		while(Q.size()!=0)
		{
			vecpVer.clear();
			Kit.FindOneRing(Q.front(), vecpVer);
			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bStatus==1&&vecpVer[i]->bused==0)
				{
					Q.push(vecpVer[i]);
					vecpVer[i]->bused=1;
				}
			}
			LOOP.push_back(Q.front());
			Q.pop();
		}
		if (LOOP.size()>=10)
		{
			Loops.push_back(LOOP);
		}
		LOOP.clear();
	}
	
	for (int i=0;i<Loops.size();i++)
	{
		for (int j=0;j<Loops[i].size();j++)
		{
			Loops[i][j]->bStatus=0;
			Loops[i][j]->bused=0;
		}
		vector<double> potList;
		double theMAX,theMIN;
		theMAX=0;theMIN=999;
		for (int j=0;j<Loops[i].size();j++)
		{
			double NV=0;
			NV = Kit.CalcuPOINTV(Loops[i][j]);
			//////////////////归一化
			potList.push_back(NV);
			if (NV>=theMAX)
			{
				theMAX=NV;
			}
			if (NV<=theMIN)
			{
				theMIN=NV;
			}
		}
		//////////////////////////////归一化
		double theL(theMAX-theMIN);
		for (int j=0;j<Loops[i].size();j++)
		{
			double NV=0;
			NV=potList[j];
			NV=(NV-theMIN)/theL;
			if (NV>0.5)
			{
				Loops[i][j]->bused=1;
			}
		}
	}
	//////////////////////////////////////////
	//vector<double> potList;
	//double theMAX,theMIN;
	//theMAX=0;theMIN=999;
	//for (int i=0;i<m_vecPVert.size();i++)
	//{
	//	double NV=0;
	//	NV=Ccalcubase::calcuPOINTV(m_vecPVert[i]);
	//	//////////////////
	//	potList.push_back(NV);
	//	theMAX=theMAX+NV;
	//}
	//double MAMI=theMAX/m_vecPVert.size();
	//double SIGEMA=0;
	//for (int i=0;i<m_vecPVert.size();i++)
	//{
	//	SIGEMA=SIGEMA+(potList[i]-MAMI)*(potList[i]-MAMI)/m_vecPVert.size();
	//}
	//SIGEMA=sqrt(SIGEMA);
	//for (int i=0;i<m_vecPVert.size();i++)
	//{
	//	double NV=0;
	//	NV=potList[i];
	//	NV=abs((NV-MAMI)/SIGEMA);
	//	if (NV>1.6)
	//	{
	//		if (m_vecPVert[i]->bStatus==1)
	//		{
	//			m_vecPVert[i]->bStatus=0;
	//			m_vecPVert[i]->bused=1;
	//		}
	//	}
	//}
}
////////////////HU的方法找骨架线
void Cboundary::Skeletonizing(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert
	,vec_PEDGE& m_vecPEdge )
{
	///////////////////////
	for (int i=0;i<m_vecPVert.size();i++)
	{
		if (m_vecPVert[i]->bused==1)
		{
			m_vecPVert[i]->bStatus=1;
			m_vecPVert[i]->bused=0;
		}
	}
	//找到几圈特征点 
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vec_PVERT vecpVerR;
	vec_PHEDGE vecpH;
	PHEDGE tempH;
	vector<PVERT> ALOOP;
	vector<PVERT> BLOOP;
	//vector<vector<PVERT>> RBLOOP,RCLOOP;
	vector<PVERT> CLOOP;
	vector<vector<PVERT>> Loops,exLOOP,inLOOP;
	vector<PVERT> theEND;
	map<PVERT,PVERT> PathA,pathB;
	while(1)
	{
		int k=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bStatus==1&&m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				Q.front()->bused=1;
				k++;
				break;
			}
		}
		if (k==0)
		{
			break;
		}
		vector<PVERT> LOOP;
		while(Q.size()!=0)
		{
			vecpVer.clear();
			//FindTwoRing(Q.front(),vecpVerT);
			Kit.FindOneRing(Q.front(), vecpVer);
			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bStatus==1&&vecpVer[i]->bused==0)
				{
					Q.push(vecpVer[i]);
					vecpVer[i]->bused=1;
				}
			}
			LOOP.push_back(Q.front());
			Q.pop();
		}
		if (LOOP.size()>=10)
		{
			Loops.push_back(LOOP);
		}
		LOOP.clear();
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<Loops.size();i++)
	{
		ALOOP=Loops[i];
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=1;
		}
		///////////////////////形成细骨架线
		vec_PEDGE patchT;
		queue<PHEDGE> patchB;
		for (int j=0;j<m_vecPHEdge.size();j++)
		{
			m_vecPHEdge[j]->bStatus=0;
			m_vecPHEdge[j]->bused=0;
		}
		for (int j=0;j<m_vecPEdge.size();j++)
		{
			m_vecPEdge[j]->bStatus=0;
			if (m_vecPEdge[j]->pVertStart->bused==1&&m_vecPEdge[j]->pVertEnd->bused==1)
			{
				m_vecPEdge[j]->bStatus=1;
				m_vecPEdge[j]->pHEdgeAdj->bStatus=1;
				m_vecPEdge[j]->pHEdgeAdj->pHEdgePair->bStatus=1;
				patchT.push_back(m_vecPEdge[j]);
			}
		}
		for (int j=0;j<m_vecPVert.size();j++)
		{
			m_vecPVert[j]->bused=0;
			m_vecPVert[j]->bStatus=0;
		}
		for (int j=0;j<patchT.size();j++)
		{
			patchT[j]->pVertStart->bused=1;
			patchT[j]->pVertStart->bStatus=1;
			patchT[j]->pVertEnd->bused=1;
			patchT[j]->pVertEnd->bStatus=1;
		}
		Skeletonizing_boundary(patchT,patchB);
		///////////////////////////////ceshi
		for (int j=0;j<m_vecPVert.size();j++)
		{
			m_vecPVert[j]->bused=0;
			m_vecPVert[j]->bStatus=0;
		}
		///////////////////////////////////
		while(!patchB.empty())
		{
			PHEDGE tempb1,tempb2;
			tempb1=patchB.front();
			patchB.pop();
			tempb2=patchB.front();
			patchB.pop();
			tempb1->pVertEnd->bused=1;
			tempb2->pVertEnd->bused=1;
			if (Skeletonizing_delet(tempb1,tempb2)==TRUE)
			{
				tempb1->bStatus=0;
				tempb1->bused=0;
				tempb2->bStatus=0;
				tempb2->bused=0;
				tempb1->pVertEnd->bused=0;
				tempb2->pVertEnd->bused=0;
				///////////插入新的边界边
				if (tempb1->pHEdgeNext->bStatus==1&&tempb1->pHEdgeNext->bused==0)
				{
					tempb1->pHEdgeNext->pVertEnd->bused=1;
					tempb1->pHEdgeNext->pHEdgePair->pVertEnd->bused=1;
					tempb1->pHEdgeNext->bused=1;
					patchB.push(tempb1->pHEdgeNext);
					tempb1->pHEdgeNext->pHEdgePair->bused=1;
					tempb1->pHEdgeNext->pHEdgePair->bStatus=1;
					patchB.push(tempb1->pHEdgeNext->pHEdgePair);
				}
				if (tempb1->pHEdgePre->bStatus==1&&tempb1->pHEdgePre->bused==0)
				{
					tempb1->pHEdgePre->pVertEnd->bused=1;
					tempb1->pHEdgePre->pHEdgePair->bused=1;
					tempb1->pHEdgePre->bused=1;
					patchB.push(tempb1->pHEdgePre);
					tempb1->pHEdgePre->pHEdgePair->bused=1;
					tempb1->pHEdgePre->pHEdgePair->bStatus=1;
					patchB.push(tempb1->pHEdgePre->pHEdgePair);
				}
				if (tempb2->pHEdgeNext->bStatus==1&&tempb2->pHEdgeNext->bused==0)
				{
					tempb2->pHEdgeNext->pVertEnd->bused=1;
					tempb2->pHEdgeNext->pHEdgePair->pVertEnd->bused=1;
					tempb2->pHEdgeNext->bused=1;
					patchB.push(tempb2->pHEdgeNext);
					tempb2->pHEdgeNext->pHEdgePair->bused=1;
					tempb2->pHEdgeNext->pHEdgePair->bStatus=1;
					patchB.push(tempb2->pHEdgeNext->pHEdgePair);
				}
				if (tempb2->pHEdgePre->bStatus==1&tempb2->pHEdgePre->bused==0)
				{
					tempb2->pHEdgePre->pVertEnd->bused=1;
					tempb2->pHEdgePre->pHEdgePair->bused=1;
					tempb2->pHEdgePre->bused=1;
					patchB.push(tempb2->pHEdgePre);
					tempb2->pHEdgePre->pHEdgePair->bused=1;
					tempb2->pHEdgePre->pHEdgePair->bStatus=1;
					patchB.push(tempb2->pHEdgePre->pHEdgePair);
				}
			}
		}
		for (int j=0;j<m_vecPHEdge.size();j++)
		{
			if (m_vecPHEdge[j]->bStatus==1)
			{
				m_vecPHEdge[j]->pHEdgePair->bStatus=1;
			}
		}
			//	////////////////////////显示不包含在RBLOOP里面的点
		for (int j=0;j<m_vecPHEdge.size();j++)
		{
		  if (m_vecPHEdge[j]->bStatus==1)
		   {
		   m_vecPHEdge[j]->pVertEnd->bused=1;
		   }
		}
	}
	queue<PVERT> END_POINT;
	queue<PVERT> bShar_POINT;
	vec_PVERT cross_POINT;//交点
	map<PVERT,vector<vec_PVERT>> ba_list;//所有分支
	map<PVERT,vector<vec_PVERT>> ex_list;//外分支
	map<PVERT,vector<vec_PVERT>> in_list;//内分支
	for(int i=0;i<Loops.size();i++)
	{
		ALOOP=Loops[i];
			for (int j=0;j<ALOOP.size();j++)
			{
				//ALOOP[j]->bused=0;
				//ALOOP[j]->bStatus=0;
				ALOOP[j]->theEND=0;
				ALOOP[j]->bSharpVer=0;
				Kit.FindOnePH(ALOOP[j], vecpH);
				int jishu=0;
				for (int num=0;num<vecpH.size();num++)
				{
					if (vecpH[num]->bStatus==1)
					{
						jishu++;
					}
				}
				if (jishu==1)
				{
					ALOOP[j]->theEND=1;
					END_POINT.push(ALOOP[j]);
				}
				if (jishu>2)
				{
					////////////
					ALOOP[j]->theEND=1;
					ALOOP[j]->bSharpVer=1;
					bShar_POINT.push(ALOOP[j]);
					cross_POINT.push_back(ALOOP[j]);
				}
			}
	}
	for (int jj=0;jj<m_vecPHEdge.size();jj++)
	{
		if (m_vecPHEdge[jj]->bused==1)
		{
			m_vecPHEdge[jj]->bStatus=1;
		}
	}
			////////////////////////////////////找外分枝
			CLOOP.clear();
			if (END_POINT.size()>0)
			{
				END_POINT.front()->bused=1;
				CLOOP.push_back(END_POINT.front());
				while (1)
				{
					Kit.FindOnePH(CLOOP.back(), vecpH);
					int numJ=0;
					for (int num=0;num<vecpH.size();num++)
					{
						if (vecpH[num]->bStatus==1)
						{
							if (vecpH[num]->pVertEnd->theEND!=1)
							{
								//vecpH[num]->bStatus=0;
								vecpH[num]->pHEdgePair->bStatus=0;
								vecpH[num]->pHEdgePair->be_pair=1;
								vecpH[num]->pVertEnd->bused=1;
								CLOOP.push_back(vecpH[num]->pVertEnd);
								break;
							}
							if (vecpH[num]->pVertEnd->theEND==1)
							{
								//vecpH[num]->bStatus=0;
								vecpH[num]->pHEdgePair->bStatus=0;
								vecpH[num]->pHEdgePair->be_pair=1;
								vecpH[num]->pVertEnd->bused=1;
								vecpH[num]->pVertEnd->bStatus=1;
								CLOOP.back()->bStatus=1;
								CLOOP.push_back(vecpH[num]->pVertEnd);
								exLOOP.push_back(CLOOP);
								CLOOP.clear();
							    END_POINT.pop();
								   if (!END_POINT.empty())
								   {
									   CLOOP.push_back(END_POINT.front());
								   }
								break;
							}
						}
						else
						{
							numJ++;
						}
					}
					if (numJ==vecpH.size())
					{
						CLOOP.clear();
						END_POINT.pop();
						if (!END_POINT.empty())
						{
							CLOOP.push_back(END_POINT.front());
						}
					}
					if (CLOOP.empty())
					{
						break;
					}
					if (END_POINT.size()==0)
					{
						break;
					}
				}
			}
			for (int ii=0;ii<exLOOP.size();ii++)
			{
				for (int jj=0;jj<exLOOP[ii].size();jj++)
				{
					exLOOP[ii][jj]->theEND=1;exLOOP[ii][jj]->bStatus=1;
				}
				exLOOP[ii].back()->bStatus=0;
			}
	///////	///////////////////内分枝
			CLOOP.clear();
			if (!bShar_POINT.empty())
			{
				bShar_POINT.front()->bused=1;
				CLOOP.push_back(bShar_POINT.front());
				while(1)
				{
					Kit.FindOnePH(CLOOP.back(),vecpH);
					int numJ=0;
					for (int num=0;num<vecpH.size();num++)
					{
						if (vecpH[num]->bStatus==1&&vecpH[num]->pVertEnd->bStatus==0)
						{
							if (vecpH[num]->pVertEnd->theEND!=1)
							{
								vecpH[num]->bStatus=0;
								vecpH[num]->pHEdgePair->bStatus=0;
								vecpH[num]->pVertEnd->bused=1;
								CLOOP.push_back(vecpH[num]->pVertEnd);
								break;
							}
							if (vecpH[num]->pVertEnd->theEND==1)
							{
								vecpH[num]->bStatus=0;
								vecpH[num]->pHEdgePair->bStatus=0;
								vecpH[num]->pVertEnd->bused=1;
								CLOOP.back()->bStatus=1;
								CLOOP.push_back(vecpH[num]->pVertEnd);
								inLOOP.push_back(CLOOP);
								CLOOP.clear();
								CLOOP.push_back(bShar_POINT.front());
								break;
							}
						}
						else
						{
							vecpH[num]->bStatus=0;
							vecpH[num]->pHEdgePair->bStatus=0;
							numJ++;
						}
					}
					if (numJ==vecpH.size())
					{
						bShar_POINT.pop();
						for (int numk=0;numk<CLOOP.size();numk++)
						{
							CLOOP[numk]->bused=0;
						}
						if (bShar_POINT.empty())
						{
							break;
						}
						CLOOP.clear();
						CLOOP.push_back(bShar_POINT.front());
					}
				}
			}
	/////////////////////////////////////////////////////////建立分支的结构
	for (int ii=0;ii<exLOOP.size();ii++)
	{
		snake_smooth(exLOOP[ii]);
		ex_list[exLOOP[ii].back()].push_back(exLOOP[ii]);
	    ba_list[exLOOP[ii].back()].push_back(exLOOP[ii]);
	}
	for (int ii=0;ii<inLOOP.size();ii++)
	{
		snake_smooth(inLOOP[ii]);
		vec_PVERT thevecT;
		for (int jj=inLOOP[ii].size()-1;jj>=0;jj--)
		{
			thevecT.push_back(inLOOP[ii][jj]);
		}
		in_list[inLOOP[ii][0]].push_back(thevecT);
		in_list[inLOOP[ii].back()].push_back(inLOOP[ii]);
		ba_list[inLOOP[ii][0]].push_back(thevecT);
		ba_list[inLOOP[ii].back()].push_back(inLOOP[ii]);
		for (int jj=0;jj<inLOOP.size();jj++)
		{
			if (ii!=jj)
			{
				if (inLOOP[ii][0]==inLOOP[jj][0]&&inLOOP[ii].back()==inLOOP[jj].back())
				{
					inLOOP.erase(inLOOP.begin()+jj);
					jj--;
				}
				if (inLOOP[ii][0]==inLOOP[jj].back()&&inLOOP[ii].back()==inLOOP[jj][0])
				{
					inLOOP.erase(inLOOP.begin()+jj);
					jj--;
				}
			}
		}
	}
	/////////////////////////////////////////////////////剪枝
	for (int ii=0;ii<cross_POINT.size();ii++)
	{
		int numB=0;
		numB=ba_list[cross_POINT[ii]].size();
		vector<vec_PVERT> vec_deLIN;
		if (numB==3)
		{
			vec_deLIN=ba_list[cross_POINT[ii]];
			snake_delete(vec_deLIN);
			ba_list[cross_POINT[ii]]=vec_deLIN;
			cross_POINT[ii]->theEND=0;
		}
	}
	for (int ii=0;ii<exLOOP.size();ii++)
	{
		if (exLOOP[ii].size()>=5)
		{
			if (exLOOP[ii].back()->theEND==1)
			{
				RBLOOP.push_back(exLOOP[ii]);
			}
		}	
	}
	//RBLOOP.insert(RBLOOP.end(),inLOOP.begin(),inLOOP.end());
	//////////////////////////////////////////////////////连成光顺的线
	/*RBLOOP=inLOOP;
	RBLOOP.insert(RBLOOP.end(),exLOOP.begin(),exLOOP.end());*/
	for (int numi=0;numi<RBLOOP.size();numi++)
	{
		snake_smooth(RBLOOP[numi]);
	}
	for (int i=0;i<m_vecPHEdge.size();i++)
	{
		m_vecPHEdge[i]->bused=0;
		m_vecPHEdge[i]->bStatus=0;
	}
	for (int i=0;i<RBLOOP.size();i++)
	{
		for (int j=0;j<RBLOOP[i].size();j++)
		{
			RBLOOP[i][j]->bused=1;
			Kit.FindOnePH(RBLOOP[i][j],vecpH);
			for (int num=0;num<vecpH.size();num++)
			{
				if (vecpH[num]->pVertEnd==RBLOOP[i][(j+1)%(RBLOOP[i].size())])
				{
					vecpH[num]->bused=1;
					vecpH[num]->pHEdgePair->bused=1;
				}
			}
		}
	}
}
void Cboundary::snake_delete(vector<vector<PVERT>>& deLoop)
{
	int theLong=0;int temp=deLoop[0].size();
	vec_PVERT vec_long;
	for (int i=0;i<deLoop.size();i++)
	{
		if (deLoop[i].size()>temp)
		{
			temp=deLoop[i].size();
			theLong=i;
		}
	}
	vec_long=deLoop[theLong];
	deLoop.erase(deLoop.begin()+theLong);
	////////////////////////////////每一条与最长边相连，比能量大小
	double theEX=999999;int theShort=0;
	vec_PVERT theLIN;
	for (int i=0;i<deLoop.size();i++)
	{
		vec_PVERT vec_temp=vec_long;
		double temp_EX=0;
		for (int j=deLoop[i].size()-2;j>=0;j--)
		{
			vec_temp.push_back(deLoop[i][j]);
		}
		temp_EX=snak_energe(vec_temp);
		if (temp_EX<=theEX)
		{
		   theShort=i;
		   //theLIN=vec_temp;
		}
	}
	theLIN=deLoop[theShort];
	deLoop.clear();
	//deLoop.push_back(theLIN);
	deLoop.push_back(vec_long);
	deLoop.push_back(theLIN);
}
double Cboundary::snak_energe(vector<PVERT>& Bloop)
{
	double A,B,C;
	double allext=0;double allinE=0;
	for (int i=1;i<Bloop.size()-1;i++)
	{
		VECTOR3D v1,v2,v3;double inE=0;
		v1.dx=Bloop[i-1]->x-Bloop[i]->x;v1.dy=Bloop[i-1]->y-Bloop[i]->y;v1.dz=Bloop[i-1]->z-Bloop[i]->z;
		v2.dx=Bloop[i+1]->x-Bloop[i]->x;v1.dy=Bloop[i+1]->y-Bloop[i]->y;v1.dz=Bloop[i+1]->z-Bloop[i]->z;
		A=v1.GetLength();
		v1.Normalize();v2.Normalize();
		v3+=v1+v2;
		B=v3.GetLength();
		inE=0.01*A+0.99*B;
		allinE=allinE+inE;

		vec_PVERT vecpVer;
		double ext=0;
		double vi = Kit.CalcuPOINTV(Bloop[i]);
		//double vi=Ccalcubase::calcuNoV_new(Bloop[i]);
		ext=(1-vi);
		allext=allext+ext;
	}
	C=(0.2*allinE+0.8*allext)/Bloop.size();
	return C;
}
/////////////////自己的方法找骨架线
void Cboundary::Skeletonizing_new(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge
	,vec_PEDGE& m_vecPEdge )
{
	for (int i=0;i<m_vecPVert.size();i++)
	{
		if (m_vecPVert[i]->bused==1)
		{
			m_vecPVert[i]->bStatus=1;
			m_vecPVert[i]->bused=0;
		}
	}
	//找到几圈特征点 
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vec_PVERT vecpVerR;
	vec_PHEDGE vecpH;
	PHEDGE tempH;
	vector<PVERT> ALOOP;
	vector<PVERT> BLOOP;
	//vector<vector<PVERT>> RBLOOP,RCLOOP;
	vector<PVERT> CLOOP;
	vector<vector<PVERT>> Loops,exLOOP,inLOOP;
	vector<PVERT> theEND;
	map<PVERT,PVERT> PathA,pathB;
	while(1)
	{
		int k=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bStatus==1&&m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				Q.front()->bused=1;
				k++;
				break;
			}
		}
		if (k==0)
		{
			break;
		}
		vector<PVERT> LOOP;
		while(Q.size()!=0)
		{
			vecpVer.clear();
			//FindTwoRing(Q.front(),vecpVerT);
			Kit.FindOneRing(Q.front(), vecpVer);
			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bStatus==1&&vecpVer[i]->bused==0)
				{
					Q.push(vecpVer[i]);
					vecpVer[i]->bused=1;
				}
			}
			LOOP.push_back(Q.front());
			Q.pop();
		}
		if (LOOP.size()>=10)
		{
			Loops.push_back(LOOP);
		}
		LOOP.clear();
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<Loops.size();i++)
	{
		ALOOP=Loops[i];
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=1;
		}
		////////////////////////////////////////
		/////////////////////形成细骨架线
		vec_PEDGE patchT;
		queue<PHEDGE> patchB;
		for (int j=0;j<m_vecPHEdge.size();j++)
		{
			m_vecPHEdge[j]->bStatus=0;
			m_vecPHEdge[j]->bused=0;
		}
		for (int j=0;j<m_vecPEdge.size();j++)
		{
			m_vecPEdge[j]->bStatus=0;
			if (m_vecPEdge[j]->pVertStart->bused==1&&m_vecPEdge[j]->pVertEnd->bused==1)
			{
				m_vecPEdge[j]->bStatus=1;
				m_vecPEdge[j]->pHEdgeAdj->bStatus=1;
				m_vecPEdge[j]->pHEdgeAdj->pHEdgePair->bStatus=1;
				patchT.push_back(m_vecPEdge[j]);
			}
		}
		for (int j=0;j<m_vecPVert.size();j++)
		{
			m_vecPVert[j]->bused=0;
			m_vecPVert[j]->bStatus=0;
		}
		for (int j=0;j<patchT.size();j++)
		{
			patchT[j]->pVertStart->bused=1;
			patchT[j]->pVertStart->bStatus=1;
			patchT[j]->pVertEnd->bused=1;
			patchT[j]->pVertEnd->bStatus=1;
		}
		Skeletonizing_boundary(patchT,patchB);
		///////////////////////////////ceshi
		for (int j=0;j<m_vecPVert.size();j++)
		{
			m_vecPVert[j]->bused=0;
			m_vecPVert[j]->bStatus=0;
		}
		///////////////////////////////////
		while(!patchB.empty())
		{
			PHEDGE tempb1,tempb2;
			tempb1=patchB.front();
			patchB.pop();
			tempb2=patchB.front();
			patchB.pop();
			tempb1->pVertEnd->bused=1;
			tempb2->pVertEnd->bused=1;
			if (Skeletonizing_delet(tempb1,tempb2)==TRUE)
			{
				tempb1->bStatus=0;
				tempb1->bused=0;
				tempb2->bStatus=0;
				tempb2->bused=0;
				tempb1->pVertEnd->bused=0;
				tempb2->pVertEnd->bused=0;
				///////////插入新的边界边
				if (tempb1->pHEdgeNext->bStatus==1&&tempb1->pHEdgeNext->bused==0)
				{
					tempb1->pHEdgeNext->pVertEnd->bused=1;
					tempb1->pHEdgeNext->pHEdgePair->pVertEnd->bused=1;
					tempb1->pHEdgeNext->bused=1;
					patchB.push(tempb1->pHEdgeNext);
					tempb1->pHEdgeNext->pHEdgePair->bused=1;
					tempb1->pHEdgeNext->pHEdgePair->bStatus=1;
					patchB.push(tempb1->pHEdgeNext->pHEdgePair);
				}
				if (tempb1->pHEdgePre->bStatus==1&&tempb1->pHEdgePre->bused==0)
				{
					tempb1->pHEdgePre->pVertEnd->bused=1;
					tempb1->pHEdgePre->pHEdgePair->bused=1;
					tempb1->pHEdgePre->bused=1;
					patchB.push(tempb1->pHEdgePre);
					tempb1->pHEdgePre->pHEdgePair->bused=1;
					tempb1->pHEdgePre->pHEdgePair->bStatus=1;
					patchB.push(tempb1->pHEdgePre->pHEdgePair);
				}
				if (tempb2->pHEdgeNext->bStatus==1&&tempb2->pHEdgeNext->bused==0)
				{
					tempb2->pHEdgeNext->pVertEnd->bused=1;
					tempb2->pHEdgeNext->pHEdgePair->pVertEnd->bused=1;
					tempb2->pHEdgeNext->bused=1;
					patchB.push(tempb2->pHEdgeNext);
					tempb2->pHEdgeNext->pHEdgePair->bused=1;
					tempb2->pHEdgeNext->pHEdgePair->bStatus=1;
					patchB.push(tempb2->pHEdgeNext->pHEdgePair);
				}
				if (tempb2->pHEdgePre->bStatus==1&tempb2->pHEdgePre->bused==0)
				{
					tempb2->pHEdgePre->pVertEnd->bused=1;
					tempb2->pHEdgePre->pHEdgePair->bused=1;
					tempb2->pHEdgePre->bused=1;
					patchB.push(tempb2->pHEdgePre);
					tempb2->pHEdgePre->pHEdgePair->bused=1;
					tempb2->pHEdgePre->pHEdgePair->bStatus=1;
					patchB.push(tempb2->pHEdgePre->pHEdgePair);
				}
			}
		}
		//////////////////////////////////////////
		queue<PVERT> END_POINT;
		queue<PVERT> bShar_POINT;
		for (int j=0;j<ALOOP.size();j++)
		{
			Kit.FindOnePH(ALOOP[j], vecpH);
			int jishu=0;
			for (int num=0;num<vecpH.size();num++)
			{
				if (vecpH[num]->bStatus==1)
				{
					jishu++;
				}
			}
			if (jishu==1)
			{
				ALOOP[j]->theEND=1;
				END_POINT.push(ALOOP[j]);
			}
		}
	    ///////////////////////
		/////////////////////////////
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=1;
		}
	    if (!END_POINT.empty())
		{
		  snke_parameter(ALOOP,m_vecPFacetTri);
		  break;
		}
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=0;
		}
	}
}
////////////////空心域收缩法
void Cboundary::Skeletonizing_new_1(vector<vector<PVERT>>& RBLOOP,vec_PVERT& m_vecPVert)
{
	for (int i=0;i<m_vecPVert.size();i++)
	{
		if (m_vecPVert[i]->bused==1)
		{
			m_vecPVert[i]->bStatus=1;
			m_vecPVert[i]->bused=0;
		}
	}
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vector<PVERT> ALOOP,BLOOP;
	vector<PVERT>::iterator ite;
	vector<vector<PVERT>> Loops;
	while(1)
	{
		int k=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bStatus==1&&m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				Q.front()->bused=1;
				k++;
				break;
			}
		}
		if (k==0)
		{
			break;
		}
		vector<PVERT> LOOP;
		while(Q.size()!=0)
		{
			vecpVer.clear();
			Kit.FindOneRing(Q.front(), vecpVer);
			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bStatus==1&&vecpVer[i]->bused==0)
				{
					Q.push(vecpVer[i]);
					vecpVer[i]->bused=1;
				}
			}
			LOOP.push_back(Q.front());
			Q.pop();
		}
		if (LOOP.size()>=5)
		{
			Loops.push_back(LOOP);
		}
		LOOP.clear();
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<Loops.size();i++)
	{
		ALOOP=Loops[i];
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=1;
			ALOOP[j]->bStatus=0;
			ALOOP[j]->bSharpVer=0;
		}
	}
	int LopSize=Loops.size();
	for (int i=0;i<LopSize;i++)
	{
		BLOOP.clear();
		ALOOP=Loops[i];
		int jishu=0;
		/////////////挖空
			jishu=0;
			for (int j=0;j<ALOOP.size();j++)
			{
				ALOOP[j]->bSharpVer=1;
				Kit.FindOneRing(ALOOP[j], vecpVer);
				int numA=0;
				for (int num=0;num<vecpVer.size();num++)
				{
					if (vecpVer[num]->bStatus==0)
					{
						if (vecpVer[num]->bused==1)
						{
							if (num>0)
							{
								if (vecpVer[num-1]->bused==1)
								{
									if (vecpVer[(num+1)%vecpVer.size()]->bused==0&&vecpVer[(num+1)%vecpVer.size()]->bStatus==0)
									{
										vecpVer[num]->bStatus=1;
										vecpVer[num]->bSharpVer=1;
										BLOOP.push_back(vecpVer[num]);
										numA++;
										break;
									}
								}
							}
							if (num==0)
							{
								if (vecpVer.back()->bused==1)
								{
									if (vecpVer[(num+1)%vecpVer.size()]->bused==0&&vecpVer[(num+1)%vecpVer.size()]->bStatus==0)
									{
										vecpVer[num]->bStatus=1;
										vecpVer[num]->bSharpVer=1;
										BLOOP.push_back(vecpVer[num]);
										numA++;
										break;
									}
								}
							}
						}
					}
				}
				if (numA==1)
				{
					break;
				}
			}
			while(1)
			{
				Kit.FindOneRing(BLOOP.back(), vecpVer);
				int numA=0;
				for (int num=0;num<vecpVer.size();num++)
				{
					if (vecpVer[num]->bStatus==0)
					{
						if (vecpVer[num]->bused==1)
						{
							if (num>0)
							{
								if (vecpVer[num-1]->bused==1)
								{
									if (vecpVer[(num+1)%vecpVer.size()]->bused==0&&vecpVer[(num+1)%vecpVer.size()]->bStatus==0)
									{
										vecpVer[num]->bStatus=1;
										vecpVer[num]->bSharpVer=1;
										BLOOP.push_back(vecpVer[num]);
										numA++;
										break;
									}
								}
							}
							if (num==0)
							{
								if (vecpVer.back()->bused==1)
								{
									if (vecpVer[(num+1)%vecpVer.size()]->bused==0&&vecpVer[(num+1)%vecpVer.size()]->bStatus==0)
									{
										vecpVer[num]->bStatus=1;
										vecpVer[num]->bSharpVer=1;
										BLOOP.push_back(vecpVer[num]);
										numA++;
										break;
									}
								}
							}
						}
					}
				}
				if (numA==0)
				{
					break;
				}
			}
		Loops[i]=BLOOP;
		for (int j=0;j<ALOOP.size();j++)
		{
			ALOOP[j]->bused=0;
			ALOOP[j]->bStatus=0;
			ALOOP[j]->bSharpVer=0;
		}
		for (int j=0;j<BLOOP.size();j++)
		{
			BLOOP[j]->bused=1;
			BLOOP[j]->bStatus=1;
		}
	}
	
		//snke_contract(RBLOOP[numi]);
}
////////////////形成封闭的边界线
void Cboundary::Skeletonizing_close(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge,double &Ang)
{
	int myCtrlPos;
	for (int j=0;j<m_vecPVert.size();j++)
	{
		m_vecPVert[j]->theEND=0;
	}
	for (int j=0;j<RBLOOP.size();j++)
	{
		Skeletonizing_link(RBLOOP[j],m_vecPFacetTri,m_vecPVert,m_vecPHEdge,Ang);
	   // Skeletonizing_link_new(RBLOOP[j],m_vecPFace五轴轨迹tTri,m_vecPVert);
		//snaking(RBLOOP[j],m_vecPVert);

		snake_smooth_close(RBLOOP[j]);
	}
}
void Cboundary::sort_smooth(vec_PVERT& templine)
{
	vec_PHEDGE vecpH;
	vec_PVERT line,vecpVer;
	vector<PVERT>::iterator sort_iter;
	for (int i=0;i<templine.size();i++)
	{
		int numb=0;
		Kit.FindOneRing(templine[i], vecpVer);
		for (int j=0;j<vecpVer.size();j++)
		{
			if (vecpVer[j]->bused==1)
			{
				numb++;
			}
		}
		if (numb==0)
		{
			templine[i]->bused=0;
			sort_iter=templine.begin()+i;
			templine.erase(sort_iter);
			i--;
		}
	}
	for (int i=0;i<templine.size();i++)
	{
		int numb=0;
		Kit.FindOnePH(templine[i], vecpH);
		for (int j=0;j<vecpH.size();j++)
		{
			if (vecpH[j]->pVertEnd==templine[(i+1)%(templine.size())])
			{
				numb++;
				vecpH[j]->bStatus=1;
				vecpH[j]->pHEdgePair->bStatus=1;
				line.push_back(templine[i]);
			}
		}
		if (numb==0)
		{
			for (int numC=i;numC<templine.size();numC++)
			{
				templine[numC]->bused=0;
			}
			break;
		}
	}
	templine=line;
	for (int i=0;i<templine.size();i++)
	{
		if (templine[i]->bused==1)
		{
			Kit.FindOnePH(templine[i], vecpH);
			for (int j=0;j<vecpH.size();j++)
			{
				if (vecpH[j]->bStatus==1)
				{
					if (vecpH[j]->pHEdgeNext->bStatus==1)
					{
						if (vecpH[j]->pHEdgeNext->pHEdgeNext->bStatus==0&&vecpH[j]->pHEdgeNext->pHEdgeNext->pHEdgePair->bStatus==0)
						{
							vecpH[j]->bStatus=0;
							vecpH[j]->pHEdgePair->bStatus=0;
							vecpH[j]->pHEdgeNext->bStatus=0;
							vecpH[j]->pHEdgeNext->pHEdgePair->bStatus=0;
							vecpH[j]->pHEdgeNext->pHEdgeNext->bStatus=1;
							vecpH[j]->pHEdgeNext->pHEdgeNext->pHEdgePair->bStatus=1;
							vecpH[j]->pVertEnd->bused=0;
							//vecpH[j]->pVertEnd->theEND=1;
						}
					}
				}
			}
		}
	}
}
/////////////////利用谱聚类找骨架线
void Cboundary::Laboundary(vector<vector<PFACETTRI>>& Rbox,vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert)
{
	for (int i=0;i<m_vecPVert.size();i++)
	{
		if (m_vecPVert[i]->bused==1)
		{
			m_vecPVert[i]->bStatus=1;
			m_vecPVert[i]->bused=0;
		}
	}
	//找到几圈特征点 
	queue<PVERT> Q;
	vec_PVERT vecpVer;
	vector<vector<PVERT>> Loops;
	while(1)
	{
		int k=0;
		for (int i=0;i<m_vecPVert.size();i++)
		{
			if (m_vecPVert[i]->bStatus==1&&m_vecPVert[i]->bused==0)
			{
				Q.push(m_vecPVert[i]);
				Q.front()->bused=1;
				k++;
				break;
			}
		}
		if (k==0)
		{
			break;
		}
		vector<PVERT> LOOP;
		while(Q.size()!=0)
		{
			vecpVer.clear();
			//FindTwoRing(Q.front(),vecpVerT);
			Kit.FindOneRing(Q.front(), vecpVer);
			for (int i=0;i<vecpVer.size();i++)
			{
				if (vecpVer[i]->bStatus==1&&vecpVer[i]->bused==0)
				{
					Q.push(vecpVer[i]);
					vecpVer[i]->bused=1;
				}
			}
			LOOP.push_back(Q.front());
			Q.pop();
		}
		if (LOOP.size()>=10)
		{
			Loops.push_back(LOOP);
		}
		LOOP.clear();
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bused=0;
		m_vecPVert[i]->bStatus=0;
	}
	for (int i=0;i<Loops.size();i++)
	{
		for (int j=0;j<Loops[i].size();j++)
		{
			Loops[i][j]->bused=1;
		}
	}
	RBLOOP=Loops;
	Loops.clear();
		////////////////////////////////////////////////////
		/////////////////////////////////
	for (int i=0;i<RBLOOP.size();i++)
	{
		for (int j=0;j<RBLOOP[i].size();j++)
		{
			RBLOOP[i][j]->bused=0;
		}
	}
	for (int i=0;i<RBLOOP.size();i++)
	{
		vec_PFACETTRI facelist;
		for (int j=0;j<RBLOOP[i].size();j++)
		{
			RBLOOP[i][j]->bused=1;
		}
		for (int j=0;j<m_vecPFacetTri.size();j++)
		{
			m_vecPFacetTri[j]->bStatus=0;
			if (m_vecPFacetTri[j]->m_PVerts[0]->bused==1&&m_vecPFacetTri[j]->m_PVerts[1]->bused==1&&m_vecPFacetTri[j]->m_PVerts[2]->bused==1)
			{
				m_vecPFacetTri[j]->bStatus=1;
			   facelist.push_back(m_vecPFacetTri[j]);
			}
		}
		for (int j=0;j<RBLOOP[i].size();j++)
		{
			RBLOOP[i][j]->bused=0;
		}
		//Laboundary_L(Rbox,facelist);
		Laboundary_L(Rbox,m_vecPFacetTri);
		break;
	}
}
void Cboundary::Laboundary_L(vector<vector<PFACETTRI>>& Rbox,vec_PFACETTRI& facelist)
{
	double avgDis,avgAng;
	avgDis=avgAng=0;
	get_avg(avgDis,avgAng,facelist);
	double sigema=2*avgDis*avgDis;
	sigema=1/sigema;
	/////构造Laplace矩阵
	MatrixXd Ones(facelist.size(),facelist.size()),D(facelist.size(),facelist.size()),L(facelist.size(),facelist.size());
	for (int i=0;i<facelist.size();i++)
	{
		for (int j=0;j<facelist.size();j++)
		{
			BOOL tehis=0;
			if (j==i)
			{
				Ones(i,j)=1;
				tehis=1;
			}
			else
			{
				if (facelist[i]->pHEdge->pHEdgePair->pFacetAdj==facelist[j])
				{
					double disANG=0;
					double disDIS=0;
					double weight=0;
					POINT3D pA,pB;
					VECTOR3D v1,v2;
					pB = Kit.Center(facelist[i]->pHEdge->pHEdgePair->pFacetAdj->pHEdge);
					pA = Kit.Center(facelist[j]->pHEdge);
					get_dis(facelist[i]->pHEdge,disDIS,disANG);
					weight=0.01*disDIS*avgDis+0.99*disANG*avgAng;
					 Ones(i,j)=1/exp(weight*sigema);
					 tehis=1;
				}
				if (facelist[i]->pHEdge->pHEdgePre->pHEdgePair->pFacetAdj==facelist[j])
				{
					double disANG=0;
					double disDIS=0;
					double weight=0;
					POINT3D pA,pB;
					VECTOR3D v1,v2;
					pB = Kit.Center(facelist[i]->pHEdge->pHEdgePre->pHEdgePair->pFacetAdj->pHEdge);
					pA = Kit.Center(facelist[j]->pHEdge);
					get_dis(facelist[i]->pHEdge->pHEdgePre,disDIS,disANG);
					weight=0.01*disDIS*avgDis+0.99*disANG*avgAng;
					Ones(i,j)=1/exp(weight*sigema);
					tehis=1;
				}
				if (facelist[i]->pHEdge->pHEdgeNext->pHEdgePair->pFacetAdj==facelist[j])
				{
					double disANG=0;
					double disDIS=0;
					double weight=0;
					POINT3D pA,pB;
					VECTOR3D v1,v2;
					pB = Kit.Center(facelist[i]->pHEdge->pHEdgeNext->pHEdgePair->pFacetAdj->pHEdge);
					pA = Kit.Center(facelist[j]->pHEdge);
					get_dis(facelist[i]->pHEdge->pHEdgeNext,disDIS,disANG);
					weight=0.01*disDIS*avgDis+0.99*disANG*avgAng;
					Ones(i,j)=1/exp(weight*sigema);
					tehis=1;
				}
			}
			if (tehis==0)
			{
				Ones(i,j)=0;
			}
		}
	}

	/*	double test_double[100][100];
		for (int i=0;i<facelist.size();i++)
		{
		  for (int j=0;j<facelist.size();j++)
		  {
			  test_double[i][j]=Ones(i,j);
		  }
		}*/
	////////////////////////////////构造D
	for (int i=0;i<facelist.size();i++)
	{
		double dii=0;
		for (int j=0;j<facelist.size();j++)
		{
			dii=dii+Ones(i,j);
			if (j!=i)
			{
				D(i,j)=0;
			}
		}
		D(i,i)=1;
	}
	/////////////////////////构造L
	L=D-Ones;
	for (int i=0;i<facelist.size();i++)
	{
		for (int j=0;j<facelist.size();j++)
		{
			L(i,j)=L(i,j)/sqrt(D(i,i)*D(j,j));
		}
	}
	////////////////////////////////求解特征向量
	EigenSolver<MatrixXd> es(L);
	 VectorXcd eiValues = es.eigenvalues() ;
	 vector<double> theValue;
	 vector<vector<double>> theVector;
	 for (int i=0;i<facelist.size();i++)
	 {
		 double temp=0;
		 temp=eiValues(i).real();
		 theValue.push_back(temp);
	 }
	 map<double,int> valueList;
	 map<double,int>::reverse_iterator theITE;
	 for (int i=0;i<theValue.size();i++)
	 {
		 valueList.insert(pair<double,int>(theValue[i],i));
	 }
	 int numlist=0;
	 for (theITE=valueList.rbegin();theITE!=valueList.rend();theITE++)
	 {
		 vector<double> tempVec;
		 VectorXcd eiVector=es.eigenvectors().col(theITE->second);
		 double chan=eiValues(theITE->second).real()*eiValues(theITE->second).real();
		 chan=sqrt(chan);
		 for (int i=0;i<facelist.size();i++)
		 {
			tempVec.push_back(chan*eiVector(i).real());
		 }
		 theVector.push_back(tempVec);
		 numlist++;
		 if (numlist==10)
		 {
			 break;
		 }
	 }
	 LaMeans(Rbox,theVector,facelist);
}
void Cboundary::LaMeans(vector<vector<PFACETTRI>>& Rbox,vector<vector<double>>& theVector,vec_PFACETTRI& facelist)
{
	vector<vector<double>> vec_thePOT;
	int theS,theT;
	theS=theT=0;
	for (int i=0;i<theVector.back().size();i++)
	{
		vector<double> thePOT;
		for (int j=0;j<theVector.size();j++)
		{
			thePOT.push_back(theVector[j][i]);
		}
		vec_thePOT.push_back(thePOT);//这里vec_thePOT的索引与facelist的索引是一一对应的
	}
	//////////////////////////////////////////初始化中心点
	theS=0;theT=vec_thePOT.size()-1;
	double Qrt=99999;
	for (int i=0;i<vec_thePOT.size();i++)
	{
		double bijiao=0;
		for (int j=0;j<vec_thePOT.back().size();j++)
		{
			bijiao=bijiao+vec_thePOT[theS][j]*vec_thePOT[i][j];
		}
		if (bijiao<=Qrt)
		{
			Qrt=bijiao;
			theT=i;
		}
	}
	Qrt=99999;
	for (int i=0;i<vec_thePOT.size();i++)
	{
		double bijiao=0;
		for (int j=0;j<vec_thePOT.back().size();j++)
		{
			bijiao=bijiao+vec_thePOT[theT][j]*vec_thePOT[i][j];
		}
		if (bijiao<=Qrt)
		{
			Qrt=bijiao;
			theS=i;
		}
	}
	/////////////////////////////////////////////
	vector<int> vec_S,vec_T;
	double jishu=0;
	while(1)
	{
		///////////////////////////////////////////判断属于哪个类别
		vec_S.clear();vec_T.clear();
		for (int i=0;i<vec_thePOT.size();i++)
		{
			double biA,biB;
			biA=biB=0;
			biA=LaMeans_dis(vec_thePOT[theS],vec_thePOT[i]);
			biB=LaMeans_dis(vec_thePOT[theT],vec_thePOT[i]);
			if (biA>=biB)
			{
				vec_S.push_back(i);
			}
			else
			{
				vec_T.push_back(i);
			}
		}

		if (jishu==100)
		{
			break;
		}
	  ////////////////////////////////////////////////S类选中心点
		double tempS=999999;double tempT=999999;
		double TtheS,TtheT;TtheS=theS;TtheT=theT;
		for (int i=0;i<vec_S.size();i++)
		{
			double allbijiao=0;
			for (int j=0;j<vec_S.size();j++)
			{
				if (i!=j)
				{
					allbijiao=allbijiao+LaMeans_dis(vec_thePOT[vec_S[i]],vec_thePOT[vec_S[j]]);
				}
			}
			if (allbijiao<=tempS)
			{
				tempS=allbijiao;
				TtheS=i;
			}
		}
		////////////////////////更新vec_S,vec_T
		vec_S.clear();vec_T.clear();
		for (int i=0;i<vec_thePOT.size();i++)
		{
			double biA,biB;
			biA=biB=0;
			biA=LaMeans_dis(vec_thePOT[TtheS],vec_thePOT[i]);
			biB=LaMeans_dis(vec_thePOT[TtheT],vec_thePOT[i]);
			if (biA>=biB)
			{
				vec_S.push_back(i);
			}
			else
			{
				vec_T.push_back(i);
			}
		}
		///////////////////////////////////T类选中心点
		for (int i=0;i<vec_T.size();i++)
		{
			double allbijiao=0;
			for (int j=0;j<vec_T.size();j++)
			{
				if (i!=j)
				{
					allbijiao=allbijiao+LaMeans_dis(vec_thePOT[vec_T[i]],vec_thePOT[vec_T[j]]);
				}
			}
			if (allbijiao<=tempT)
			{
				tempT=allbijiao;
				TtheT=i;
			}
		}
		//////////////////////////////
		if (theS==TtheS&&theT==TtheT)
		{
			break;
		}
		else
		{
			theS=TtheS;
			theT=TtheT;
			jishu++;
		}
		//////////////
	}
	///////////////////////////////////////得到边界点
	vec_PFACETTRI vec_facs,vec_fact;
	for (int i=0;i<facelist.size();i++)
	{
		facelist[i]->bStatus=0;
	}
	for (int i=0;i<vec_S.size();i++)
	{
		facelist[vec_S[i]]->bStatus=1;
		vec_facs.push_back(facelist[vec_S[i]]);
	}
	for (int i=0;i<vec_T.size();i++)
	{
		vec_fact.push_back(facelist[vec_T[i]]);
		if (facelist[vec_T[i]]->pHEdge->pHEdgePair->pFacetAdj->bStatus==1)
		{
			facelist[vec_T[i]]->pHEdge->pVertEnd->bused=1;
			facelist[vec_T[i]]->pHEdge->pHEdgePre->pVertEnd->bused=1;
		}
		if (facelist[vec_T[i]]->pHEdge->pHEdgePre->pHEdgePair->pFacetAdj->bStatus==1)
		{
			facelist[vec_T[i]]->pHEdge->pHEdgePre->pVertEnd->bused=1;
			facelist[vec_T[i]]->pHEdge->pHEdgePre->pHEdgePre->pVertEnd->bused=1;
		}
		if (facelist[vec_T[i]]->pHEdge->pHEdgeNext->pHEdgePair->pFacetAdj->bStatus==1)
		{
			facelist[vec_T[i]]->pHEdge->pHEdgeNext->pVertEnd->bused=1;
			facelist[vec_T[i]]->pHEdge->pHEdgeNext->pHEdgePre->pVertEnd->bused=1;
		}
	}
	Rbox.push_back(vec_facs);
	Rbox.push_back(vec_fact);
}
double Cboundary::LaMeans_dis(vector<double>& A,vector<double>& B)
{
	double alldis=0;
	for (int i=0;i<A.size();i++)
	{
		alldis=alldis+(A[i]-B[i])*(A[i]-B[i]);
	}
	return alldis;
}
void Cboundary::get_dis(PHEDGE A,double& theDis,double& theAng)
{
	////////////////////侧地距离
	POINT3D temA,temB,temC;
	temA = Kit.Center(A);
	temB = Kit.Center(A->pHEdgePair);
	temC.x=A->pVertEnd->x+A->pHEdgePair->pVertEnd->x;temC.y=A->pVertEnd->y+A->pHEdgePair->pVertEnd->y;
	temC.z=A->pVertEnd->z+A->pHEdgePair->pVertEnd->z;
	theDis = Kit.dis(temA, temC) + Kit.dis(temB, temC);
 //////////////////////////////////夹角
	VECTOR3D P1,P2,P3;
	PFACETTRI facA=(PFACETTRI)(A->pFacetAdj);
	PFACETTRI facB=(PFACETTRI)(A->pHEdgePair->pFacetAdj);
	P1=temA-temB;
	P2=*facA->m_PFacetNorm;
	P3=*facB->m_PFacetNorm;
	if ((P1|P2)<0)
	{
		theAng=0.1*(1-(P2|P3)/(P2.GetLength()*P3.GetLength()));
	}
	else
	{
		theAng=(1-(P2|P3)/(P2.GetLength()*P3.GetLength()));
	}
}
void Cboundary::get_avg(double& avgDis,double& avgAng,vec_PFACETTRI& vec_face)
{
	vec_PFACETTRI vecpFac;
	vec_PHEDGE vec_PH;
	int num=0;
	for (int i=0;i<vec_face.size();i++)
	{
		if (vec_face[i]->pHEdge->pHEdgePair->pFacetAdj->bStatus==1&&vec_face[i]->pHEdge->be_pair==0)
		{
		   double theDis=0;double theAng=0;
		   get_dis(vec_face[i]->pHEdge,theDis,theAng);
		   avgDis=avgDis+theDis;
		   avgAng=avgAng+theAng;
		   vec_face[i]->pHEdge->be_pair=1;
		   vec_face[i]->pHEdge->pHEdgePair->be_pair=1;
		   vec_PH.push_back(vec_face[i]->pHEdge);
		   vec_PH.push_back(vec_face[i]->pHEdge->pHEdgePair);
		   num++;
		}
		if (vec_face[i]->pHEdge->pHEdgePre->pHEdgePair->pFacetAdj->bStatus==1&&vec_face[i]->pHEdge->pHEdgePre->be_pair==0)
		{
			double theDis=0;double theAng=0;
			get_dis(vec_face[i]->pHEdge->pHEdgePre,theDis,theAng);
			avgDis=avgDis+theDis;
			avgAng=avgAng+theAng;
			vec_face[i]->pHEdge->pHEdgePre->be_pair=1;
			vec_face[i]->pHEdge->pHEdgePre->pHEdgePair->be_pair=1;
			vec_PH.push_back(vec_face[i]->pHEdge->pHEdgePre);
			vec_PH.push_back(vec_face[i]->pHEdge->pHEdgePre->pHEdgePair);
			num++;
		}
		if (vec_face[i]->pHEdge->pHEdgeNext->pHEdgePair->pFacetAdj->bStatus==1&&vec_face[i]->pHEdge->pHEdgeNext->be_pair==0)
		{
			double theDis=0;double theAng=0;
			get_dis(vec_face[i]->pHEdge->pHEdgeNext,theDis,theAng);
			avgDis=avgDis+theDis;
			avgAng=avgAng+theAng;
			vec_face[i]->pHEdge->pHEdgeNext->be_pair=1;
			vec_face[i]->pHEdge->pHEdgeNext->pHEdgePair->be_pair=1;
			vec_PH.push_back(vec_face[i]->pHEdge->pHEdgeNext);
			vec_PH.push_back(vec_face[i]->pHEdge->pHEdgeNext->pHEdgePair);
			num++;
		}
	}
	avgAng=num/avgAng;
	avgDis=num/avgDis;
	for (int i=0;i<vec_PH.size();i++)
	{
		vec_PH[i]->be_pair=0;
	}
}
/////////////////////SNAK光顺
void Cboundary::snke_parameter(vec_PVERT& vec_pot,vec_PFACETTRI& facelist)
{
	vec_PFACETTRI temp_facelist;
	for (int i=0;i<vec_pot.size()-1;i++)
	{
		vec_PHEDGE vecpH;
		Kit.FindOnePH(vec_pot[i], vecpH);
		for (int j=0;j<vecpH.size();j++)
		{
			if (vecpH[j]->pVertEnd==vec_pot[i+1])
			{
				vecpH[j]->bused=1;
			}
		}
	}
	int num=vec_pot.size();
	for (int i=0;i<num;i++)
	{
		vec_pot[i]->bStatus=1;
		vec_PVERT vecPver;
		Kit.FindTwoRing_new(vec_pot[i], vecPver);
		for (int j=0;j<vecPver.size();j++)
		{
			if (vecPver[j]->bStatus==0)
			{
				vecPver[j]->bStatus=1;
				vec_pot.push_back(vecPver[j]);
			}
		}
	}
	for (int j=0;j<facelist.size();j++)
	{
		facelist[j]->bStatus=0;
		if (facelist[j]->m_PVerts[0]->bStatus==1&&facelist[j]->m_PVerts[1]->bStatus==1&&facelist[j]->m_PVerts[2]->bStatus==1)
		{
			facelist[j]->bStatus=1;
			temp_facelist.push_back(facelist[j]);
		}
	}
	facelist=temp_facelist;

	vec_PVERT vecpVer;
	vector<T> tripletList;
	map<PVERT,int> map_ID;
	double numM=0;
	for (int i=0;i<vec_pot.size();i++)
	{
		map_ID.insert(pair<PVERT,int>(vec_pot[i],i));
	}
	for (int i=0;i<vec_pot.size();i++)
	{
		Kit.FindOneRing(vec_pot[i], vecpVer);
		POINT3D p0;
		p0=*vec_pot[i];
		vec_PVERT temp_vecpVer;
		for (int j=0;j<vecpVer.size();j++)
		{
			if (vecpVer[j]->bStatus==1)
			{
				temp_vecpVer.push_back(vecpVer[j]);
			}
		}
		vecpVer=temp_vecpVer;
		for (int j=0;j<vecpVer.size();j++)
		{
			VECTOR3D v1,v2;POINT3D p1,p2; 
			p1=*vecpVer[j];p2=*vecpVer[(j+1)%vecpVer.size()];
			v1=p1-p0;v2=p2-p0;
			double tempAng=(v1|v2)/(v1.GetLength()*v2.GetLength());
			double Ang=acos(tempAng);
			double r012=v2.GetLength()/v1.GetLength();
			if(cos(Ang)!=1)
			tripletList.push_back(T(2*j+numM,2*i,r012*cos(Ang)-1));
			if(sin(Ang)!=0)
			tripletList.push_back(T(2*j+numM,2*i+1,-r012*sin(Ang)));
			if(sin(Ang)!=0)
			tripletList.push_back(T(2*j+1+numM,2*i,r012*sin(Ang)));
			if(cos(Ang)!=1)
			tripletList.push_back(T(2*j+1+numM,2*i+1,r012*cos(Ang)-1));

			if(cos(Ang)!=0)
			tripletList.push_back(T(2*j+numM,2*map_ID[vecpVer[j]],-r012*cos(Ang)));
			if(sin(Ang)!=0)
			tripletList.push_back(T(2*j+numM,2*map_ID[vecpVer[j]]+1,r012*sin(Ang)));
			if(sin(Ang)!=0)
			tripletList.push_back(T(2*j+numM+1,2*map_ID[vecpVer[j]],-r012*sin(Ang)));
			if(cos(Ang)!=0)
			tripletList.push_back(T(2*j+numM+1,2*map_ID[vecpVer[j]]+1,-r012*cos(Ang)));

			tripletList.push_back(T(2*j+numM,2*map_ID[vecpVer[(j+1)%vecpVer.size()]],1));
			tripletList.push_back(T(2*j+numM+1,2*map_ID[vecpVer[(j+1)%vecpVer.size()]]+1,1));
		}
		numM=2*vecpVer.size()+numM;
	}
		tripletList.push_back(T(numM,0,1));
		tripletList.push_back(T(numM+1,1,1));

		tripletList.push_back(T(numM+2,2,1));
		tripletList.push_back(T(numM+3,3,1));

	SparseMatrix<double> A(numM+4,2*vec_pot.size()),AT(2*vec_pot.size(),numM+4),LA(2*vec_pot.size(),2*vec_pot.size());
	A.setFromTriplets(tripletList.begin(),tripletList.end());
	////////////右边
	VectorXd b(numM+4),Lb(2*vec_pot.size());
	for (int i=0;i<numM;i++)
	{
		b(i)=0;
	}
    b(numM)=1;b(1+numM)=1;
	b(2+numM)=10;b(3+numM)=10;
	//求解
	AT=A.transpose();
	LA=AT*A;
	Lb=AT*b;
	SimplicialCholesky<SpMat> chol(LA);
   VectorXd Z=chol.solve(Lb);
   /////////////////用于显示
	for (int i=0;i<vec_pot.size();i++)
	{
		vec_pot[i]->x=Z(2*i);
		vec_pot[i]->y=Z(2*i+1);
		vec_pot[i]->z=0;
		vec_pot[i]->bused=1;
	}
	//////////////////////////
	/*vector<POINT2D> vec_paLine,vec_paAll;
	for (int i=0;i<num;i++)
	{
		POINT2D temp;
		temp.x=Z(2*i);
		temp.y=Z(2*i+1);
		vec_paLine.push_back(temp);
	}
	for (int i=0;i<vec_pot.size();i++)
	{
		POINT2D temp;
		temp.x=Z(2*i);
		temp.y=Z(2*i+1);
		vec_paAll.push_back(temp);
	}*/
}
//////////////////////SNAK收缩
void Cboundary::snke_contract(vec_PVERT& Bloop,vec_PVERT& m_vecPVert)
{
	map<PVERT,double> Dist;
	for (int i=0;i<m_vecPVert.size();i++)
	{
		Dist.insert(pair<PVERT,double>(m_vecPVert[i],999));
	}
	vector<PVERT>::iterator ite;
	for (int i=0;i<Bloop.size();i++)
	{
		Bloop[i]->bused=0;
	}
	int count=0;
	while(1)
	{
		for (int i=1;i<Bloop.size()-1;i++)
		{
			snake_move(Bloop[i],Bloop[i-1],Bloop[i+1]);
		}
		count++;
		if (count==15)
		{
			break;
		}
	}
	for (int i=1;i<Bloop.size();i++)
	{
		if (Bloop[i]==Bloop[i-1])
		{
			ite=Bloop.begin()+i;
			Bloop.erase(ite);
			i--;
		}
	}
	int numk=Bloop.size();
	for (int i=0;i<numk-1;i++)
	{
		Bloop[i]->bused=1;
		//snake_link(Bloop,Bloop[i],Bloop[(i+1)%numk],m_vecPVert,Dist);
	}
	Bloop[0]->bused=1;
}
void Cboundary::snake_move(PVERT& Apot,PVERT& Ppot,PVERT& Npot)
{
	vec_PVERT vecpVer;
	Kit.FindOneRing(Apot,vecpVer);
	vecpVer.push_back(Apot);
	PVERT potK;
	double bijiao=10000;
	for (int i=0;i<vecpVer.size();i++)
	{
		double temp=0;
		temp=snake_exter(Apot)+snake_inside(vecpVer[i],Ppot,Npot);
		if (temp<bijiao)
		{
			bijiao=temp;
			potK=vecpVer[i];
		}
	}
	Apot=potK;
	
}
double Cboundary::snake_inside(PVERT& Apot,PVERT& Ppot,PVERT& Npot)
{
	double A,B;
	VECTOR3D v1,v2,v3;
	v1.dx=Apot->x-Ppot->x;v1.dy=Apot->y-Ppot->y;v1.dz=Apot->z-Ppot->z;
	v2.dx=Apot->x-Npot->x;v1.dy=Apot->y-Npot->y;v1.dz=Apot->z-Npot->z;
	A=v1.GetLength();
	v1.Normalize();v2.Normalize();
	v3+=v1+v2;
	B=v3.GetLength();
	return 0.5*A+0.5*B;
}
double Cboundary::snake_exter(PVERT& Apot)
{
	vec_PVERT vecpVer;
	Kit.FindOneRing(Apot, vecpVer);
	//double vi=Ccalcubase::calcuNoV(Apot);
	//Ccalcubase::FindTwoRing_new(Apot,vecpVer);
	double vi = Kit.CalcuPOINTV(Apot);
	double ext=0;
	double bijiao=-100;
	for (int i=0;i<vecpVer.size();i++)
	{
		double temp=0;
		//Ccalcubase::calcuNoV_new(vecpVer[i]);
		double vi = Kit.CalcuPOINTV(Apot);
		if (temp>bijiao)
		{
			bijiao=temp;
		}
	}
	ext=(vi>bijiao)?(-vi):(100);
	return ext;
}
BOOL Cboundary::snake_link(vector<PVERT>& m_Pst,PVERT& ST,PVERT& EN,vector<PVERT>& m_vecPVert)
{
	m_Pst.clear();
	if (ST==EN)
	{
		return false;
	}
	map<PVERT,double> Dist;
	map<PVERT,PVERT> Path;
	queue<PVERT> Q; 
	vector<PVERT> Pnear;
	vec_PVERT vecpVer;
	Kit.FindOneRing(ST, vecpVer);
	for (int i=0;i<vecpVer.size();i++)
	{
		if (vecpVer[i]==EN)
		{
			return false;
		}
	}
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bStatus=0;
		Dist.insert(pair<PVERT,double>(m_vecPVert[i],999));
	}
	Dist[ST]=0; 
	Q.push(ST);
	Path[ST]=ST->pHEdgeOut->pVertEnd;
	ST->bStatus=1;
	while(!Q.empty())
	{
		Kit.FindOneRing(Q.front(), vecpVer);
		for (int i=0;i<vecpVer.size();i++)
		{
			Pnear.push_back(vecpVer[i]);
			double k = Kit.dis(*Q.front(), *vecpVer[i]);//
			if ((Dist[Q.front()]+k)<Dist[vecpVer[i]])
			{
				Path[vecpVer[i]]=Q.front();
				Dist[vecpVer[i]]=Dist[Q.front()]+k;
			}
		}
		while(!Pnear.empty())
		{
			if (Pnear.size()>1)
			{
				for (int i=0;i<Pnear.size()-1;i++)
				{
					if (Dist[Pnear[i]]<Dist[Pnear[i+1]])
					{
						swap(Pnear[i],Pnear[i+1]);
					}
				}
				if (Pnear.back()->bStatus==0)
				{
					Pnear.back()->bStatus=1;
					Q.push(Pnear.back());
				}
				Pnear.pop_back();
			}
			if (Pnear.size()==1)
			{
				if (Pnear.back()->bStatus==0)
				{
					Pnear.back()->bStatus=1;
					Q.push(Pnear.back());
				}
				Pnear.pop_back();
			}
		}
		Q.pop();
		if (Q.size()>0)
		{
			if (Q.front()==EN)
			{
				break;
			}
		}
	}
	PVERT temp;
	temp=EN;
	while (temp!=ST)
	{
		m_Pst.push_back(temp);
		temp=Path[temp];
	}
	m_Pst.erase(m_Pst.begin()+0);
	reverse(m_Pst.begin(),m_Pst.end());
	return true;
}
void Cboundary::snake_smooth(vector<PVERT>& Bloop)//去锯齿光顺
{
	vec_PHEDGE vecpH,vecpH2;
	while(1)
	{
		int jishu=0;
	for (int i=0;i<Bloop.size()-2;i++)
	{
		Kit.FindOnePH(Bloop[i], vecpH);
	  for (int j=0;j<vecpH.size();j++)
	  {
		  if (vecpH[j]->pVertEnd==Bloop[i+2])
		  {
			 Bloop[i+1]->bused=0;
			 Bloop[i+1]->bStatus=0;
			 Bloop.erase(Bloop.begin()+i+1);
			 i--;
			 jishu++;
		 }

	  }
	}
	if (jishu==0)
	{
		break;
	}
	}
}
void Cboundary::snake_smooth_close(vector<PVERT>& Bloop)
{
	vec_PHEDGE vecpH,vecpH2;
	vec_PVERT::iterator er1,er2;
	while(1)
	{
		int jishu=0;
		for (int i=0;i<Bloop.size();i++)
		{
			if (Bloop[i]==Bloop[(i+1)%Bloop.size()])
			{
				Bloop.erase(Bloop.begin()+i);
				i--;
				jishu++;
			}
		}
		for (int i=0;i<Bloop.size();i++)
		{
			Kit.FindOnePH(Bloop[i], vecpH);
			for (int j=0;j<vecpH.size();j++)
			{
				if (vecpH[j]->pVertEnd==Bloop[(i+2)%Bloop.size()])
				{
					Bloop[(i+1)%Bloop.size()]->bused=0;
					Bloop[(i+1)%Bloop.size()]->bStatus=0;
					Bloop.erase(Bloop.begin()+(i+1)%Bloop.size());
					jishu++;
				}
			}
		}
		for (int i=0;i<Bloop.size();i++)
		{
			if (Bloop[i]==Bloop[(i+2)%Bloop.size()])
			{
				Bloop[(i+1)%Bloop.size()]->bused=0;
				Bloop[(i+1)%Bloop.size()]->bStatus=0;
				Bloop.erase(Bloop.begin() + ((i+1)%Bloop.size()));
				jishu++;
			}
		}
		if (jishu==0)
		{
			break;
		}
	}
}
void Cboundary::snaking(vector<PVERT>& Bloop,vector<PVERT>& m_vecPVert)
{
	for (int i=0;i<Bloop.size();i++)
	{
		if (Bloop[i]==Bloop[(i+1)%Bloop.size()])
		{
			Bloop.erase(Bloop.begin()+i);
			i--;
		}
	}
	vector<PVERT>::iterator ite;
	for (int i=0;i<Bloop.size();i++)
	{
		Bloop[i]->bused=0;
	}
	int count=0;
	while(1)
	{
		vector<PVERT> m_Pst;
		for (int i=0;i<Bloop.size();i++)
		{
			PVERT temP=NULL;
			if (i==0)
			{
				temP=Bloop[0];
				snake_move(temP,Bloop.back(),Bloop[1]);
				m_Pst.push_back(temP);
			}
			else
			{
				temP=Bloop[i];
				snake_move(temP,Bloop[i-1],Bloop[(i+1)%Bloop.size()]);
				m_Pst.push_back(temP);
			}
		}
		while(1)
		{
			int jishu=0;
			for (int i=1;i<m_Pst.size();i++)
			{
				if (m_Pst[i]==m_Pst[i-1])
				{
					ite=m_Pst.begin()+i;
					m_Pst.erase(ite);
					i--;
					jishu++;
				}
			}
			if (m_Pst.back()==m_Pst[0])
			{
				m_Pst.pop_back();
				jishu++;
			}
			if (jishu==0)
			{
				break;
			}
		}
		//Bloop=m_Pst;
		Bloop.clear();
		
		for (int i=0;i<m_Pst.size();i++)
		{
		    vector<PVERT> m_Plist;
			snake_link(m_Plist,m_Pst[i],m_Pst[(i+1)%m_Pst.size()],m_vecPVert);
			///////////////////////////
			Bloop.push_back(m_Pst[i]);
			for (int j=0;j<m_Plist.size();j++)
			{
				Bloop.push_back(m_Plist[j]);
			}
		}
		snake_smooth_close(Bloop);
		count++;
		if (count==20)
		{
			break;
		}
	}
	////////////
	for (int i=0;i<m_vecPVert.size();i++)
	{
		m_vecPVert[i]->bStatus=0;
	}
	//////////////////////画线
	vec_PHEDGE vecpH;
	for (int i=0;i<Bloop.size();i++)
	{
		Bloop[i]->bused=1;
		Kit.FindOnePH(Bloop[i], vecpH);
		for (int num=0;num<vecpH.size();num++)
		{
			if (vecpH[num]->pVertEnd==Bloop[(i+1)%(Bloop.size())])
			{
				vecpH[num]->bused=1;
				vecpH[num]->pHEdgePair->bused=1;
			}
		}
	}
}
void Cboundary::showVetex(drawTool *pDC, vec_PVERT	m_vecPVert)
{
	COLORREF old, old1;
	pDC->GetColor(old);
	pDC->GetMaterialColor(old1);
	pDC->SetMaterialColor(RGB(0, 255, 0));
	for (int i = 0; i < m_vecPVert.size(); i++)
	{
		POINT3D temp;
		if (m_vecPVert[i]->bused)
		{
			temp.x = m_vecPVert[i]->x;
			temp.y = m_vecPVert[i]->y;
			temp.z = m_vecPVert[i]->z;
			pDC->DrawSphere(temp, 0.5);
		}
	}
	pDC->SetColor(old);
	pDC->SetMaterialColor(old1);
}
