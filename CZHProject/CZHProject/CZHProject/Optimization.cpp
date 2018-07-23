
#include "Optimization.h"
#include "DrawTool.h"
#include <ctime>

COptDir::COptDir()
{
	for (int i=0;i<10;i++)
	{
		vector<int>  TriNum;
		m_vecPFacetTri.push_back(TriNum);
	}
}

COptDir::~COptDir()
{

}
//////////////////////////////////////////////////////
/// 基于遗传算法的的优化，种群大小120，交叉率0.75  ///
/// 变异率0.01，最大代数200，双点交叉。———Extra_Lin ///
//////////////////////////////////////////////////////
/// 保留最优种群，每次都判断最优种群//////////////////
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
///多目标优化，NSGA-II，MOPSO 两种算法提供////////////
//////////////////////////////////////////////////////



VECTOR2D COptDir::GA(STLModell *pSTL)
{
	vec_VECTOR2D ALLDir;
	int SZ=120;
	ALLDir=RandomDir(SZ);
	bool IFgood=false;//为了解决正好为0的情况
	int j=0;
	int gene=0;
	//VECTOR2D v1(0,0);//调试专用
	vector<double> AllSum;
	int myCtrlPos;
	vector<double>    vec_Area;
	for(int i=0;i<pSTL->m_vecPFacetTri.size();i++)
	{
		double S=KitTool::GetTriArea(pSTL->m_vecPFacetTri[i]);
		vec_Area.push_back(S);
	}
	///////////////迭代循环
	do 
	{
		myCtrlPos=gene/2;
		AllSum.clear();
		set<i2Sum>  betterSum;
		///////////////计算适应度函数
		for (int i=0;i<SZ;i++)
		{
			
			double temp = SA(pSTL->m_vecFacetNorm, ALLDir[i], vec_Area);
			if (temp==0)
			{
				IFgood=true;
				j=i;
				break;
			} 
			else
			{
				i2Sum i2s;
				i2s.i=i;
				i2s.sum=1/temp;
				AllSum.push_back(1/temp);
				betterSum.insert(i2s);
			}	
		}
		if (IFgood==true)
		{
			break;
		} 
		else
		{
			vector<int> vec_betterDir;
			set<i2Sum>::iterator iterS;
			iterS=betterSum.end();
			//////取出最优适应度的30个方向////
			int bestSumSZ=betterSum.size();
			if (bestSumSZ>=30)
			{
				for (int i=0;i<30;i++)
				{
					iterS--;
					vec_betterDir.push_back(iterS->i);
				}
			} 
			else
			{
				////////////处理bestSum的个数小于30
				for (int i=0;i<bestSumSZ;i++)
				{
					iterS--;
					vec_betterDir.push_back(iterS->i);
				}
				do 
				{
					vec_betterDir.push_back(vec_betterDir.back());
				} while (vec_betterDir.size()<30);
			}
			if (bestSumSZ==1)
			{

				return ALLDir[vec_betterDir[0]];

			}
			vector<int> vec_DirInt;
			///////选择算子
			vec_DirInt=GA_selectDir(ALLDir,AllSum);
			vec_VECTOR2D vec_Temp;
			for(int i=0;i<90;i++)
			{
				int v1=vec_DirInt[i];
				i++;
				int v2=vec_DirInt[i];
				//////交叉算子
				vec_VECTOR2D temp=GA_Crossover(ALLDir[v1],ALLDir[v2]);
				vec_Temp.push_back(temp[0]);
				vec_Temp.push_back(temp[1]);
			}
			for (int i=0;i<30;i++)
			{
				vec_Temp.push_back(ALLDir[vec_betterDir[i]]);
			}
			ALLDir=vec_Temp;
			///////变异算子
			int  ifMutation =(double)rand()/RAND_MAX*100;
			if (ifMutation<1)
			{
				int Mutation=(double)rand()/RAND_MAX*119;
				ALLDir[Mutation]=GA_Mutation();
			}		
			gene++;
		}
	} while (gene<200);
	if (IFgood==true)
	{
		return ALLDir[j];
	} 
	else
	{
		////处理最后一代种群的适应度函数
		AllSum.clear();
		for (int i=0;i<SZ;i++)
		{
			double temp=SA(pSTL->m_vecFacetNorm,ALLDir[i],vec_Area);
			if (temp==0)
			{
				IFgood=true;
				j=i;
				break;
			} 
			else
			{
				AllSum.push_back(1/temp);
			}	
		}
		double Maxsum=AllSum[0];
		int maxI=0;
		for (int i=1;i<SZ;i++)
		{
			if (Maxsum<AllSum[i])
			{
				Maxsum=AllSum[i];
				maxI=i;
			}
		}
		//VisualResult(pSTL->m_vecFacetNorm,ALLDir[maxI]);
		return ALLDir[maxI];
	}
	
}
/////////////////////基于最好表面质量的优化///////
double COptDir::SA(vec_PVECTOR3D m_vecPFacetNorm,VECTOR2D DirV,vector<double>vec_Area)
{	
	
	int SZ=m_vecPFacetNorm.size();
	double Sum=0;
	VECTOR3D v1;
	v1.dx=sin(DirV.dy)*cos(DirV.dx);
	v1.dy=sin(DirV.dy)*sin(DirV.dx);
	v1.dz=cos(DirV.dy);
	for (int i=0;i<SZ;i++)
	{
		double t=v1|(*m_vecPFacetNorm[i]);
		double absT=abs(t);
		//if (t>0&&abs(t)<1)
		if (absT>CAD_ZERO&&absT<(1-CAD_ZERO))
		{
			double u=absT*vec_Area[i];
			Sum+=u;
		}
	}
	return Sum;
}

