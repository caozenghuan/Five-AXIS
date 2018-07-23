#include "Optisequence.h"


COptisequence::COptisequence()
{
}


COptisequence::~COptisequence()
{
}
vector<SUNIT>  COptisequence::MOPSO(vector<MyBlock> Rbox, vector<VECTOR3D> &All_VecDir, DlgInterveResult &InterveResult, QProgressBar *MyProgressBar)
{
	///////////保存面片法矢/////////////////////////
	vector<vec_PVECTOR3D> m_vecAllFNorm;
	for (int i = 0; i < Rbox.size(); i++)
	{
		vec_PVECTOR3D m_vecFacetNorm;
		for (int j = 0; j < Rbox[i].OneBlock.size(); j++)
		{
			m_vecFacetNorm.push_back(Rbox[i].OneBlock[j]->m_PFacetNorm);
		}
		m_vecAllFNorm.push_back(m_vecFacetNorm);
	}
	vector<vector<int>> AllSQ;;
	vector<SUNIT> LastUnit;
	int SZ = 120; int SZT = 1;
	for (int i = 0; i < Rbox.size();i++)
	{
		SZT *= (i+1);
	}
	(SZ < SZT) ? SZ = 120 : SZ = SZT;
	//vector<int> aTemp;
	//aTemp.push_back(0); aTemp.push_back(3); aTemp.push_back(6);
	//aTemp.push_back(1); aTemp.push_back(8); aTemp.push_back(2);
	//aTemp.push_back(7); aTemp.push_back(5); aTemp.push_back(4);
	//RC1(aTemp, Rbox);
	//RC2(aTemp, Rbox);
	//初始化种群////
	AllSQ = RandomDir(SZ, Rbox, All_VecDir, InterveResult, MyProgressBar);
	int gene = 0;
	vector<double> AllSum;

	srand((unsigned)time(NULL));
	                 //写一下依据合理性约束剔除初始种群中不对的点
	////初始化速度///
	double v1max = Rbox.size() / 10.0;
	vector<vector<int>> vec_pBest;/////个体最优
	vector<int>    gBest;////全局最优
	vector<vector<double>>  vec_Velocity;//
	vector<SUNIT>  ElitePopulation;////精英种群
	for (int i = 0; i < AllSQ.size(); i++)
	{
		vector<double>  v(Rbox.size(),0);
		for (int j = 0; j < Rbox.size();j++)
		{
			v[j] = (double)rand() / RAND_MAX*v1max;
		}
		vec_Velocity.push_back(v);
		vec_pBest.push_back(AllSQ[i]);
	}
	
	vector<SUNIT> Bestnums = non_inferior(AllSQ, All_VecDir, Rbox);
	//do
	//{
	//	MyProgressBar->setValue(gene);
	//	//myCtrlPos=gene/2;
	//	//myCtrl.SetPos(myCtrlPos);
	//	gBest = MOPSO_Select_gbest(ALLDir, Bestnums, ElitePopulation, m_vecFacetNorm, vec_Area);////包含精英种群的维护更新和全局最优的选取
	//	ALLDir = MOPSO_upDate(gBest, ALLDir, vec_Velocity, v1max, v2max, vec_pBest);
	//	MOPSO_UpdatePbest(m_vecFacetNorm, ALLDir, vec_Area, vec_pBest);
	//	Bestnums = non_inferior(ALLDir, m_vecFacetNorm, vec_Area);
	//	gene++;
	//} while (gene < 200);
	////LastUnit=non_dominatedSet(ElitePopulation,pSTL->m_vecFacetNorm,vec_Area)[0];
	//MyProgressBar->setValue(0);
	return Bestnums;
	//return  ElitePopulation;
}
void COptisequence::UpdateBox(vector<MyBlock> Rbox)
{
	double x0, y0, z0, x1, y1, z1;
	for (int i = 0; i < Rbox.size();i++)
	{
		x0 = x1 = Rbox[i].OneBlock[1]->m_PVerts[0]->x;
		y0 = y1 = Rbox[i].OneBlock[1]->m_PVerts[0]->y;
		z0 = z1 = Rbox[i].OneBlock[1]->m_PVerts[0]->z;

		FACETTRI* ptri;
		for (int j = 0; j < Rbox[i].OneBlock.size(); j++)
		{
			ptri = Rbox[i].OneBlock[j];
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
		Rbox[i].box_p0.x = x0;
		Rbox[i].box_p0.y = y0;
		Rbox[i].box_p0.z = z0;
		Rbox[i].box_p1.x = x1;
		Rbox[i].box_p1.y = y1;
		Rbox[i].box_p1.z = z1;
	}
}
vector<vector<int>> COptisequence::RandomDir(int SZ, vector<MyBlock> Rbox, vector<VECTOR3D> &All_VecDir, DlgInterveResult &InterveResult, QProgressBar *MyProgressBar)
{
	
	
	//////////////测试////////////////
	//vector<QString> vec_strTemp;
	//vector<int> aTemptt(10, 0);
	//QString str, strint;
	//QString str1;
	//QString str2;
	//str1 = QString::number(0, 10);
	//for (int r = 1; r < 10; r++)
	//{
	//	str1 = str1 + QString::fromLocal8Bit("-");
	//	str1 = str1 + QString::number(aTemptt[r], 10);
	//}
	//str2 = QString::number(3, 10); str2 = str2 + QString::fromLocal8Bit("与"); str2 = str2 + QString::number(8, 10);

	//str = QString::fromLocal8Bit("当前子块顺序为：") + str1;
	//strint = QString::fromLocal8Bit("干涉子块为：") + str2;
	//vec_strTemp.push_back(str); vec_strTemp.push_back(strint);
	//InterveResult.m_vec_str = vec_strTemp;
	//InterveResult.getStr();

	//if (InterveResult.exec() == QDialog::Accepted)
	//{
	//	int ceshi = 0;
	//}
	//////////////测试终止////////////////
	MyBlock CurrentBlock;
	POINT3D Pcenter;//底层面中心点
	vector<vec_POINT3D> vec_Head;
	vector<vec_POINT3D> vec_PlanBoard;
	vector<SInterB> AllModelDate;
	SInterB CurrentJoinModel;

	CurrentBlock = Rbox[0];
	Pcenter = CalCenterPoint(CurrentBlock,All_VecDir[0]);
	Creatboarddate(vec_PlanBoard, Pcenter); 
	CurrentJoinModel.ModelDate = vec_PlanBoard; CurrentJoinModel.BoxDate = vec_PlanBoard;
	AllModelDate.push_back(CurrentJoinModel);//平台板数据置入容器

	UpdateBox(Rbox);//更新所有块的包围盒信息
	JoinCurrentBlock(AllModelDate, CurrentBlock);//默认0为主块
	int Ntime = 0;
	int Nblock = Rbox.size();
	vector<vector<int>> VEC_SE;
	srand((unsigned)time(NULL));//播种
	vector<QString> vec_str;
	do 
	{
		MyProgressBar->setValue(Ntime);
		vector<int> aTemp(Nblock, 0);
		int n = 1;
		while (n < Nblock)
		{
			int m = rand() % Nblock ;
			bool flag = 0;
			for (int j = 0; j < n; j++)
			{
				if (aTemp[j] == m)
				{
					flag = 1;//如果之前保存的数中出现了和m相同的数，就把flag标记为1并跳出循环，表示需要重新生成随机数m  
					break;
				}
			}
			if (flag!=1)
			{
				int ntime = 0;
				vector<int> LastblockPair;
				LastblockPair = Rbox[aTemp[n - 1]].vec_PairBlock;
				for (int j = 0; j < LastblockPair.size(); j++)//查找当前块是不是上一块的邻接块
				{
					if (m != LastblockPair[j])  ntime++;
					if (ntime == LastblockPair.size())
					{
						flag = 1;//找到顶端块了，再重新从上上个块找

						//查找上一块的邻接块是否均找完
						ntime = 0;
						for (int j = 0; j < LastblockPair.size(); j++)
						{
							for (int k = 0; k < n; k++)
							{
								if (LastblockPair[j] == aTemp[k])
								{
									ntime++;
								}
							}
						}
						if (ntime == LastblockPair.size())  flag = 0;
					}
				}
				if (LastblockPair.size() == 1 )
				{
					///////////不能从一顶端块走到另一顶端块////////////////
					if (Rbox[LastblockPair[0]].vec_PairBlock.size() == 2 && Rbox[m].vec_PairBlock.size() == 1)
					{
						flag = 1;
					}
					///////不能从一顶端块走到另一顶端块邻接中间块//////////////
					bool FindTheblock = true;
					for (int j = 0; j < n; j++)
					{
						for (int k = 0; k < Rbox[aTemp[j]].vec_PairBlock.size(); k++)
						{
							if (m == Rbox[aTemp[j]].vec_PairBlock[k])
							{
								FindTheblock = false;
								break;
							}
						}
						if (!FindTheblock) break;
					}
					if (FindTheblock)  flag = 1;
				}
			}
			if (flag == 0)
			{
				CurrentBlock = Rbox[m];
				//依据当前块信息更新喷头数据
				Pcenter = CalCenterPoint(CurrentBlock,All_VecDir[m]);
				vec_Head.clear();
				CreatHeadboxdate(vec_Head, Pcenter,All_VecDir[m]);

				vec_str.clear(); InterveResult.m_vec_str.clear();
				int TheInterBlock=0;
				if (GetIntereResult(AllModelDate, CurrentBlock, vec_Head, Pcenter, All_VecDir[m],TheInterBlock))//有干涉情况
				{
					QString str, strint;
					QString str1;
					QString str2;
					str1 =QString::number(0, 10);
					for (int r = 1; r < n;r++)
					{
						str1 = str1 + QString::fromLocal8Bit("-");
						str1 = str1 + QString::number(aTemp[r], 10);
					}
					str2 = QString::number(m, 10); str2 = str2 + QString::fromLocal8Bit("与"); str2 = str2 + QString::number(TheInterBlock, 10);

					str = QString::fromLocal8Bit("当前子块顺序为：") + str1;
					strint = QString::fromLocal8Bit("干涉子块为：") + str2;
					vec_str.push_back(str); vec_str.push_back(strint);
					InterveResult.m_vec_str = vec_str;
					InterveResult.getStr();

					if (InterveResult.exec() == QDialog::Accepted)
					{
						int ceshi = 0;
					}

				}
				else
				{
					//继续产生下一块加工块
					aTemp[n] = m;//如果flag是0，表示前n个数中没有和m相同的数，因此可以把第n+1个元素赋值为m  
					n++;
				}

			}
		}
		AllModelDate.clear();
		if (RC1(aTemp, Rbox))
		{
			VEC_SE.push_back(aTemp);
			Ntime++;
			/*if (RC2(aTemp, Rbox))
			{
			VEC_SE.push_back(aTemp);
			Ntime++;
			}*/
		}
		
	} while (Ntime!=6);
	MyProgressBar->setValue(0);
	return VEC_SE;
}

vector<SUNIT> COptisequence::non_inferior(vector<vector<int>> &MyDirS, vector<VECTOR3D> &All_VecDir, vector<MyBlock> Rbox)
{
	///////////加载目标函数
	vector<vector<SUNIT>>  my_Dominated;
	vector<pSUNIT> vec_pUnit;

	int SZ = MyDirS.size();
	VECTOR3D DirZ(0,0,1);
	for (int i = 0; i < SZ; i++)
	{
		pSUNIT myUnit = new SUNIT;
		myUnit->Sequence = MyDirS[i];
		for (int j = 0; j < MyDirS[i].size();j++)
		{
			int Nblcok = MyDirS[i][j];
			myUnit->f1Sum += TheLestAngle(MyDirS[i], All_VecDir);
		}
		myUnit->f2Sum = TheLestDistance(MyDirS[i], All_VecDir, Rbox);
		vec_pUnit.push_back(myUnit);
	}
	////////非支配集合/////
	////////非支配集合/////
	for (int i = 0; i < SZ - 1; i++)
	{
		for (int j = i + 1; j < SZ; j++)
		{
			if (vec_pUnit[i]->f1Sum < vec_pUnit[j]->f1Sum)
			{
				if (vec_pUnit[i]->f2Sum <= vec_pUnit[j]->f2Sum)
				{
					vec_pUnit[i]->S.push_back(vec_pUnit[j]);
					vec_pUnit[j]->n++;
				}
			}
			else
			{
				if (vec_pUnit[i]->f1Sum == vec_pUnit[j]->f1Sum)
				{
					if (vec_pUnit[i]->f2Sum < vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[i]->S.push_back(vec_pUnit[j]);
						vec_pUnit[j]->n++;
					}
					else
					{
						if (vec_pUnit[i]->f2Sum > vec_pUnit[j]->f2Sum)
						{
							vec_pUnit[j]->S.push_back(vec_pUnit[i]);
							vec_pUnit[i]->n++;
						}
					}
				}
				else
				{
					if (vec_pUnit[i]->f2Sum > vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[j]->S.push_back(vec_pUnit[i]);
						vec_pUnit[i]->n++;
					}
				}


			}
		}
	}
	set<pSUNIT>  set_Unit;/////用于查找重复
	pair<set<pSUNIT>::iterator, bool>  pairS;/////用于查找重复
	vector<pSUNIT> vec_oper = vec_pUnit;
	set_Unit.clear();
	int Level = 1;
	vector<pSUNIT> vec_temp;
	vector<SUNIT> vec_dominated;
	int vecSz = vec_oper.size();
	//vector<int>  vec_int;
	for (int i = 0; i < vecSz; i++)
	{
		if (vec_oper[i]->n == 0)
		{
			/////赋支配等级
			vec_oper[i]->level = Level;
			vec_dominated.push_back(*vec_oper[i]);
			//vec_int.push_back(i);
		}
	}
	//Distance(vec_dominated);/////计算拥挤距离
	//my_Dominated.push_back(vec_dominated);
	vec_oper = vec_temp;
	Level++;
	/////析构释放内存/////
	for (int i = 0; i < SZ; i++)
	{
		delete vec_pUnit[i];
	}
	return vec_dominated;
}

//void COptisequence::IfAcuteAngle(VECTOR3D &Dir, VECTOR3D Diz)
//{
//	double angel = Dir | Diz;
//	//angel = abs(abs(angel) - 1);
//	if (angel < 0)
//	{
//		Dir.dx = -Dir.dx;
//		Dir.dy = -Dir.dy;
//		Dir.dz = -Dir.dz;
//	}
//}
double COptisequence::TheLestAngle(vector<int> Sqence, vector<VECTOR3D> All_VecDir)
{
	double Allangle=0;//整个过程摆动角度之和;
	double adle = 0;
	for (int i = 0; i < Sqence.size() - 1; i++)
	{
		int n0, n1;
		n0 = Sqence[i];
		n1 = Sqence[i + 1];
		adle = acos(All_VecDir[n0] | All_VecDir[n1]);
		Allangle+= acos(All_VecDir[n0] | All_VecDir[n1]);
	}
	return Allangle;
}
double COptisequence::TheLestDistance(vector<int> Sqence, vector<VECTOR3D> All_VecDir, vector<MyBlock> Rbox)
{
	double _Dis = 0;
	vector<POINT3D> vec_box;
	for (int i = 0; i < Rbox.size(); i++)
	{
		POINT3D p0;
		BOX3D ONE;
		UpdateBlockBox(Rbox[i].OneBlock, ONE);
		p0.x = (ONE.x0 + ONE.x1)*0.5;
		p0.y = (ONE.y0 + ONE.y1)*0.5;
		p0.z = (ONE.z0 + ONE.z1)*0.5;
		vec_box.push_back(p0);
	}
	int n0, n1;
	for (int i = 0; i < Sqence.size()-1;i++)
	{
		n0 = Sqence[i];
		n1 = Sqence[i+1];
		_Dis+=Pkit.dis(vec_box[n0], vec_box[n1]);
	}
	return _Dis;
}
int COptisequence::RC1(vector<int> Sqence, vector<MyBlock> Rbox)
{
	for (int i = 0; i < Sqence.size()-1;i++)
	{
		int n0, n1;
		n0 = Sqence[i];
		n1 = Sqence[i + 1];
		MyBlock B1 = Rbox[n0];
		MyBlock B2 = Rbox[n1];
		bool Find = false;
		for (int j = 0; j < B1.vec_PairBlock.size(); j++)
		{
			if (n1 == B1.vec_PairBlock[j])
			{
				Rbox[n0].Status = true;
				Find = true;
				break;
			}
		}
		if (!Find)
		{
			for (int j = 0; j < B1.vec_PairBlock.size(); j++)
			{
				if (Rbox[B1.vec_PairBlock[j]].Status == false)
				{
					if (B1.vec_PairBlock[j] != n1)
					{
						return 0;
					}
				}
			}
		}
	}
	return 1;
}
int COptisequence::RC2(vector<int> Sqence, vector<MyBlock> Rbox)
{
	vector<double> vec_box;
	for (int i = 0; i < Rbox.size();i++)
	{
		BOX3D ONE;
		UpdateBlockBox(Rbox[i].OneBlock, ONE);
		vec_box.push_back(ONE.z0);
	}
	//按照物理原则，第一块必须是最低点所在的块。
	for (int i = 0; i < vec_box.size();i++)
	{
		if (vec_box[Sqence[0]]>vec_box[i])
		{
			return 0;
		}
	}
	int n0, n1;
	for (int i = 0; i < Sqence.size()-1;i++)
	{
		n0 = Sqence[i];
		n1 = Sqence[i + 1];
		if (vec_box[n0]<vec_box[n1])
		{
			for (int j = 0; j < Rbox[n1].vec_PairBlock.size(); j++)
			{
				int nct = Rbox[n1].vec_PairBlock[j];
				if (!Rbox[nct].Status&&vec_box[nct] < vec_box[n0])
				{
					return 0;
				}
			}
			Rbox[n0].Status = true;
		}
		else
		{
			for (int j = 0; j < Rbox[n0].vec_PairBlock.size(); j++)
			{
				if (Rbox[Rbox[n0].vec_PairBlock[j]].Status == false)
				{
					if (Rbox[n0].vec_PairBlock[j] != n1)
					{
						return 0;
					}
				}
			}
			for (int j = 0; j < Rbox[n1].vec_PairBlock.size(); j++)
			{
				if (vec_box[n1]>vec_box[Rbox[n1].vec_PairBlock[j]])
				{
					if (Rbox[Rbox[n1].vec_PairBlock[j]].Status == false)
					{
						return 0;
					}
				}
			}
		}
	}
	return 1;
}
void COptisequence::UpdateBlockBox(vector<PFACETTRI> pMeshTri, BOX3D &ONE)
{
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = pMeshTri[1]->m_PVerts[0]->x;
	y0 = y1 = pMeshTri[1]->m_PVerts[0]->y;
	z0 = z1 = pMeshTri[1]->m_PVerts[0]->z;

	FACETTRI* ptri;
	for (int i = 0; i < pMeshTri.size(); i++)
	{
		ptri = pMeshTri[i];
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
void COptisequence::SaveOptiResult(vector<MyBlock> &Rbox,vector<VECTOR3D> &All_VecDir,vector<int> Sequence)
{
	//复制块
	vector<MyBlock> RbTemp;
	vector<VECTOR3D> _VecDir;
	for (int i = 0; i < Sequence.size(); i++)
	{
		int nct = Sequence[i];
		Rbox[nct].vec_PairBlock.clear();
		RbTemp.push_back(Rbox[nct]);
		_VecDir.push_back(All_VecDir[nct]);
	}
	//保存块的邻接信息
	for (int i = 0; i < RbTemp.size();i++)
	{
		for (int j = 0; j < RbTemp[i].OneBlockPair.size();j++)
		{
			for (int k = 0; k < RbTemp.size();k++)
			{
				if (RbTemp[i].OneBlockPair[j].OneBlock.size() == RbTemp[k].OneBlock.size())
				{
					RbTemp[i].vec_PairBlock.push_back(k);
					break;
				}
			}
		}
	}
	All_VecDir.clear();
	All_VecDir = _VecDir;
	_VecDir.clear();
	Rbox.clear();
	Rbox = RbTemp;
	RbTemp.clear();
}
void COptisequence::Creatboarddate(vector<vec_POINT3D> &vec_face,POINT3D pcenter)
{
	VECTOR3D v_Normal; POINT3D OneNormal;
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	POINT3D p0, p1, p2, p3;
	p0 = POINT3D(pcenter.x - 100, pcenter.y - 100, pcenter.z); p1 = POINT3D(pcenter.x - 100, pcenter.y + 100, pcenter.z); p2 = POINT3D(pcenter.x + 100, pcenter.y - 100, pcenter.z); p3 = POINT3D(pcenter.x + 100, pcenter.x + 100, pcenter.z);
	vec_POINT3D pface;
	OneNormal = POINT3D(0, 0, 1);
	pface.push_back(OneNormal);
	if (KitTool::JudgeFaceDir(p0, p1, p2, DirZ))
	{
		pface.push_back(p0); pface.push_back(p1); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p0); pface.push_back(p2);
	}
	vec_face.push_back(pface); pface.clear();
	
	pface.push_back(OneNormal);
	if (KitTool::JudgeFaceDir(p1, p3, p2, DirZ))
	{
		pface.push_back(p1); pface.push_back(p3); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p1); pface.push_back(p2);
	}
	vec_face.push_back(pface); pface.clear();
}
POINT3D COptisequence::CalCenterPoint(MyBlock CurrnetBlock, VECTOR3D DirV)
{
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	//DirV.dx = All_VecDir[2].dx; DirV.dy = -All_VecDir[2].dy; DirV.dz = -All_VecDir[2].dz;
	vector<vec_POINT3D> vec_face;
	for (int i = 0; i < CurrnetBlock.OneBlock.size(); i++)
	{
		vec_POINT3D Oneface; POINT3D p00;
		for (int j = 0; j < 3; j++)
		{
			p00.x = CurrnetBlock.OneBlock[i]->m_PVerts[j]->x;
			p00.y = CurrnetBlock.OneBlock[i]->m_PVerts[j]->y;
			p00.z = CurrnetBlock.OneBlock[i]->m_PVerts[j]->z;
			Oneface.push_back(p00);
		}
		vec_face.push_back(Oneface);
	}
	RotateHeadBox(vec_face, DirV, DirZ);
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = vec_face[1][0].x;
	y0 = y1 = vec_face[1][0].y;
	z0 = z1 = vec_face[1][0].z;

	for (int i = 0; i < vec_face.size(); i++)
	{
		for (int n = 0; n < 3; n++)
		{
			if (vec_face[i][n].x < x0)
				x0 = vec_face[i][n].x;
			if (vec_face[i][n].x > x1)
				x1 = vec_face[i][n].x;
			if (vec_face[i][n].y < y0)
				y0 = vec_face[i][n].y;
			if (vec_face[i][n].y > y1)
				y1 = vec_face[i][n].y;
			if (vec_face[i][n].z < z0)
				z0 = vec_face[i][n].z;
			if (vec_face[i][n].z > z1)
				z1 = vec_face[i][n].z;
		}
	}
	POINT3D Cenpoint;
	Cenpoint.x = (x0 + x1) / 2; Cenpoint.y = (y0 + y1) / 2; Cenpoint.z = z0;
	return Cenpoint;
}
void COptisequence::CreatHeadboxdate(vector<vec_POINT3D> &vec_face, POINT3D Pcenter,VECTOR3D DirV)
{
	POINT3D p0, p1, p2, p3, p00, p01, p02, p03;
	POINT3D p000, p001, p002, p003;
	p000 = POINT3D(Pcenter.x - 2, Pcenter.y - 2, Pcenter.z); p001 = POINT3D(Pcenter.x - 2, Pcenter.y + 2, Pcenter.z);
	p002 = POINT3D(Pcenter.x + 2, Pcenter.y - 2, Pcenter.z); p003 = POINT3D(Pcenter.x + 2, Pcenter.y + 2, Pcenter.z);


	p0 = POINT3D(Pcenter.x - 10, Pcenter.y - 10, Pcenter.z + 8); p1 = POINT3D(Pcenter.x - 10, Pcenter.y + 10, Pcenter.z + 8);
	p2 = POINT3D(Pcenter.x + 10, Pcenter.y - 10, Pcenter.z + 8); p3 = POINT3D(Pcenter.x + 10, Pcenter.y + 10, Pcenter.z + 8);
	p00 = POINT3D(Pcenter.x - 10, Pcenter.y - 10, Pcenter.z + 78); p01 = POINT3D(Pcenter.x - 10, Pcenter.y + 10, Pcenter.z + 78);
	p02 = POINT3D(Pcenter.x + 10, Pcenter.y - 10, Pcenter.z + 78); p03 = POINT3D(Pcenter.x + 10, Pcenter.y + 10, Pcenter.z + 78);
	////////////////////上面的长方体/////////////////////////
	//下底面---略去放下边//
	vec_POINT3D pface; VECTOR3D v_Normal; POINT3D OneNormal;

	//上底面//
	VECTOR3D DirZ = VECTOR3D(0, 0, 1);
	DirZ.dx = 0; DirZ.dy = 0; DirZ.dz = 1;

	if (KitTool::JudgeFaceDir(p00, p01, p02, DirZ))
	{
		pface.push_back(p00); pface.push_back(p01); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p01); pface.push_back(p00); pface.push_back(p02);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p01, p03, p02, DirZ))
	{
		pface.push_back(p01); pface.push_back(p03); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p03); pface.push_back(p01); pface.push_back(p02);
	}
	vec_face.push_back(pface); pface.clear();
	//左侧面//
	DirZ.dx = 0; DirZ.dy = -1; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p00, p0, p2, DirZ))
	{
		pface.push_back(p00); pface.push_back(p0); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p0); pface.push_back(p00); pface.push_back(p2);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p00, p2, p02, DirZ))
	{
		pface.push_back(p00); pface.push_back(p2); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p2); pface.push_back(p00); pface.push_back(p02);
	}
	vec_face.push_back(pface); pface.clear();
	//右侧面//
	DirZ.dx = 0; DirZ.dy = 1; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p01, p1, p3, DirZ))
	{
		pface.push_back(p01); pface.push_back(p1); pface.push_back(p3);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p01); pface.push_back(p3);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p01, p3, p03, DirZ))
	{
		pface.push_back(p01); pface.push_back(p3); pface.push_back(p03);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p01); pface.push_back(p03);
	}
	vec_face.push_back(pface); pface.clear();
	//前侧面//
	DirZ.dx = 1; DirZ.dy = 0; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p02, p2, p3, DirZ))
	{
		pface.push_back(p02); pface.push_back(p2); pface.push_back(p3);
	}
	else
	{
		pface.push_back(p2); pface.push_back(p02); pface.push_back(p3);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p02, p3, p03, DirZ))
	{
		pface.push_back(p02); pface.push_back(p3); pface.push_back(p03);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p02); pface.push_back(p03);
	}
	vec_face.push_back(pface); pface.clear();
	//后侧面//
	DirZ.dx = -1; DirZ.dy = 0; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p00, p0, p1, DirZ))
	{
		pface.push_back(p00); pface.push_back(p0); pface.push_back(p1);
	}
	else
	{
		pface.push_back(p0); pface.push_back(p00); pface.push_back(p1);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p00, p1, p01, DirZ))
	{
		pface.push_back(p00); pface.push_back(p1); pface.push_back(p01);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p00); pface.push_back(p01);
	}
	vec_face.push_back(pface); pface.clear();
	////////////////////下面的四棱柱/////////////////////////
	//下底面//
	DirZ = VECTOR3D(0, 0, -1);

	if (KitTool::JudgeFaceDir(p000, p001, p002, DirZ))
	{
		pface.push_back(p000); pface.push_back(p001); pface.push_back(p002);
	}
	else
	{
		pface.push_back(p001); pface.push_back(p000); pface.push_back(p002);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p001, p003, p002, DirZ))
	{
		pface.push_back(p001); pface.push_back(p003); pface.push_back(p002);
	}
	else
	{
		pface.push_back(p003); pface.push_back(p001); pface.push_back(p002);
	}
	vec_face.push_back(pface); pface.clear();
	//前侧面//
	VECTOR3D R1 = p003 - p002; VECTOR3D R2 = p3 - p003;
	DirZ = R1*R2;
	if (KitTool::JudgeFaceDir(p002, p003, p2, DirZ))
	{
		pface.push_back(p002); pface.push_back(p003); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p003); pface.push_back(p002); pface.push_back(p2);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p003, p3, p2, DirZ))
	{
		pface.push_back(p003); pface.push_back(p3); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p3); pface.push_back(p2);
	}
	vec_face.push_back(pface); pface.clear();
	//后侧面//
	R1 = p000 - p001;  R2 = p0 - p000;
	DirZ = R1*R2;
	if (KitTool::JudgeFaceDir(p001, p000, p1, DirZ))
	{
		pface.push_back(p001); pface.push_back(p000); pface.push_back(p1);
	}
	else
	{
		pface.push_back(p000); pface.push_back(p001); pface.push_back(p1);
	}
	vec_face.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p000, p0, p1, DirZ))
	{
		pface.push_back(p000); pface.push_back(p0); pface.push_back(p1);
	}
	else
	{
		pface.push_back(p0); pface.push_back(p000); pface.push_back(p1);
	}
	vec_face.push_back(pface); pface.clear();
	//左侧面//
	R1 = p002 - p000; R2 = p2 - p002;
	DirZ = R1*R2;
	if (KitTool::JudgeFaceDir(p0, p000, p002, DirZ))
	{
		pface.push_back(p0); pface.push_back(p000); pface.push_back(p002);
	}
	else
	{
		pface.push_back(p000); pface.push_back(p0); pface.push_back(p002);
	}
	vec_face.push_back(pface); pface.clear();


	if (KitTool::JudgeFaceDir(p002, p2, p0, DirZ))
	{
		pface.push_back(p002); pface.push_back(p2); pface.push_back(p0);
	}
	else
	{
		pface.push_back(p2); pface.push_back(p002); pface.push_back(p0);
	}
	vec_face.push_back(pface); pface.clear();

	//右侧面//
	R1 = p001 - p003; R2 = p1 - p002;
	DirZ = R1*R2;
	if (KitTool::JudgeFaceDir(p003, p001, p3, DirZ))
	{
		pface.push_back(p003); pface.push_back(p001); pface.push_back(p3);
	}
	else
	{
		pface.push_back(p001); pface.push_back(p003);  pface.push_back(p3);
	}
	vec_face.push_back(pface); pface.clear();


	if (KitTool::JudgeFaceDir(p3, p001, p1, DirZ))
	{
		pface.push_back(p3); pface.push_back(p001); pface.push_back(p1);
	}
	else
	{
		pface.push_back(p001); pface.push_back(p3); pface.push_back(p1);
	}
	vec_face.push_back(pface); pface.clear();
	DirZ.dx = 0; DirZ.dy = 0; DirZ.dz = 1;
	RotateHeadBox(vec_face, DirZ, DirV);
	//重新求法矢量
	for (int i = 0; i < vec_face.size(); i++)
	{
		p0 = vec_face[i][0]; p1 = vec_face[i][1]; p2 = vec_face[i][2];
		v_Normal = KitTool::CalcuPfacNormal(p0, p1, p2);
		OneNormal.x = v_Normal.dx; OneNormal.y = v_Normal.dy; OneNormal.z = v_Normal.dz;
		pface.clear(); pface.push_back(OneNormal); pface.push_back(p0); pface.push_back(p1); pface.push_back(p2);
		vec_face[i] = pface;
	}
}
bool COptisequence::GetIntereResult(vector<SInterB> &AllModelInfor, MyBlock CurrnetBlock, vector<vec_POINT3D> vec_Head, POINT3D pCenter, VECTOR3D Dir, int &InteBlock)
{
	//先通过包围盒测试相交性
	bool IfFind=false;
	vector<vec_POINT3D> oneModel;
	for (int i = 0; i < vec_Head.size();i++)
	{
		vec_POINT3D QFact = vec_Head[i];
		for (int j = 0; j < AllModelInfor.size(); j++)
		{
			oneModel.clear();
			oneModel = AllModelInfor[j].BoxDate;
			for (int k = 0; k < oneModel.size(); k++)
			{
				vec_POINT3D OneFact = oneModel[k];
				if (JudetheInsert(QFact, OneFact,pCenter,Dir))
				{
					AllModelInfor[j].status = true;
					IfFind = true;
					break;

					return true;
				}
			}
			if (IfFind) break;

		}
		if (IfFind) break;
	}
	//对通过包围盒测试相交的块进行面片相交测试
	for (int i = 0; i < vec_Head.size(); i++)
	{
		vec_POINT3D QFact = vec_Head[i];
		for (int j = 0; j < AllModelInfor.size(); j++)
		{
			if (AllModelInfor[j].status)
			{
				oneModel.clear();
				oneModel = AllModelInfor[j].ModelDate;
				for (int k = 0; k < oneModel.size(); k++)
				{
					vec_POINT3D OneFact = oneModel[k];
					if (JudetheInsert(QFact, OneFact, pCenter, Dir))
					{
						return true;
					}
				}
			}
		}
	}
	//将当前块面片信息加入总面片集
	JoinCurrentBlock(AllModelInfor,CurrnetBlock);
	return false;
}
void COptisequence::JoinCurrentBlock(vector<SInterB> &AllModelInfor, MyBlock CurrnetBlock)
{
	vector<vec_POINT3D> vec_Model, vec_Box;
	for (int i = 0; i < CurrnetBlock.OneBlock.size(); i++)
	{
		PFACETTRI pf0 = CurrnetBlock.OneBlock[i];
		vec_POINT3D Pface; POINT3D p0, p1, p2;
		p0.x = pf0->m_PVerts[0]->x; p0.y = pf0->m_PVerts[0]->y; p0.z = pf0->m_PVerts[0]->z;
		p1.x = pf0->m_PVerts[1]->x; p1.y = pf0->m_PVerts[1]->y; p1.z = pf0->m_PVerts[1]->z;
		p2.x = pf0->m_PVerts[2]->x; p2.y = pf0->m_PVerts[2]->y; p2.z = pf0->m_PVerts[2]->z;
		VECTOR3D v_Normal; POINT3D OneNormal;
		v_Normal = KitTool::CalcuPfacNormal(p0, p1, p2);
		OneNormal.x = v_Normal.dx; OneNormal.y = v_Normal.dy; OneNormal.z = v_Normal.dz;
		Pface.push_back(OneNormal); Pface.push_back(p0); Pface.push_back(p1); Pface.push_back(p2);
		vec_Model.push_back(Pface);
	}
	SInterB ModelInfo;
	ModelInfo.ModelDate = vec_Model;

	POINT3D p0, p1, p2, p3, p00, p01, p02, p03;
	p0 = CurrnetBlock.box_p0;          p03 = CurrnetBlock.box_p1;
	p1 = POINT3D(p0.x, p03.y, p0.z);   p2 = POINT3D(p03.x, p0.y, p0.z);
	p3 = POINT3D(p03.x, p03.y, p0.z);  p00 = POINT3D(p0.x,p03.y,p03.z);
	p01 = POINT3D(p0.x, p03.y, p03.z); p02 = POINT3D(p03.x, p0.y, p03.z); 

	//下底面//
	vec_POINT3D pface; VECTOR3D v_Normal; POINT3D OneNormal;
	VECTOR3D DirZ = VECTOR3D(0, 0, -1);

	if (KitTool::JudgeFaceDir(p0, p1, p2, DirZ))
	{
		pface.push_back(p0); pface.push_back(p1); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p0); pface.push_back(p2);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p1, p3, p2, DirZ))
	{
		pface.push_back(p1); pface.push_back(p3); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p1); pface.push_back(p2);
	}
	vec_Box.push_back(pface); pface.clear();
	//上底面//
	DirZ.dx = 0; DirZ.dy = 0; DirZ.dz = 1;

	if (KitTool::JudgeFaceDir(p00, p01, p02, DirZ))
	{
		pface.push_back(p00); pface.push_back(p01); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p01); pface.push_back(p00); pface.push_back(p02);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p01, p03, p02, DirZ))
	{
		pface.push_back(p01); pface.push_back(p03); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p03); pface.push_back(p01); pface.push_back(p02);
	}
	vec_Box.push_back(pface); pface.clear();
	//左侧面//
	DirZ.dx = 0; DirZ.dy = -1; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p00, p0, p2, DirZ))
	{
		pface.push_back(p00); pface.push_back(p0); pface.push_back(p2);
	}
	else
	{
		pface.push_back(p0); pface.push_back(p00); pface.push_back(p2);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p00, p2, p02, DirZ))
	{
		pface.push_back(p00); pface.push_back(p2); pface.push_back(p02);
	}
	else
	{
		pface.push_back(p2); pface.push_back(p00); pface.push_back(p02);
	}
	vec_Box.push_back(pface); pface.clear();
	//右侧面//
	DirZ.dx = 0; DirZ.dy = 1; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p01, p1, p3, DirZ))
	{
		pface.push_back(p01); pface.push_back(p1); pface.push_back(p3);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p01); pface.push_back(p3);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p01, p3, p03, DirZ))
	{
		pface.push_back(p01); pface.push_back(p3); pface.push_back(p03);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p01); pface.push_back(p03);
	}
	vec_Box.push_back(pface); pface.clear();
	//前侧面//
	DirZ.dx = 1; DirZ.dy = 0; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p02, p2, p3, DirZ))
	{
		pface.push_back(p02); pface.push_back(p2); pface.push_back(p3);
	}
	else
	{
		pface.push_back(p2); pface.push_back(p02); pface.push_back(p3);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p02, p3, p03, DirZ))
	{
		pface.push_back(p02); pface.push_back(p3); pface.push_back(p03);
	}
	else
	{
		pface.push_back(p3); pface.push_back(p02); pface.push_back(p03);
	}
	vec_Box.push_back(pface); pface.clear();
	//后侧面//
	DirZ.dx = -1; DirZ.dy = 0; DirZ.dz = 0;

	if (KitTool::JudgeFaceDir(p00, p0, p1, DirZ))
	{
		pface.push_back(p00); pface.push_back(p0); pface.push_back(p1);
	}
	else
	{
		pface.push_back(p0); pface.push_back(p00); pface.push_back(p1);
	}
	vec_Box.push_back(pface); pface.clear();

	if (KitTool::JudgeFaceDir(p00, p1, p01, DirZ))
	{
		pface.push_back(p00); pface.push_back(p1); pface.push_back(p01);
	}
	else
	{
		pface.push_back(p1); pface.push_back(p00); pface.push_back(p01);
	}
	vec_Box.push_back(pface); pface.clear();
	//重新求法矢量
	for (int i = 0; i < vec_Box.size(); i++)
	{
		p0 = vec_Box[i][0]; p1 = vec_Box[i][1]; p2 = vec_Box[i][2];
		v_Normal = KitTool::CalcuPfacNormal(p0, p1, p2);
		OneNormal.x = v_Normal.dx; OneNormal.y = v_Normal.dy; OneNormal.z = v_Normal.dz;
		pface.clear(); pface.push_back(OneNormal); pface.push_back(p0); pface.push_back(p1); pface.push_back(p2);
		vec_Box[i] = pface;
	}
	ModelInfo.BoxDate = vec_Box;
	AllModelInfor.push_back(ModelInfo);
}
bool COptisequence::JudetheInsert(vec_POINT3D Pfac1, vec_POINT3D Pfac2, POINT3D pCenter, VECTOR3D Dir)
{
	VECTOR3D dir,dir1,dir2;
	VECTOR3D  DirZ,d0; double s,s0, s1, s2;
	DirZ = VECTOR3D(0, 0, 1);
	dir1.dx = Pfac1[0].x; dir1.dy = Pfac1[0].y; dir1.dz = Pfac1[0].z;
	dir2.dx = Pfac2[0].x; dir2.dy = Pfac2[0].y; dir2.dz = Pfac2[0].z;
	dir = dir1*dir2;
	if (dir.GetLength()<AuxZero)
	{
		return false;
	}

	POINT3D PInsert;
	s = KitTool::AreaTri(Pfac2[1], Pfac2[2], Pfac2[3]);
	for (int i = 1; i < Pfac1.size();i++)
	{
		int nct = (i + 1) % 4;
		if (i == 3) nct = 1;
		if (KitTool::CalPlaneLineIntPoint(Pfac1[i], Pfac1[nct], Pfac2[1], dir2, PInsert))
		{
			s0 = KitTool::AreaTri(Pfac2[1], Pfac2[2], PInsert);
			s1 = KitTool::AreaTri(Pfac2[2], Pfac2[3], PInsert);
			s2 = KitTool::AreaTri(Pfac2[3], Pfac2[1], PInsert);
			if (s==(s0+s1+s2))
			{
				double theta = _AngleBetween3D(DirZ, Dir);
				VECTOR3D uAxis = DirZ * Dir;
				uAxis.Normalize();
				MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
				pCenter = pCenter*TM;
				d0 = pCenter - PInsert;
				s0 = d0 | Dir;
				if (fabs(s0)>AuxZero)
				{
					return true;
				}
			}
		}
	}
	
	/*d0 = KitTool::PlanePointDistance(Pfac1[1], Pfac2[1], dir2);
	d1 = KitTool::PlanePointDistance(Pfac1[2], Pfac2[1], dir2);
	d2 = KitTool::PlanePointDistance(Pfac1[3], Pfac2[1], dir2);
	s0 = d0 | d1; s1 = d1 | d2; s2 = d2 | d0;
	s0 = s0*s1*s2;
	if (s0>=0)
	{
	return false;
	}

	d0 = KitTool::PlanePointDistance(Pfac2[1], Pfac1[1], dir1);
	d1 = KitTool::PlanePointDistance(Pfac2[2], Pfac1[1], dir1);
	d2 = KitTool::PlanePointDistance(Pfac2[3], Pfac1[1], dir1);
	s0 = d0 | d1; s1 = d1 | d2; s2 = d2 | d0;
	s0 = s0*s1*s2;
	if (s0 >= 0)
	{
	return false;
	}*/
	return false;
}
void  COptisequence::RotateHeadBox(vector<vec_POINT3D> &vec_face, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < vec_face.size(); i++)
	{
		vec_POINT3D poly;
		poly = vec_face[i];
		for (int j = 0; j < poly.size(); j++)
		{
			POINT3D Temp;
			Temp.x = poly[j].x;
			Temp.y = poly[j].y;
			Temp.z = poly[j].z;
			Temp = Temp*TM;
			poly[j].x = Temp.x;
			poly[j].y = Temp.y;
			poly[j].z = Temp.z;
		}
		vec_face[i] = poly;
	}
}
void COptisequence::RotateBlock(MyBlock &OneBlock, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < OneBlock.OneBlock.size(); i++)
	{
		POINT3D Temp;
		PFACETTRI pFac;
		pFac = OneBlock.OneBlock[i];
		for (int j = 0; j < 3; j++)
		{
			if (pFac->m_PVerts[j]->bused!=1)
			{
				Temp.x = pFac->m_PVerts[j]->x;
				Temp.y = pFac->m_PVerts[j]->y;
				Temp.z = pFac->m_PVerts[j]->z;
				Temp = Temp*TM;
				pFac->m_PVerts[j]->x = Temp.x;
				pFac->m_PVerts[j]->y = Temp.y;
				pFac->m_PVerts[j]->z = Temp.z;
				pFac->m_PVerts[j]->bused = 1;
			}
		}
		OneBlock.OneBlock[i] = pFac; 
	}
	for (int i = 0; i < OneBlock.OneBlock.size(); i++)
	{
		PFACETTRI pFac;
		pFac = OneBlock.OneBlock[i];
		for (int j = 0; j < 3; j++)
		{
			if (pFac->m_PVerts[j]->bused == 1)
			{
				pFac->m_PVerts[j]->bused = 0;
			}
		}
		OneBlock.OneBlock[i] = pFac;
	}
}
void COptisequence::RotateDir(vector<VECTOR3D> &Vec_Dir, VECTOR3D curr_Dir, VECTOR3D tager_Dir)
{
	double theta = _AngleBetween3D(curr_Dir, tager_Dir);
	VECTOR3D uAxis = curr_Dir * tager_Dir;
	uAxis.Normalize();
	MATRIX3D TM = TM.CreateRotateMatrix(theta, uAxis);
	for (int i = 0; i < Vec_Dir.size(); i++)
	{
		VECTOR3D Dir;
		Dir = Vec_Dir[i];
		POINT3D Temp;
		Temp.x = Dir.dx;
		Temp.y = Dir.dy;
		Temp.z = Dir.dz;
		Temp = Temp*TM;
		Dir.dx = Temp.x;
		Dir.dy = Temp.y; 
		Dir.dz = Temp.z;
		Vec_Dir[i] = Dir;
	}
}
void COptisequence::DrawResult(vector<MyBlock> Rbox, vector<VECTOR3D> All_VecDir, drawTool *pDC, double len)
{
	COLORREF old_clr;
	pDC->GetColor(old_clr);
	pDC->SetColor(RGB(100, 10, 200));
	VECTOR3D Vx = VECTOR3D(1, 0, 0);
	for (int i = 0; i < Rbox.size(); i++)
	{
		BOX3D ONE;
		UpdateBlockBox(Rbox[i].OneBlock, ONE);
		POINT3D P0;
		double adle = cos(Vx | All_VecDir[i]);
		if (adle<0.600)
		{
			P0.x = (ONE.x0 + ONE.x1)*0.5;
			P0.y = ONE.y0;
			P0.z = (ONE.z0 + ONE.z1)*0.5;
		}
		else
		{
			P0.x = ONE.x1;
			P0.y = (ONE.y0 + ONE.y1)*0.5;
			P0.z = (ONE.z0 + ONE.z1)*0.5;
		}
		glRasterPos3d(P0.x, P0.y, P0.z);
		string ss = pDC->itos(i);
		char c[3];
		strcpy(c, ss.c_str());
		pDC->DrawText(c,0.8*len);
	}
	pDC->SetColor(old_clr); //恢复绘图颜色
}