//////基于最小支撑面积的优化/////
double COptDir::SE(vec_PVECTOR3D m_vecPFacetNorm,VECTOR2D DirV,vector<double>vec_Area)
{
	int SZ=m_vecPFacetNorm.size();
	double Sum=0;
	VECTOR3D v1;
	v1.dx=sin(DirV.dy)*cos(DirV.dx);
	v1.dy=sin(DirV.dy)*sin(DirV.dx);
	v1.dz=cos(DirV.dy);
	for (int i=0;i<SZ;i++)
	{
		VECTOR3D mNormal = *m_vecPFacetNorm[i];
		double t=v1|mNormal ;
		double absT=abs(t);
		//if (t>0&&abs(t)<1)
		if (absT>CAD_ZERO&&absT<(1-CAD_ZERO))
		{
			if (t<0)
			{
				double u=absT*vec_Area[i];
				Sum+=u;
			}
		}
	}
	return Sum;
}
////////////////////////初始种群
vec_VECTOR2D COptDir::RandomDir(int SZ)
{
	vec_VECTOR2D VEC_DirV;
	double a=0,b=2*PI;
	double u=0,v=PI;
	srand((unsigned)time(NULL));//播种
	for (int i =0;i<SZ;i++)
	{
		VECTOR2D DirV;
		DirV.dx=(double)rand()/RAND_MAX*(b-a);
		DirV.dy=(double)rand()/RAND_MAX*(v-u)-(PI/2);
		VEC_DirV.push_back(DirV);
	}
    return VEC_DirV;
}
////////////////////////选择算子
vector<int> COptDir::GA_selectDir(vec_VECTOR2D AllDir,vector<double> AllSum)
{
	vector<int>   vec_DirInt;
	double Sum=0;
	int SZ=AllSum.size();
	for(int i=0;i<SZ;i++)
	{
		Sum+=AllSum[i];
	}
	double a=0;
	double b=Sum;
	do 
	{
		double temp=0;
		double t=(double)rand()/RAND_MAX*(b-a);
		for (int i=0;i<SZ;i++)
		{
			temp+=AllSum[i];
			if (temp>t)
			{
				vec_DirInt.push_back(i);
				break;
			}

		}
	} while (vec_DirInt.size()!=90);

	return vec_DirInt;
}
///////////////////////交叉算子
vec_VECTOR2D COptDir::GA_Crossover(VECTOR2D v1,VECTOR2D v2)
{
	vec_VECTOR2D vec;
	VECTOR2D tempv1;
	VECTOR2D tempv2;
	int a=0;
	int b=24;
	double t1=(double)rand()/RAND_MAX*(b-a);
	double t2=(double)rand()/RAND_MAX*(b-a);
	tempv1.dx=(v1.dx*t1+v2.dx*(24-t1))/24;
	tempv1.dy=(v2.dy*t2+v1.dy*(24-t2))/24;
	tempv2.dx=(v2.dx*t1+v1.dx*(24-t1))/24;
	tempv2.dy=(v1.dy*t2+v2.dy*(24-t2))/24;
	vec.push_back(tempv1);
	vec.push_back(tempv2);
	return vec;
}
///////////////////变异
VECTOR2D COptDir::GA_Mutation()
{
	vec_VECTOR2D vec_V=RandomDir(1);
	return vec_V[0];
}


//double COptDir::GetTriArea(PFACETTRI ptri)
//{
//	vector<POINT3D> m_vecPoint;
//	for (int i=0;i<3;i++)
//	{
//		m_vecPoint.push_back(*ptri->m_PVerts[i]);
//	}
//	VECTOR3D v1=m_vecPoint[1]-m_vecPoint[0];
//	VECTOR3D v2=m_vecPoint[2]-m_vecPoint[0];
//	VECTOR3D v3=v1*v2;
//	return 0.5*v3.GetLength();
//}