bool COptisequence::GettheBlockpath(vec_PPOLYPOLYGON &OnelRotateBlock, POINT3D Plow, MATRIX3D TM)
{
	//平移
	VECTOR3D Tran_Dir;
	//POINT3D pTheLow = POINT3D(141.9, 105.9, z0);
	Tran_Dir = POINT3D(0, 0, 0) - Plow;
	MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);

	//平移回原来位置
	Tran_Dir = Plow - POINT3D(0, 0, 0);
	MATRIX3D TM1 = TM1.CreateTransfMatrix(Tran_Dir);

	//平移填充轮廓///
	for (int i = 0; i < OnelRotateBlock.size(); i++)
	{
		PPOLYPOLYGON OneLayer = OnelRotateBlock[i];
		for (int j = 0; j < OneLayer->m_vecPPolygons.size(); j++)
		{
			vec_POINT3D OneCircle = OneLayer->m_vecPPolygons[j]->m_vecPnts;
			for (int k = 0; k < OneCircle.size(); k++)
			{
				POINT3D Temp;
				Temp.x = OneCircle[k].x;
				Temp.y = OneCircle[k].y;
				Temp.z = OneCircle[k].z;
				Temp = Temp*TM0;//平移轮廓
				Temp = Temp*TM;//旋转轮廓
				Temp = Temp*TM1;//平移回去轮廓
				OneCircle[k].x = Temp.x;
				OneCircle[k].y = Temp.y;
				OneCircle[k].z = Temp.z;
			}
			OneLayer->m_vecPPolygons[j]->m_vecPnts = OneCircle;
		}
		OnelRotateBlock[i] = OneLayer;
	}

	return true;
}

void COptisequence::CalRotateMaix(VECTOR3D &Dir_now, MATRIX3D &TM)
{
	//变换最优成型方向
	POINT3D Temp;
	Temp.x = Dir_now.dx;
	Temp.y = Dir_now.dy;
	Temp.z = Dir_now.dz;
	Temp = Temp*TM;
	Dir_now.dx = Temp.x;
	Dir_now.dy = Temp.y;
	Dir_now.dz = Temp.z;

	double theta = _AngleBetween3D(Dir_now, VECTOR3D(0, 0, 1));
	VECTOR3D uAxis = Dir_now * VECTOR3D(0, 0, 1);
	uAxis.Normalize();
	MATRIX3D TMtemp = TMtemp.CreateRotateMatrix(theta, uAxis);
	TM *= TMtemp;//累积变换矩阵
}
void COptisequence::TranRbox(vector<MyBlock> &Rbox,POINT3D plow)
{
	//平移
	VECTOR3D Tran_Dir;
	POINT3D pCET = POINT3D(20, 20, 0);
	Tran_Dir = pCET - plow;
	MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);

	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	vec_PVERT FeaPoint;
	for (int i = 0; i < Rbox.size(); i++)
	{
		for (int j = 0; j < Rbox[i].OneBlock.size(); j++)
		{
			PFACETTRI ptri = Rbox[i].OneBlock[j];
			for (int k = 0; k < 3;k++)
			{
				PVERT POint = ptri->m_PVerts[k];
				iter = map_Vert.find(*POint);
				if (iter == map_Vert.end())//不存在该点
				{
					map_Vert.insert(pair<VERT, int>(*POint, FeaPoint.size()));
					FeaPoint.push_back(POint);

					POINT3D Temp;
					Temp.x = ptri->m_PVerts[k]->x;
					Temp.y = ptri->m_PVerts[k]->y;
					Temp.z = ptri->m_PVerts[k]->z;
					Temp = Temp*TM0;//平移轮廓

					ptri->m_PVerts[k]->x = Temp.x;
					ptri->m_PVerts[k]->y = Temp.y;
					ptri->m_PVerts[k]->z = Temp.z;
				}
				//POINT3D Temp;
				//Temp.x = ptri->m_PVerts[k]->x;
				//Temp.y = ptri->m_PVerts[k]->y;
				//Temp.z = ptri->m_PVerts[k]->z;
				//Temp = Temp*TM0;//平移轮廓

				//ptri->m_PVerts[k]->x = Temp.x;
				//ptri->m_PVerts[k]->y = Temp.y;
				//ptri->m_PVerts[k]->z = Temp.z;
				delete ptri->m_PFacetNorm; ptri->m_PFacetNorm = NULL;
				ptri->m_PFacetNorm = KitTool::CalcuPfacNormal(ptri->m_PVerts[0], ptri->m_PVerts[1], ptri->m_PVerts[2]);
				Rbox[i].OneBlock[j] = ptri;
			}
		}
	}
}
void COptisequence::TranRbox(vector<vec_PPOLYPOLYGON> &BlockFill, POINT3D plow)
{
	//平移
	VECTOR3D Tran_Dir;
	POINT3D pCET = POINT3D(20, 20, 0);
	Tran_Dir = pCET - plow;
	MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);
	for (int i = 0; i < BlockFill.size();i++)
	{
		for (int j = 0; j < BlockFill[i].size(); j++)
		{
			int Num = BlockFill[i][j]->m_vecPPolygons.size();
			for (int k = 0; k < Num; k++)
			{
				int Nub = BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts.size();
				for (int m = 0; m < Nub; m++)
				{
					POINT3D ptemp;
					ptemp.x = BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].x;
					ptemp.y = BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].y;
					ptemp.z = BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].z;
					ptemp = ptemp*TM0;//平移轮廓
					BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].x = ptemp.x;
					BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].y = ptemp.y;
					BlockFill[i][j]->m_vecPPolygons[k]->m_vecPnts[m].z = ptemp.z;
				}
			}
		}
	}
	
	
}