/////////////////评价系统////////
void  COptDir::VisualResult(vec_PVECTOR3D m_vecPFacetNorm,VECTOR3D DirV)
{
	int SZ=m_vecPFacetNorm.size();
	for (int i=0;i<SZ;i++)
	{
		VECTOR3D mNormal = *m_vecPFacetNorm[i];
		double t=abs(DirV|mNormal);
		if (t>0.9894&&t<=1.0000-CAD_ZERO)
		{
			m_vecPFacetTri[0].push_back(i);
			continue;
		};
		if (t<=0.9894&&t>0.9397)
		{
			m_vecPFacetTri[1].push_back(i);
			continue;
		};
		if (t<=0.9397&&t>0.8660)
		{
			m_vecPFacetTri[2].push_back(i);
			continue;
		};
		if (t<=0.8660&&t>0.7660)
		{
			m_vecPFacetTri[3].push_back(i);
			continue;
		};
		if (t<=0.7660&&t>0.6428)
		{
			m_vecPFacetTri[4].push_back(i);
			continue;
		};
		if (t<=0.64286&&t>0.5000)
		{
			m_vecPFacetTri[5].push_back(i);
			continue;
		};
		if (t<=0.5000&&t>0.3420)
		{
			m_vecPFacetTri[6].push_back(i);
			continue;
		};
		if (t<=0.3420&&t>0.1736)
		{
			m_vecPFacetTri[7].push_back(i);
			continue;
		};
		if (t<=0.1736&&t>CAD_ZERO)
		{
			m_vecPFacetTri[8].push_back(i);
			continue;
		};
		if ((t>=1.0000-CAD_ZERO&&t<=1.0000)|(t>=0&&t<=CAD_ZERO))
		{
			m_vecPFacetTri[9].push_back(i);
			continue;
		}
	}
}
/////////画出优化结果图//
void COptDir::DrawResult(vec_PFACETTRI m_vecBlockFacet, vector<vector<int>> m_vecPFacetTri, drawTool *pDC)
{
	COLORREF old_clr, old_clr1;
	pDC->GetMaterialColor(old_clr);
	pDC->GetColor(old_clr1);
	glDisable(GL_LIGHTING);
	vector<PVECTOR3D> m_PFacetNorm;
	for (int i = 0; i < m_vecBlockFacet.size(); i++)
	{
		m_PFacetNorm.push_back(m_vecBlockFacet[i]->m_PFacetNorm);
	}
	for (int i = 0; i < 10; i++)
	{
		for (int m = 0; m < 1; m++)
		{
			if (i == 0)
			{
				pDC->SetMaterialColor(RGB(255, 0, 0));
				pDC->SetColor(RGB(255, 0, 0));
				break;
			}
			if (i == 1)
			{
				pDC->SetMaterialColor(RGB(255, 193, 37));
				pDC->SetColor(RGB(255, 193, 37));
				break;
			}
			if (i == 2)
			{
				pDC->SetMaterialColor(RGB(255, 255, 0));
				pDC->SetColor(RGB(255, 255, 0));
				break;
			}
			if (i == 3)
			{
				pDC->SetMaterialColor(RGB(202, 255, 122));
				pDC->SetColor(RGB(202, 255, 122));
				break;
			}
			if (i == 4)
			{
				pDC->SetMaterialColor(RGB(0, 255, 0));
				pDC->SetColor(RGB(0, 255, 0));
				break;
			}
			if (i == 5)
			{
				pDC->SetMaterialColor(RGB(0, 255, 255));
				pDC->SetColor(RGB(0, 255, 255));
				break;
			}
			if (i == 6)
			{
				pDC->SetMaterialColor(RGB(0, 206, 209));
				pDC->SetColor(RGB(0, 206, 209));
				break;
			}
			if (i == 7)
			{
				pDC->SetMaterialColor(RGB(30, 144, 255));
				pDC->SetColor(RGB(30, 144, 255));
				break;
			}
			if (i == 8)
			{
				pDC->SetMaterialColor(RGB(0, 0, 255));
				pDC->SetColor(RGB(0, 0, 255));
				break;
			}
			if (i == 9)
			{
				pDC->SetMaterialColor(RGB(0, 0, 0));
				pDC->SetColor(RGB(0, 0, 0));
				break;
			}
		}
		
		int sz = m_vecPFacetTri[i].size();
		for (int j = 0; j < sz; j++)
		{
			int n = m_vecPFacetTri[i][j];
			pDC->DrawTriChip(m_PFacetNorm[n]->dx, m_PFacetNorm[n]->dy, m_PFacetNorm[n]->dz,
				m_vecBlockFacet[n]->m_PVerts[0]->x, m_vecBlockFacet[n]->m_PVerts[0]->y, m_vecBlockFacet[n]->m_PVerts[0]->z,
				m_vecBlockFacet[n]->m_PVerts[1]->x, m_vecBlockFacet[n]->m_PVerts[1]->y, m_vecBlockFacet[n]->m_PVerts[1]->z,
				m_vecBlockFacet[n]->m_PVerts[2]->x, m_vecBlockFacet[n]->m_PVerts[2]->y, m_vecBlockFacet[n]->m_PVerts[2]->z
				);
		}
	}
	pDC->SetMaterialColor(old_clr);
	pDC->SetColor(old_clr1);
	glEnable(GL_LIGHTING);

}


/////////画出优化结果图//
void COptDir::DrawResult(STLModell* pSTLModel , vector<vector<int>> m_vecPFacetTri,drawTool *pDC)
{
	COLORREF old_clr, old_clr1;
	pDC->GetMaterialColor(old_clr);
	pDC->GetColor(old_clr1);
	glDisable(GL_LIGHTING);
	for (int i=0;i<10;i++)
	{
		for (int m=0 ;m<1;m++)
		{
			if(i==0)
			{
				pDC->SetMaterialColor(RGB(255, 0, 0));
				pDC->SetColor(RGB(255, 0, 0));
				break;
			}
			if(i==1)
			{
				pDC->SetMaterialColor(RGB(255, 193, 37));
				pDC->SetColor(RGB(255, 193, 37));
				break;
			}
			if(i==2)
			{
				pDC->SetMaterialColor(RGB(255, 255, 0));
				pDC->SetColor(RGB(255, 255, 0));
				break;
			}
			if(i==3)
			{
				pDC->SetMaterialColor(RGB(202, 255, 122));
				pDC->SetColor(RGB(202, 255, 122));
				break;
			}
			if(i==4)
			{
				pDC->SetMaterialColor(RGB(0, 255, 0));
				pDC->SetColor(RGB(0, 255, 0));
				break;
			}
			if(i==5)
			{
				pDC->SetMaterialColor(RGB(0, 255, 255));
				pDC->SetColor(RGB(0, 255, 255));
				break;
			}
			if(i==6)
			{
				pDC->SetMaterialColor(RGB(0, 206, 209));
				pDC->SetColor(RGB(0, 206, 209));
				break;
			}
			if(i==7)
			{
				pDC->SetMaterialColor(RGB(30, 144, 255));
				pDC->SetColor(RGB(30, 144, 255));
				break;
			}
			if(i==8)
			{
				pDC->SetMaterialColor(RGB(0, 0, 255));
				pDC->SetColor(RGB(0, 0, 255));
				break;
			}
			if (i==9)
			{
				pDC->SetMaterialColor(RGB(0, 0, 0));
				pDC->SetColor(RGB(0, 0, 0));
				break;
			}
		}
		
		int sz=m_vecPFacetTri[i].size();
		for (int j=0;j<sz;j++)
		{
			int n=m_vecPFacetTri[i][j];
			pDC->DrawTriChip(pSTLModel->m_vecFacetNorm[n]->dx, pSTLModel->m_vecFacetNorm[n]->dy, pSTLModel->m_vecFacetNorm[n]->dz,
				pSTLModel->m_vecPFacetTri[n]->m_PVerts[0]->x,pSTLModel->m_vecPFacetTri[n]->m_PVerts[0]->y,pSTLModel->m_vecPFacetTri[n]->m_PVerts[0]->z,
				pSTLModel->m_vecPFacetTri[n]->m_PVerts[1]->x,pSTLModel->m_vecPFacetTri[n]->m_PVerts[1]->y,pSTLModel->m_vecPFacetTri[n]->m_PVerts[1]->z,
				pSTLModel->m_vecPFacetTri[n]->m_PVerts[2]->x,pSTLModel->m_vecPFacetTri[n]->m_PVerts[2]->y,pSTLModel->m_vecPFacetTri[n]->m_PVerts[2]->z	
				);
		}
	}
	pDC->SetMaterialColor(old_clr);
	pDC->SetColor(old_clr1);
	glEnable(GL_LIGHTING);

}
void  COptDir::DrawOptiDir(drawTool *pDC, VECTOR3D m_VecDir, BOX3D ONE,double len)
{
	////////画出最优方向
	POINT3D p1, p2;
	glDisable(GL_LIGHTING);
	p1.x = (ONE.x0 + ONE.x1) / 2;
	p1.y = (ONE.y0 + ONE.y1) / 2;
	p1.z = ONE.z1;

	len *= 0.05;
	p2 = p1 + m_VecDir*len * 2;
	glLineWidth(3);
	COLORREF old_clr1, old_clr2;
	pDC->GetMaterialColor(old_clr1);
	pDC->GetColor(old_clr2);
	pDC->SetMaterialColor(RGB(160, 32, 240));
	pDC->SetColor(RGB(160, 32, 240));
	pDC->DrawLine(p1, p2);
	pDC->Drawcone(p2, len / 5, m_VecDir*len / 2);
	pDC->SetMaterialColor(old_clr1);
	pDC->SetColor(old_clr2);
	glLineWidth(1);
	glEnable(GL_LIGHTING);
	DrawInfo(pDC);
}
//////画标尺//////
void COptDir::DrawInfo(drawTool *pDC)
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	COLORREF old_COLOR;
	pDC->GetColor(old_COLOR);

	/*pDC->SetColor(qRgb(0, 0, 0));
	glRectf(0.825, 0.4, 0.85, 0.322);

	pDC->SetColor(qRgb(0,0,255));
	glRectf(0.825,0.32,0.85,0.242);

	pDC->SetColor(qRgb(30,144,255));
	glRectf(0.825,0.24,0.85,0.162);

	pDC->SetColor(qRgb(0,206,209));
	glRectf(0.825,0.16,0.85,0.082);

	pDC->SetColor(qRgb(0,255,255));
	glRectf(0.825,0.08,0.85,0.002);

	pDC->SetColor(qRgb(0,255,0));
	glRectf(0.825,0,0.85,-0.078);

	pDC->SetColor(qRgb(202,255,122));
	glRectf(0.825,-0.08,0.85,-0.158);

	pDC->SetColor(qRgb(255,255,0));
	glRectf(0.825,-0.16,0.85,-0.238);

	pDC->SetColor(qRgb(255,193,37));
	glRectf(0.825,-0.24,0.85,-0.318);

	pDC->SetColor(qRgb(255,0,0));
	glRectf(0.825,-0.32,0.85,-0.398);*/

	pDC->SetColor(qRgb(0, 0, 255));
	glRectf(0.825, 0.4, 0.85, 0.322);

	pDC->SetColor(qRgb(30, 144, 255));
	glRectf(0.825, 0.32, 0.85, 0.242);

	pDC->SetColor(qRgb(0, 206, 209));
	glRectf(0.825, 0.24, 0.85, 0.162);

	pDC->SetColor(qRgb(0, 255, 255));
	glRectf(0.825, 0.16, 0.85, 0.082);

	pDC->SetColor(qRgb(0, 255, 0));
	glRectf(0.825, 0.08, 0.85, 0.002);

	pDC->SetColor(qRgb(202, 255, 122));
	glRectf(0.825, 0, 0.85, -0.078);

	pDC->SetColor(qRgb(255, 255, 0));
	glRectf(0.825, -0.08, 0.85, -0.158);

	pDC->SetColor(qRgb(255, 193, 37));
	glRectf(0.825, -0.16, 0.85, -0.238);

	pDC->SetColor(qRgb(255, 0, 0));
	glRectf(0.825, -0.24, 0.85, -0.318);

	pDC->SetColor(qRgb(0, 0, 0));
	glRectf(0.825, -0.32, 0.85, -0.398);

	pDC->SetColor(qRgb(0, 0, 0));
	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glLineWidth(2);
	glRectf(0.824,0.4,0.851,-0.3999);
	glBegin(GL_LINES);
		glVertex2f(0.824,0.32);
	    glVertex2f(0.851,0.32);
		glVertex2f(0.824,0.24);
		glVertex2f(0.851,0.24);
		glVertex2f(0.824,0.16);
		glVertex2f(0.851,0.16);
		glVertex2f(0.824,0.08);
		glVertex2f(0.851,0.08);
		glVertex2f(0.824,0.0);
		glVertex2f(0.851,0.0);
		glVertex2f(0.824,-0.08);
		glVertex2f(0.851,-0.08);
		glVertex2f(0.824,-0.16);
		glVertex2f(0.851,-0.16);
		glVertex2f(0.824,-0.24);
		glVertex2f(0.851,-0.24);
		glVertex2f(0.824,-0.32);
		glVertex2f(0.851,-0.32);
	glEnd();


	pDC->SetColor(qRgb(0,0,0));
	glRasterPos2f(0.75,0.34);
	pDC->DrawText("Bad  ",12);
	glRasterPos2f(0.75,0.26);
	pDC->DrawText("Level 1",12);
	glRasterPos2f(0.75,0.18);
	pDC->DrawText("Level 2",12);
	glRasterPos2f(0.75,0.1);
	pDC->DrawText("Level 3",12);
	glRasterPos2f(0.75,0.02);
	pDC->DrawText("Level 4",12);
	glRasterPos2f(0.75,-0.06);
	pDC->DrawText("Level 5",12);
	glRasterPos2f(0.75,-0.14);
	pDC->DrawText("Level 6",12);
	glRasterPos2f(0.75,-0.22);
	pDC->DrawText("Level 7",12);
	glRasterPos2f(0.75,-0.30);
	pDC->DrawText("Level 8",12);
	glRasterPos2f(0.75,-0.38);
	pDC->DrawText("Best  ",12);

	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnable(GL_LIGHTING);
	pDC->SetColor(old_COLOR);
	glLineWidth(1);
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////
/////////////////////////////NSGA_ii/////////////////////////////////
/////////////////////////////////////////////////////////////////////

vector<vector<UNIT>> COptDir::non_dominatedSet(vec_VECTOR2D MyDirS,vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area)
{
	///////////加载目标函数
	vector<vector<UNIT>>  my_Dominated;
	vector<pUNIT> vec_pUnit;
	int SZ=MyDirS.size();
	for (int i=0;i<SZ;i++)
	{
		pUNIT myUnit=new UNIT;
		myUnit->Dir=MyDirS[i];
		myUnit->f1Sum=SE(m_vecPFacetNorm,MyDirS[i],vec_Area);
		myUnit->f2Sum=SA(m_vecPFacetNorm,MyDirS[i],vec_Area);
		vec_pUnit.push_back(myUnit);

	}
	////////非支配集合/////
	for(int i=0; i <SZ-1;i++)
	{
		for(int j=i+1;j<SZ;j++)
		{
			if (vec_pUnit[i]->f1Sum<vec_pUnit[j]->f1Sum)
			{
				if (vec_pUnit[i]->f2Sum<=vec_pUnit[j]->f2Sum)
				{
					vec_pUnit[i]->S.push_back(vec_pUnit[j]);
					vec_pUnit[j]->n++;
				}
			}
			else
			{
				if (vec_pUnit[i]->f1Sum==vec_pUnit[j]->f1Sum)
				{
					if (vec_pUnit[i]->f2Sum<vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[i]->S.push_back(vec_pUnit[j]);
						vec_pUnit[j]->n++;
					}
					else
					{
						if (vec_pUnit[i]->f2Sum>vec_pUnit[j]->f2Sum)
						{
							vec_pUnit[j]->S.push_back(vec_pUnit[i]);
							vec_pUnit[i]->n++;
						}
					}
				}
				else
				{
					if (vec_pUnit[i]->f2Sum>vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[j]->S.push_back(vec_pUnit[i]);
						vec_pUnit[i]->n++;
					}
				}


			}		
		}
	}
	//////非支配等级分配///////
	set<pUNIT>  set_Unit;/////用于查找重复
	pair<set<pUNIT>::iterator,bool>  pairS;/////用于查找重复
	vector<pUNIT> vec_oper=vec_pUnit;

	do 
	{
		set_Unit.clear();
		int Level=1;
		vector<pUNIT> vec_temp;
		vector<UNIT> vec_dominated;
		int vecSz=vec_oper.size();
		vector<int>  vec_int;
		for (int i=0;i<vecSz;i++)
		{
			if (vec_oper[i]->n==0)
			{				
				/////赋支配等级
				vec_oper[i]->level=Level;
				vec_dominated.push_back(*vec_oper[i]);
				vec_int.push_back(i);
			}
		}
		for (int i=0;i<vec_int.size();i++)
		{
			int num =vec_int[i];
			int Ssz=vec_oper[num]->S.size();
			////////处理下一次循环的数据
			for (int j=0;j<Ssz;j++)
			{
				vec_oper[num]->S[j]->n--;
				pairS=set_Unit.insert(vec_oper[num]->S[j]);
				if (pairS.second!=0)
				{
					vec_temp.push_back(vec_oper[num]->S[j]);
				}
			}
		}
		Distance(vec_dominated);/////计算拥挤距离
		my_Dominated.push_back(vec_dominated);
		vec_oper=vec_temp;
		Level++;
	} while (vec_oper.size()!=0);
	/////析构释放内存/////
	for (int i=0;i<SZ;i++)
	{
		delete vec_pUnit[i];
	}
	return my_Dominated;
}


////赋予拥挤距离
void COptDir::Distance(vector<UNIT> &vec_dominated)
{
	int sz=vec_dominated.size();
	for (int i=0;i<sz-1;i++)
	{
		for (int j=0;j<sz-1;j++)
		{
			if (vec_dominated[j].f1Sum>vec_dominated[j+1].f1Sum)
			{
				UNIT temp=vec_dominated[j];
				vec_dominated[j+1]=vec_dominated[j];
				vec_dominated[j]=vec_dominated[j+1];
			}
		}
	}
	////赋予拥挤距离
	for (int i=0;i<sz;i++)
	{
		if (i==0|i==sz-1)
		{
			vec_dominated[i].distance=-1;/////-1表示是最好的！
		}
		else
		{
			vec_dominated[i].distance=vec_dominated[i-1].f2Sum-vec_dominated[i+1].f2Sum+
				vec_dominated[i+1].f1Sum-vec_dominated[i-1].f1Sum;
		}
	}
}


vector<UNIT> COptDir::NSGA_ii(STLModell *pSTL)
{
	vec_VECTOR2D ALLDir;
	vector<UNIT> LastUnit;
	int SZ=120;
	ALLDir=RandomDir(SZ);
	int gene=0;
	//VECTOR2D v1(0,0);//调试专用
	vector<double> AllSum;
	int myCtrlPos;
	srand((unsigned)time(NULL));
	vector<double>    vec_Area;
	for(int i=0;i<pSTL->m_vecPFacetTri.size();i++)
	{
		double S=KitTool::GetTriArea(pSTL->m_vecPFacetTri[i]);
		vec_Area.push_back(S);
	}
	vector<vector<UNIT>> vec_Unit=non_dominatedSet(ALLDir,pSTL->m_vecFacetNorm,vec_Area);
	do 
	{	
		///////进度条/////
		CGlobal::m_myCtrlSet=gene;
		vector<UNIT> vec_Select=TournamentSelection(vec_Unit,SZ);
		for (int i=0;i<SZ/2;i++)
		{
			VECTOR2D v1=vec_Select[i].Dir;
			VECTOR2D v2=vec_Select[vec_Select.size()-1-i].Dir;
			vec_VECTOR2D  CrossDir = GA_Crossover(v1,v2);
			ALLDir.push_back( CrossDir[0]);
			ALLDir.push_back( CrossDir[1]);
		}
		vector<vector<UNIT>> vec_MyDominated = non_dominatedSet(ALLDir,pSTL->m_vecFacetNorm,vec_Area);
		vec_Unit=NSGA_ii_Sort(vec_MyDominated,SZ);
		ALLDir.clear();
		int UnitSZ=vec_Unit.size();
		int sum=0;
		for (int i=0;i<UnitSZ;i++)
		{
			for (int j=0;j<vec_Unit[i].size();j++)
			{
				ALLDir.push_back(vec_Unit[i][j].Dir);
			}	
		}
		gene++;
	} while (gene<200);
	ALLDir.clear();
	//////对最后的数据进行处理/////
	for (int i=0;i<vec_Unit[0].size();i++)
	{
		LastUnit.push_back(vec_Unit[0][i]);
	}
	return  LastUnit;
}


vector<UNIT> COptDir::TournamentSelection(vector<vector<UNIT>> vec_MyDominated,int SZ)
{
	vector<UNIT> mySelect;
	for(int i=0;i<SZ;i++)
	{
		int a=vec_MyDominated.size();
		int i1=(double)rand()/RAND_MAX*(a-1);
		int i2=(double)rand()/RAND_MAX*(a-1);
		int b1=vec_MyDominated[i1].size();
		int b2=vec_MyDominated[i2].size();
		int j1=(double)rand()/RAND_MAX*(b1-1);
		int j2=(double)rand()/RAND_MAX*(b2-1);
		if (vec_MyDominated[i1][j1].level<vec_MyDominated[i2][j2].level)
		{
			mySelect.push_back(vec_MyDominated[i1][j1]);
		}
		else
		{
			if (vec_MyDominated[i1][j1].level==vec_MyDominated[i2][j2].level)
			{
				if (vec_MyDominated[i1][j1].distance<0) mySelect.push_back(vec_MyDominated[i1][j1]);
				else
				{
					if (vec_MyDominated[i2][j2].distance<0) mySelect.push_back(vec_MyDominated[i2][j2]);
					else
					{
						if (vec_MyDominated[i1][j1].distance>vec_MyDominated[i2][j2].distance) mySelect.push_back(vec_MyDominated[i1][j1]);
						else mySelect.push_back(vec_MyDominated[i2][j2]);
					}
				}
			}
			else
			{
				mySelect.push_back(vec_MyDominated[i2][j2]);
			}
		}
	}	
	return mySelect;
}


vector<vector<UNIT>>  COptDir::NSGA_ii_Sort(vector<vector<UNIT>> vec_MyDominated,int SZ)
{
	vector<vector<UNIT>> SortUnit;
	int sz1=vec_MyDominated.size();
	int sum=0;
 	for (int i=0;i<sz1;i++)
	{
		int sz2=vec_MyDominated[i].size();
		sum+=sz2;
		if (sum>SZ)
		{
			sum-=sz2;
			vector<UNIT> lastUint;
			for (int j=0;j<sz2;j++)
			{
				sum++;
				if (sum<=SZ)
				{
					lastUint.push_back(vec_MyDominated[i][j]);
				}
				else
				{
					break;
				}
				
			}
			if (!lastUint.empty()) SortUnit.push_back(lastUint);
			break;
		}
		else
		{
			SortUnit.push_back(vec_MyDominated[i]);
		}
	}
	return SortUnit;
}
vector<UNIT>  COptDir::MOPSO(vec_PFACETTRI m_vecPFacetTri, QProgressBar *MyProgressBar)
{
	///////////保存面片法矢/////////////////////////
	vec_PVECTOR3D m_vecFacetNorm;
	for (int i = 0; i < m_vecPFacetTri.size();i++)
	{
		m_vecFacetNorm.push_back(m_vecPFacetTri[i]->m_PFacetNorm);
	}
	vec_VECTOR2D ALLDir;
	vector<UNIT> LastUnit;
	int SZ = 120;
	////初始化种群////
	ALLDir = RandomDir(SZ);
	int gene = 0;
	vector<double> AllSum;
	//CProgressCtrl myCtrl;//设定进度条
	//	myCtrl.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(rtwindow.Width()/2-150,rtwindow.Height()/2-10,
	//rtwindow.Width()/2+150,rtwindow.Height()/2+10), AfxGetMainWnd(),1);//进度条的参数
	//myCtrl.SetRange(0, 100);
	//int myCtrlPos;
	srand((unsigned)time(NULL));
	vector<double>    vec_Area;
	for (int i = 0; i < m_vecPFacetTri.size(); i++)
	{
		double S = KitTool::GetTriArea(m_vecPFacetTri[i]);
		vec_Area.push_back(S);
	}
	////初始化速度///
	double v1max = PI / 90;
	double v2max = PI / 180;
	vec_VECTOR2D vec_pBest;/////个体最优
	VECTOR2D    gBest;////全局最优
	vec_VECTOR2D  vec_Velocity;//
	vector<UNIT>  ElitePopulation;////精英种群
	for (int i = 0; i < SZ; i++)
	{
		VECTOR2D  v;
		v.dx = (double)rand() / RAND_MAX*v1max;
		v.dy = (double)rand() / RAND_MAX*v2max;
		vec_Velocity.push_back(v);
		vec_pBest.push_back(ALLDir[i]);
	}
	vector<UNIT> Bestnums = non_inferior(ALLDir, m_vecFacetNorm, vec_Area);
	do
	{
		MyProgressBar->setValue(gene);
		//myCtrlPos=gene/2;
		//myCtrl.SetPos(myCtrlPos);
		gBest = MOPSO_Select_gbest(ALLDir, Bestnums, ElitePopulation, m_vecFacetNorm, vec_Area);////包含精英种群的维护更新和全局最优的选取
		ALLDir = MOPSO_upDate(gBest, ALLDir, vec_Velocity, v1max, v2max, vec_pBest);
		MOPSO_UpdatePbest(m_vecFacetNorm, ALLDir, vec_Area, vec_pBest);
		Bestnums = non_inferior(ALLDir, m_vecFacetNorm, vec_Area);
		gene++;
	} while (gene < 200);
	//LastUnit=non_dominatedSet(ElitePopulation,pSTL->m_vecFacetNorm,vec_Area)[0];
	MyProgressBar->setValue(0);
	return  ElitePopulation;
}

vector<UNIT>  COptDir::MOPSO(STLModell *pSTL,QProgressBar *MyProgressBar)
{
	vec_VECTOR2D ALLDir;
	vector<UNIT> LastUnit;
	int SZ=120;
	////初始化种群////
	ALLDir=RandomDir(SZ);
	int gene=0;
	vector<double> AllSum;
	//CProgressCtrl myCtrl;//设定进度条
//	myCtrl.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH, CRect(rtwindow.Width()/2-150,rtwindow.Height()/2-10,
		//rtwindow.Width()/2+150,rtwindow.Height()/2+10), AfxGetMainWnd(),1);//进度条的参数
	//myCtrl.SetRange(0, 100);
	//int myCtrlPos;
	srand((unsigned)time(NULL));
	vector<double>    vec_Area;
	for(int i=0;i<pSTL->m_vecPFacetTri.size();i++)
	{
		double S=KitTool::GetTriArea(pSTL->m_vecPFacetTri[i]);
		vec_Area.push_back(S);
	}
	////初始化速度///
	double v1max=PI/90;
	double v2max=PI/180;
	vec_VECTOR2D vec_pBest;/////个体最优
	VECTOR2D    gBest;////全局最优
	vec_VECTOR2D  vec_Velocity;//
	vector<UNIT>  ElitePopulation;////精英种群
	for (int i=0;i<SZ;i++)
	{
		VECTOR2D  v;
		v.dx=(double)rand()/RAND_MAX*v1max;
		v.dy=(double)rand()/RAND_MAX*v2max;
		vec_Velocity.push_back(v);
		vec_pBest.push_back(ALLDir[i]);
	}
	vector<UNIT> Bestnums=non_inferior(ALLDir,pSTL->m_vecFacetNorm,vec_Area);
	do 
	{
		MyProgressBar->setValue(gene);
		//myCtrlPos=gene/2;
		//myCtrl.SetPos(myCtrlPos);
		gBest = MOPSO_Select_gbest(ALLDir, Bestnums, ElitePopulation, pSTL->m_vecFacetNorm, vec_Area);////包含精英种群的维护更新和全局最优的选取
		ALLDir=MOPSO_upDate(gBest,ALLDir, vec_Velocity,v1max,v2max,vec_pBest);
		MOPSO_UpdatePbest(pSTL->m_vecFacetNorm, ALLDir, vec_Area, vec_pBest);
		Bestnums = non_inferior(ALLDir, pSTL->m_vecFacetNorm, vec_Area);
		gene++;
	} while (gene<200);
	//LastUnit=non_dominatedSet(ElitePopulation,pSTL->m_vecFacetNorm,vec_Area)[0];
	MyProgressBar->setValue(0);
	return  ElitePopulation;
}


void  COptDir::MOPSO_UpdatePbest(vec_PVECTOR3D m_vecPFacetNorm,vec_VECTOR2D AllDir,vector<double>vec_Area,vec_VECTOR2D &vec_pBest)
{
	for (int i=0;i<AllDir.size();i++)
	{
		if (SE(m_vecPFacetNorm,AllDir[i],vec_Area)<SE(m_vecPFacetNorm,vec_pBest[i],vec_Area)
			&& SA(m_vecPFacetNorm,AllDir[i],vec_Area)<SA(m_vecPFacetNorm,vec_pBest[i],vec_Area))
		{
			vec_pBest[i]=AllDir[i];
		}
		else
		{
			if (!(SE(m_vecPFacetNorm,AllDir[i],vec_Area)>SE(m_vecPFacetNorm,vec_pBest[i],vec_Area)
				&& SA(m_vecPFacetNorm,AllDir[i],vec_Area)>SA(m_vecPFacetNorm,vec_pBest[i],vec_Area)))
			{
				if (rand()/RAND_MAX*2>1)
				{
					vec_pBest[i]=AllDir[i];
				}
			}
		}
	}
}


vec_VECTOR2D COptDir::MOPSO_upDate(VECTOR2D gBest,vec_VECTOR2D ALLDir,vec_VECTOR2D  vec_Velocity,
								double &v1max,double & v2max,vec_VECTOR2D &vec_pBest)
{

	double C1=2;
	double C2=2;
	double w=0.4;
	/////粒子更新////
	VECTOR2D newvelocity;
	for (int i=0;i<ALLDir.size();i++)
	{
		double  r1=(double)rand()/RAND_MAX;
		double  r2=(double)rand()/RAND_MAX;
		double  r3=(double)rand()/RAND_MAX;
		double  r4=(double)rand()/RAND_MAX;
		newvelocity.dx=w*vec_Velocity[i].dx+C1*r1*(vec_pBest[i].dx-ALLDir[i].dx)+
			C2*r2*(gBest.dx-ALLDir[i].dx);
		if (newvelocity.dx>v1max)  newvelocity.dx=v1max;
		if (newvelocity.dx<-v1max)  newvelocity.dx=-v1max;

		newvelocity.dy=w*vec_Velocity[i].dy+C1*r3*(vec_pBest[i].dy-ALLDir[i].dy)+
			C2*r4*(gBest.dy-ALLDir[i].dy);  
		if (newvelocity.dy>v2max)  newvelocity.dy=v2max;
		if (newvelocity.dx<v2max)  newvelocity.dx=-v2max;

		ALLDir[i]+=newvelocity;
	}
	return ALLDir;
}



 

vector<UNIT> COptDir::non_inferior(vec_VECTOR2D MyDirS,vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area)
{
	///////////加载目标函数
	vector<vector<UNIT>>  my_Dominated;
	vector<pUNIT> vec_pUnit;
	int SZ=MyDirS.size();
	for (int i=0;i<SZ;i++)
	{
		pUNIT myUnit=new UNIT;
		myUnit->Dir=MyDirS[i];
		myUnit->f1Sum=SE(m_vecPFacetNorm,MyDirS[i],vec_Area);
		myUnit->f2Sum=SA(m_vecPFacetNorm,MyDirS[i],vec_Area);
		vec_pUnit.push_back(myUnit);

	}
	////////非支配集合/////
	for(int i=0; i <SZ-1;i++)
	{
		for(int j=i+1;j<SZ;j++)
		{
			if (vec_pUnit[i]->f1Sum<vec_pUnit[j]->f1Sum)
			{
				if (vec_pUnit[i]->f2Sum<=vec_pUnit[j]->f2Sum)
				{
					vec_pUnit[i]->S.push_back(vec_pUnit[j]);
					vec_pUnit[j]->n++;
				}
			}
			else
			{
				if (vec_pUnit[i]->f1Sum==vec_pUnit[j]->f1Sum)
				{
					if (vec_pUnit[i]->f2Sum<vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[i]->S.push_back(vec_pUnit[j]);
						vec_pUnit[j]->n++;
					}
					else
					{
						if (vec_pUnit[i]->f2Sum>vec_pUnit[j]->f2Sum)
						{
							vec_pUnit[j]->S.push_back(vec_pUnit[i]);
							vec_pUnit[i]->n++;
						}
					}
				}
				else
				{
					if (vec_pUnit[i]->f2Sum>vec_pUnit[j]->f2Sum)
					{
						vec_pUnit[j]->S.push_back(vec_pUnit[i]);
						vec_pUnit[i]->n++;
					}
				}
			}		
		}
	}
	set<pUNIT>  set_Unit;/////用于查找重复
	pair<set<pUNIT>::iterator,bool>  pairS;/////用于查找重复
	vector<pUNIT> vec_oper=vec_pUnit;
	set_Unit.clear();
    int Level=1;
	vector<pUNIT> vec_temp;
	vector<UNIT> vec_dominated;
	int vecSz=vec_oper.size();
	//vector<int>  vec_int;
	for (int i=0;i<vecSz;i++)
	{
		if (vec_oper[i]->n==0)
		{				
			/////赋支配等级
			vec_oper[i]->level=Level;
			vec_dominated.push_back(*vec_oper[i]);
			//vec_int.push_back(i);
		}
	}
	//Distance(vec_dominated);/////计算拥挤距离
	//my_Dominated.push_back(vec_dominated);
	vec_oper=vec_temp;
	Level++;
	/////析构释放内存/////
	for (int i=0;i<SZ;i++)
	{
		delete vec_pUnit[i];
	}
	return vec_dominated;
}




VECTOR2D  COptDir::MOPSO_Select_gbest(vec_VECTOR2D ALLDir,vector<UNIT> Bestnums,vector<UNIT>  &ElitePopulation,
									vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area)
{

	for (int i=0;i<Bestnums.size();i++)
	{
		bool  ifPush=true;
		for (int j=0;j<ElitePopulation.size();j++)
		{
			if (abs(ALLDir[i].dx-ElitePopulation[j].Dir.dx)<NC_ZERO&&
				abs(ALLDir[i].dy-ElitePopulation[j].Dir.dy)<NC_ZERO)
			{
				ifPush=false;
				break;
			}
		}
		if (ifPush==true)
		{
			ElitePopulation.push_back(Bestnums[i]);
		}
	}
	/////精英的更新
	vec_VECTOR2D  tempDir;
	for (int i=0;i<ElitePopulation.size();i++)
	{
		tempDir.push_back(ElitePopulation[i].Dir);
	}

	vector<UNIT>  UpdateElite=non_dominatedSet(tempDir,m_vecPFacetNorm,vec_Area)[0];
	///////精英的维护
	if (UpdateElite.size()>10)
	{
		for (int i=0;i<UpdateElite.size()-1;i++)
		{
			for (int j=0;j<UpdateElite.size()-1-i;j++)
			{
				if (UpdateElite[j].distance>UpdateElite[j+1].distance)
				{
					UNIT temp=UpdateElite[j];
					UpdateElite[j]=UpdateElite[j+1];
					UpdateElite[j+1]=temp;
				}
			}
		}
		do 
		{
			UpdateElite.pop_back();
		} while (UpdateElite.size()>10);
	}
	ElitePopulation.clear();
	for (int i=0;i<UpdateElite.size();i++)
	{
		ElitePopulation.push_back(UpdateElite[i]);
	}
	///////选择全局最优
	int EliteSZ=UpdateElite.size();
	int sum=0;
	for (int i=1;i<=EliteSZ;i++)
	{
		sum+=i;
	}
	double rand_i=(double)rand()/RAND_MAX*sum;
	sum=0;
	for (int i=0;i<EliteSZ;i++)
	{
		sum+=(EliteSZ-i);
		if (sum>rand_i)
		{
			return ElitePopulation[i].Dir;
		}
	}
}



CGlobal::CGlobal(void)
{



}


CGlobal::~CGlobal(void)
{

}



int CGlobal::m_myCtrlSet = 0;
