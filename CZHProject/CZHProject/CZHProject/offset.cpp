#include "offset.h"

 Coffset::Coffset(void)
{
  
}
 Coffset::~Coffset(void)
 {

 }

 void Coffset::pretreatment(vec_PPOLYPOLYGON m_vecPPolyPolygons)
 {
	 int sz=m_vecPPolyPolygons.size();
	 for (int z=0;z<sz;z++)
	 {
		 int classfiy=1;
		 vec_PPOLYGON vec_poly1;
		 vec_PPOLYGON vec_poly2;
		 int csz=m_vecPPolyPolygons[z]->m_vecPPolygons.size();
		 for (int i=0;i<csz;i++)
		 {
			 PPOLYGON poly=m_vecPPolyPolygons[z]->m_vecPPolygons[i];
			 bool b1=KitTool::judeorientation(poly);
			 if (b1==true)
			 {
				 poly->m_IoriClass=classfiy;
				 vec_poly1.push_back(poly);///////外部轮廓
				 classfiy++;
			 }
			 else
			 {
				 vec_poly2.push_back(poly);////内部轮廓
			 }
		 }
		 m_vecPPolyPolygons[z]->maxclassfiy=classfiy-1;//////最大原始分类个数
		 for (int i=0;i<vec_poly2.size();i++)
		 {
			 vec_PPOLYGON vec_poly3;
			 int num=0;
			 for (int j=0;j<vec_poly1.size();j++)
			 {
				 bool ifin=KitTool::pointInpoly(vec_poly2[i]->m_vecPnts[0],vec_poly1[j]);
				 if (ifin==true)
				 {
					 num++;
					 vec_poly3.push_back(vec_poly1[j]);//////该内部轮廓属于那些外部轮廓
				 }
			 }
			 if (num==1)
			 {
				 vec_poly2[i]->m_IoriClass=vec_poly3[0]->m_IoriClass;
			 }
			 else
			 {
				 if (vec_poly3.size()>0)
				 {
					 double area = KitTool::GetPolyArea(vec_poly3[0]);
					 vec_poly2[i]->m_IoriClass = vec_poly3[0]->m_IoriClass;
					 for (int j = 1; j < vec_poly3.size(); j++)
					 {
						 double areaT = KitTool::GetPolyArea(vec_poly3[j]);
						 if (area > areaT)
						 {
							 area = areaT;
							 vec_poly2[i]->m_IoriClass = vec_poly3[j]->m_IoriClass;
						 }
					 }
				 }
			 }
		 }
	 }
 }
 void  Coffset::OffsetCircle1(double FillLineWidth, VECTOR3D vec_Dir, PPOLYGON ploy, PPOLYPOLYGON &m_vecFillP)
 {
	 VECTOR3D  DirZ(0, 0, 1);
	 KitTool::RotateOneRingPolygon(ploy, vec_Dir, DirZ);
	
	 POINT3D Midpoint;
	 int nct = ploy->m_vecPnts.size();
	 for (int i = 0; i < nct; i++)
	 {
		 Midpoint.x += ploy->m_vecPnts[i].x;
		 Midpoint.y += ploy->m_vecPnts[i].y;
		 Midpoint.z += ploy->m_vecPnts[i].z;
	 }
	 Midpoint.x /= nct; Midpoint.y /= nct; Midpoint.z /= nct;
	 double Dist = _DistOf(Midpoint, ploy->m_vecPnts[0]);


	 PPOLYPOLYGON m_vec;
	 m_vec = new POLYPOLYGON;
	 m_vec->m_vecPPolygons.push_back(ploy);

	 bool bifinciseTemp = true;
	 bool  bifincise = false;//定义一个布尔型变量
	 for (int i = 0; i < m_vec->m_vecPPolygons.size(); i++)
	 {
		 if (!bifincise)
		 {
			 if (!KitTool::judeorientation(ploy)) bifinciseTemp = false;//判断旋向逆时针为bool为正
		 }

		 PPOLYGON  pPoly = offsetpiont(FillLineWidth, ploy);//对点进行偏置：

		 PPOLYPOLYGON pPolyPoly;
		 pPolyPoly = new POLYPOLYGON;

		 selfing(ploy, pPoly, pPolyPoly);//原始的点//偏置后的点//去除自交后的点

		 PPOLYGON    poly1;                     //定义一个新的容器
		 poly1 = new POLYGON;

		 poly1 = pPolyPoly->m_vecPPolygons[0];


		 m_vec->m_vecPPolygons.push_back(poly1);

		 m_vecFillP = m_vec;
		 int tt, kkk;
		 tt = m_vecFillP->m_vecPPolygons.size();

		 kkk = Dist / FillLineWidth;

		 if (tt > kkk + 1)
		 {
			 break;
		 }
	 }
	 KitTool::RotateTwoRingPolygon(m_vecFillP, DirZ, vec_Dir);
 }
 void Coffset::GetOffset1(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth)
 {
	 VECTOR3D DirZ(0, 0, 1);
	 KitTool::RotatePolygon(m_vecPPolyPolygons, m_VecDir, DirZ);
	
	 int m_PPPsz = m_vecPPolyPolygons.size();
	 for (int i = 0; i < m_PPPsz; i++)
	 {
		 PPOLYPOLYGON  afteroffset;
		 afteroffset = new POLYPOLYGON;
		 for (int j = 0; j < m_vecPPolyPolygons[i]->m_vecPPolygons.size(); j++)
		 {
			 PPOLYGON poly = new POLYGON;
			 *poly = *m_vecPPolyPolygons[i]->m_vecPPolygons[j];
			 afteroffset->m_vecPPolygons.push_back(poly);
			 afteroffset->maxclassfiy = m_vecPPolyPolygons[i]->maxclassfiy;
		 }
		 int m = m_vecPPolyPolygons[i]->m_vecPPolygons.size();//原始偏置曲线段数量
		 int originalNum = m;
		 bool  bifincise = false;
		 //for (int z=0;z<11;z++)//调试专用！
		 do //在原来的偏置曲线容器一直往里加偏置曲线直到结束
		 {
			 int m_ppsz = afteroffset->m_vecPPolygons.size();
			 int u = 0;
			 PPOLYPOLYGON pPolyPoly;
			 pPolyPoly = new POLYPOLYGON;
			 bool bifinciseTemp = true;
			 for (int j = 0; j < m; j++)//只取偏置容器中初始偏置数量的曲线进行下次偏置
			 {
				 if (!bifincise)
				 {
					 if (!KitTool::judeorientation(afteroffset->m_vecPPolygons[m_ppsz - 1 - j])) bifinciseTemp = false;
				 }
				 PPOLYGON  pPoly = offsetpiont(FillLineWidth, afteroffset->m_vecPPolygons[m_ppsz - 1 - j]);
				 POLYGON  ploy = *afteroffset->m_vecPPolygons[m_ppsz - 1 - j];
				 int n = selfing(afteroffset->m_vecPPolygons[m_ppsz - 1 - j], pPoly, pPolyPoly);
				 offsetClassify(originalNum, n, ploy, pPolyPoly, 0.5);
				 //afteroffset->m_vecPPolygons.push_back(pPoly);//调试用 	
				 u = u + n;
			 }
			 if (bifinciseTemp)  bifincise = true;
			 //控制偏置曲线后的下次需要偏置的个数
			 int inciseNum;
			 if (bifincise)
			 {
				 afteroffset->m_vecPPolygons.insert(afteroffset->m_vecPPolygons.end(),
					 pPolyPoly->m_vecPPolygons.begin(), pPolyPoly->m_vecPPolygons.end());
				 inciseNum = 0;
			 }
			 else inciseNum = incise(afteroffset, pPolyPoly, originalNum);
			 m = m + u + inciseNum;
			 ifdeleOffset(m, afteroffset, FillLineWidth);
		 } while (m != 0);
		 offsetSort(afteroffset);
		 m_vecFillP.push_back(afteroffset);
	 }
	 KitTool::RotatePolygon(m_vecFillP, DirZ, m_VecDir);
 }
 void Coffset::OffsetCircle(double degree, VECTOR3D vec_Dir, PPOLYGON ploy, PPOLYPOLYGON vec_poly)
 {
	 VECTOR3D DirZ(0, 0, 1);
	 KitTool::RotateOneRingPolygon(ploy, vec_Dir, DirZ);
	 int m = 1;//原始偏置曲线段数量
	 int originalNum = m;
	 bool  bifincise = false;
	 vec_poly->m_vecPPolygons.push_back(ploy);
	
	 do //在原来的偏置曲线容器一直往里加偏置曲线直到结束
	 {
		 int m_ppsz = vec_poly->m_vecPPolygons.size();
		 int u = 0;
		 PPOLYPOLYGON pPolyPoly;
		 pPolyPoly = new POLYPOLYGON;
		 bool bifinciseTemp = true;
		 for (int j = 0; j < m; j++)//只取偏置容器中初始偏置数量的曲线进行下次偏置
		 {
			 if (!bifincise)
			 {
				 if (!KitTool::judeorientation(vec_poly->m_vecPPolygons[m_ppsz - 1 - j])) bifinciseTemp = false;
			 }
			 PPOLYGON  pPoly = offsetpiont(degree, vec_poly->m_vecPPolygons[m_ppsz - 1 - j]);
			 POLYGON  prloy = *vec_poly->m_vecPPolygons[m_ppsz - 1 - j];
			 int n = selfing(vec_poly->m_vecPPolygons[m_ppsz - 1 - j], pPoly, pPolyPoly);
			 offsetClassify(originalNum, n, prloy, pPolyPoly, 0.5);
			 u = u + n;
		 }
		 if (bifinciseTemp)  bifincise = true;
		 //控制偏置曲线后的下次需要偏置的个数
		 int inciseNum;
		 if (bifincise)
		 {
			 vec_poly->m_vecPPolygons.insert(vec_poly->m_vecPPolygons.end(),
				 pPolyPoly->m_vecPPolygons.begin(), pPolyPoly->m_vecPPolygons.end());
			 inciseNum = 0;
		 }
		 else inciseNum = incise(vec_poly, pPolyPoly, originalNum);
		 m = m + u + inciseNum;
		 ifdeleOffset(m, vec_poly, degree);
	 } while (m != 0);
	 //offsetSort(vec_poly);//将所有环由里向外排布
	 KitTool::RotateTwoRingPolygon(vec_poly, DirZ, vec_Dir);
 }
 void Coffset::GetOffset(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth)
{
    VECTOR3D DirZ(0,0,1);
	KitTool::RotatePolygon(m_vecPPolyPolygons,m_VecDir,DirZ);
	//pretreatment(m_vecPPolyPolygons);
	int m_PPPsz =m_vecPPolyPolygons.size();
	for(int i=0;i<m_PPPsz;i++)
	{
		PPOLYPOLYGON  afteroffset;
		afteroffset =new POLYPOLYGON;
		for (int j=0;j<m_vecPPolyPolygons[i]->m_vecPPolygons.size();j++)
		{
			PPOLYGON poly=new POLYGON;
			*poly=*m_vecPPolyPolygons[i]->m_vecPPolygons[j];
			afteroffset->m_vecPPolygons.push_back(poly);
			afteroffset->maxclassfiy=m_vecPPolyPolygons[i]->maxclassfiy;
		}
		int m=m_vecPPolyPolygons[i]->m_vecPPolygons.size();//原始偏置曲线段数量
		int originalNum=m;
		bool  bifincise=false;
		//for (int z=0;z<11;z++)//调试专用！
		do //在原来的偏置曲线容器一直往里加偏置曲线直到结束
		{ 
			int m_ppsz=afteroffset->m_vecPPolygons.size();
			int u=0;
			PPOLYPOLYGON pPolyPoly;
			pPolyPoly = new POLYPOLYGON;
			bool bifinciseTemp=true;
			for (int j =0;j<m;j++)//只取偏置容器中初始偏置数量的曲线进行下次偏置
			{
				if (!bifincise)
				{
					if (!KitTool::judeorientation(afteroffset->m_vecPPolygons[m_ppsz-1-j])) bifinciseTemp=false;
				}
				PPOLYGON  pPoly  = offsetpiont(FillLineWidth,afteroffset->m_vecPPolygons[m_ppsz-1-j]);
				POLYGON  ploy =  *afteroffset->m_vecPPolygons[m_ppsz-1-j];
				int n = selfing(afteroffset->m_vecPPolygons[m_ppsz-1-j],pPoly,pPolyPoly);
				offsetClassify(originalNum,n, ploy,pPolyPoly,0.5);
				//afteroffset->m_vecPPolygons.push_back(pPoly);//调试用 	
				u=u+n;
			}
			if (bifinciseTemp)  bifincise=true;
			//控制偏置曲线后的下次需要偏置的个数
			int inciseNum ;
			if (bifincise)
			{
				afteroffset->m_vecPPolygons.insert(afteroffset->m_vecPPolygons.end(),
					pPolyPoly->m_vecPPolygons.begin(),pPolyPoly->m_vecPPolygons.end());
				inciseNum=0;
			}
			else inciseNum = incise(afteroffset,pPolyPoly,originalNum);
			m=m+u+inciseNum;
			ifdeleOffset(m,afteroffset,FillLineWidth);
		} while (m!=0);
		offsetSort(afteroffset);
		m_vecFillP.push_back(afteroffset);
	}
	KitTool::RotatePolygon(m_vecFillP,DirZ,m_VecDir);
}

void Coffset::GetOffset(vec_PPOLYPOLYGON	m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON & m_vecFillP,
	double FillLineWidth,int degree,vec_PPOLYPOLYGON &vec_lastoffset)
{
	VECTOR3D DirZ(0,0,1);
	KitTool::RotatePolygon(m_vecPPolyPolygons,m_VecDir,DirZ);
	int m_PPPsz =m_vecPPolyPolygons.size();
	for(int i=0;i<m_PPPsz;i++)
	{
		PPOLYPOLYGON  afteroffset;
		afteroffset =new POLYPOLYGON;
		for (int j=0;j<m_vecPPolyPolygons[i]->m_vecPPolygons.size();j++)
		{
			PPOLYGON poly=new POLYGON;
			*poly=*m_vecPPolyPolygons[i]->m_vecPPolygons[j];
			afteroffset->m_vecPPolygons.push_back(poly);

		}
		//afteroffset->m_vecPPolygons=(pSlice->m_vecPPolyPolygons[i]->m_vecPPolygons);
		int m=m_vecPPolyPolygons[i]->m_vecPPolygons.size();//原始偏置曲线段数量
		int originalNum=m;
		for (int z=0;z<degree;z++)
		{ 
			int m_ppsz=afteroffset->m_vecPPolygons.size();
			int n=0;
			int u=0;
			PPOLYPOLYGON pPolyPoly;
			pPolyPoly = new POLYPOLYGON;
			for (int j =0;j<m;j++)//只取偏置容器中初始偏置数量的曲线进行下次偏置
			{
				PPOLYGON  pPoly  = offsetpiont(FillLineWidth,afteroffset->m_vecPPolygons[m_ppsz-1-j]);
				POLYGON  ploy =  *afteroffset->m_vecPPolygons[m_ppsz-1-j];
				n = selfing(afteroffset->m_vecPPolygons[m_ppsz-1-j],pPoly,pPolyPoly);	
				offsetClassify(originalNum,n, ploy,pPolyPoly,0.5);
				//afteroffset->m_vecPPolygons.push_back(pPoly);//调试用 	
				u=u+n;
			}
			//控制偏置曲线后的下次需要偏置的个数
			int inciseNum = incise(afteroffset,pPolyPoly,originalNum);
			m=m+u+inciseNum;
		}
		PPOLYPOLYGON lastoffset;
		lastoffset =new POLYPOLYGON;
		for (int j=0;j<m;j++)
		{
			int m_ppsz=afteroffset->m_vecPPolygons.size();
			PPOLYGON poly=new POLYGON;
			*poly=*afteroffset->m_vecPPolygons[m_ppsz-1-j];
			lastoffset->m_vecPPolygons.push_back(poly);
		}
		for (int j=0;j<m;j++)
		{
			afteroffset->m_vecPPolygons.pop_back();
		}
		vec_lastoffset.push_back(lastoffset);
		//offsetSort(afteroffset);
		m_vecFillP.push_back(afteroffset);
	}
	KitTool::RotatePolygon(m_vecFillP,DirZ,m_VecDir);
}


void Coffset::ifdeleOffset(int &m,PPOLYPOLYGON afteroffset,double FillLineWidth)
{

	for (int i=m-1;i>-1;i--)
	{
		int ppsz=afteroffset->m_vecPPolygons.size();
		afteroffset->m_vecPPolygons[ppsz-1-i]->UpdateBox();
		double b1=afteroffset->m_vecPPolygons[ppsz-1-i]->m_BoxPolygon.x1-
			afteroffset->m_vecPPolygons[ppsz-1-i]->m_BoxPolygon.x0;
		double b2=afteroffset->m_vecPPolygons[ppsz-1-i]->m_BoxPolygon.y1-
			afteroffset->m_vecPPolygons[ppsz-1-i]->m_BoxPolygon.y0;
		if ((b1<2*FillLineWidth)||(b2<2*FillLineWidth))
		{
			POLYGON  temp;
			temp=*afteroffset->m_vecPPolygons[ppsz-m];
			*afteroffset->m_vecPPolygons[ppsz-m]=*afteroffset->m_vecPPolygons[ppsz-1-i];
			*afteroffset->m_vecPPolygons[ppsz-1-i]=temp;
			m--;
			i=m;
		}
	}
}

void  Coffset::offsetSort(PPOLYPOLYGON ployploy)//排序从小到大，并且每个数字相连
{
	POLYGON  TempPloy; 
	int sz = ployploy->m_vecPPolygons.size();
	for(int i=0;i<sz-1;i++) //冒泡升序
	{ 
		for (int j= 0;j<sz-1-i;j++) 
		{
			if (ployploy->m_vecPPolygons[j]->m_Iclassify>ployploy->m_vecPPolygons[j+1]->m_Iclassify) 
			{ 
				TempPloy=*ployploy->m_vecPPolygons[j+1]; 
				*ployploy->m_vecPPolygons[j+1]=*ployploy->m_vecPPolygons[j]; 
				*ployploy->m_vecPPolygons[j]=TempPloy;
			} 
		}	
	} 

	for (int i =0 ;i<sz-1;i++)
	{
		if (i==0)
		{
			if (ployploy->m_vecPPolygons[0]->m_Iclassify!=1)
			{
				int Num=ployploy->m_vecPPolygons[0]->m_Iclassify-1;
				for (int j =0 ;j<sz;j++)
				{
					ployploy->m_vecPPolygons[j]->m_Iclassify=ployploy->m_vecPPolygons[j]->m_Iclassify-Num;
				}

			}

		} 
		else
		{
			int Num =ployploy->m_vecPPolygons[i+1]->m_Iclassify-ployploy->m_vecPPolygons[i]->m_Iclassify;
			if (Num!=0)
			{
				if (Num!=1)
				{
					int chanNum =Num-1;
					for (int j =i+1 ;j<sz;j++)
					{
						ployploy->m_vecPPolygons[j]->m_Iclassify=ployploy->m_vecPPolygons[j]->m_Iclassify-chanNum;
					}
				}
			} 
		}
	}
	///////后加入的排序让偏置环由外到内
	PPOLYPOLYGON tempPoly1=new POLYPOLYGON;
	PPOLYPOLYGON tempPloy2=new POLYPOLYGON;
	for (int i=0;i<ployploy->m_vecPPolygons.size();i++)
	{

		bool b1=KitTool::judeorientation(ployploy->m_vecPPolygons[i]);
		if (b1==true)
		{
			tempPoly1->m_vecPPolygons.push_back(ployploy->m_vecPPolygons[i]);
			tempPoly1->m_vecPPolygons.back()->m_Iclassify=ployploy->m_vecPPolygons[i]->m_Iclassify;
		}
		else
		{
			reverse(ployploy->m_vecPPolygons[i]->m_vecPnts.begin(),ployploy->m_vecPPolygons[i]->m_vecPnts.end());///把所有顺序都改为逆时针
			tempPloy2->m_vecPPolygons.push_back(ployploy->m_vecPPolygons[i]);
			tempPloy2->m_vecPPolygons.back()->m_Iclassify=ployploy->m_vecPPolygons[i]->m_Iclassify;
		}
	}
	reverse(tempPloy2->m_vecPPolygons.begin(),tempPloy2->m_vecPPolygons.end());
	ployploy->m_vecPPolygons.clear();
    ployploy->m_vecPPolygons.insert(ployploy->m_vecPPolygons.begin(),tempPoly1->m_vecPPolygons.begin(),tempPoly1->m_vecPPolygons.end());
	ployploy->m_vecPPolygons.insert(ployploy->m_vecPPolygons.end(),tempPloy2->m_vecPPolygons.begin(),tempPloy2->m_vecPPolygons.end());
	/////重新分配
	int classfiy=1;
	int  num =ployploy->m_vecPPolygons[0]->m_Iclassify;
	for (int i=0;i<ployploy->m_vecPPolygons.size();i++)
	{
		if (ployploy->m_vecPPolygons[i]->m_Iclassify==num)
		{
			ployploy->m_vecPPolygons[i]->m_Iclassify=classfiy;
		}
		else
		{
			classfiy++;
			num=ployploy->m_vecPPolygons[i]->m_Iclassify;
			ployploy->m_vecPPolygons[i]->m_Iclassify=classfiy;
		}
	}
}

void Coffset::offsetClassify(int &m ,int n , POLYGON beoffsetpoly ,PPOLYPOLYGON ployploy,double threshold)//对偏置曲线进行分类
{
		if (n==0)
		{
			if (threshold>0)
			{
				double ratio =KitTool::GetPerimeter(*ployploy->m_vecPPolygons.back())/KitTool::GetPerimeter(beoffsetpoly);
				if (ratio>(1-threshold)&&ratio<(1+threshold))
				{
					ployploy->m_vecPPolygons.back()->m_Iclassify=beoffsetpoly.m_Iclassify;
				} 
				else
				{
					ployploy->m_vecPPolygons.back()->m_Iclassify=m+1;
					m++;
				}	
			}
			else
			{
				ployploy->m_vecPPolygons.back()->m_Iclassify=beoffsetpoly.m_Iclassify;
			}
		} 
		else
		{
			for(int i = 1;i<(n+2);i++)
			{
				int sz= ployploy->m_vecPPolygons.size();
				ployploy->m_vecPPolygons[sz-i]->m_Iclassify=m+1;
				m++;
		
			}
		} 
}

bool  Coffset::ifstop(PPOLYPOLYGON ploy,double d)//判断是否停止函数
{
	int m= ploy->m_vecPPolygons.size();
	for (int i=0;i<m;i++)
	{
		ploy->m_vecPPolygons[i]->UpdateBox();
		if ((ploy->m_vecPPolygons[i]->m_BoxPolygon.x1-ploy->m_vecPPolygons[i]->m_BoxPolygon.x0<d)|
			  (ploy->m_vecPPolygons[i]->m_BoxPolygon.y1-ploy->m_vecPPolygons[i]->m_BoxPolygon.y0<d))
		{
			return true;
			break;
		}
		else
		{
			return false;
		}
	}
}

int Coffset::incise(PPOLYPOLYGON afteroffset, PPOLYPOLYGON ployploy,int &originalNum)//处理相互干涉
{
	int m=0;
	int n=0;
	for (int i = 0;i<ployploy->m_vecPPolygons.size()-1;i++)
	{
	
		for (int j = i+1;j<ployploy->m_vecPPolygons.size();j++)
		{
			PPOLYPOLYGON Newpolypoly;
			Newpolypoly = new POLYPOLYGON;
			if (IfIncise(ployploy->m_vecPPolygons[i],ployploy->m_vecPPolygons[j], Newpolypoly,false))
			{
				int NewploySZ=Newpolypoly->m_vecPPolygons.size();//给新生成的多边形进行新的分类。
				for (int z = 0;z<NewploySZ;z++)
				{
						
					Newpolypoly->m_vecPPolygons[z]->m_Iclassify=originalNum+1;
					originalNum++;

				}
				ployploy->m_vecPPolygons.erase(ployploy->m_vecPPolygons.begin()+j);
				ployploy->m_vecPPolygons.erase(ployploy->m_vecPPolygons.begin()+i);

				for (int u=0;u<Newpolypoly->m_vecPPolygons.size();u++)
				{
					ployploy->m_vecPPolygons.insert(ployploy->m_vecPPolygons.begin()+i+u,Newpolypoly->m_vecPPolygons[u]);
				}
				i=i-1;////重新开始循环
				n=Newpolypoly->m_vecPPolygons.size()-2;
				break;
			}
			else
			{
				delete Newpolypoly;
			}			
		}
		m=m+n;
		n=0;
	}
	for (int i = 0 ; i<ployploy->m_vecPPolygons.size();i++)
	{
		afteroffset->m_vecPPolygons.push_back(ployploy->m_vecPPolygons[i]);
	}

	return m;
}

bool Coffset::IfIncise(PPOLYGON poly1,PPOLYGON poly2,PPOLYPOLYGON  Newpolypoly ,bool ifOutCutOut)//2条曲线之间的相互干涉
{
	////先用BOX初略判断是否可能相交
	poly1->UpdateBox();
	poly2->UpdateBox();

	if (poly1->m_BoxPolygon.x1>poly2->m_BoxPolygon.x1)
	{
		if (poly1->m_BoxPolygon.x0>poly2->m_BoxPolygon.x1)
			return false;
	}
	else 
	{
		if (poly2->m_BoxPolygon.x0>poly1->m_BoxPolygon.x1)
			return false;
		else
		{
			if (poly1->m_BoxPolygon.y1>poly2->m_BoxPolygon.y1)
			{
			    if (poly1->m_BoxPolygon.y0>poly2->m_BoxPolygon.y1)
				return false;
	        }
			else
			{
				if (poly2->m_BoxPolygon.y0>poly1->m_BoxPolygon.y1)
					return false;
			}
		}
	}

	//确保裁剪顺序
	bool b1,b2;
	b1=KitTool::judeorientation(poly1);
	b2=KitTool::judeorientation(poly2);
	if (b1!=b2)
	{
		if (b1==false)
		{
			
			PPOLYGON poly;
			poly = poly1;
			poly1 =poly2;
			poly2 = poly;
		}
	}
	//////判断是否操作外环切外环的操作
	bool ifCut = false;
	if (ifOutCutOut)
	{
		ifCut =true;
	}
	else
	{
		if (!(b1==true&&b2==true))
		{
			ifCut = true;
		}
	}
	if (ifCut)
	//if (!(b1==true&&b2==true))
	{
		int sz1= poly1->m_vecPnts.size();
		int sz2= poly2->m_vecPnts.size();
		vector<LineInterS>   vec_lineInter;
		////判断交点
		for(int i=0;i<sz1-1;i++)
		{
			for (int j=0;j<sz2-1;j++)
			{

				POINT3D p1,p2,p3,p4;
				p1=poly1->m_vecPnts[i];
				p2=poly1->m_vecPnts[i+1];
				p3=poly2->m_vecPnts[j];
				p4=poly2->m_vecPnts[j+1];
				POINT3D p;
				if (KitTool::lineSegInter(p1,p2,p3,p4,p))
				{
					LineInterS   lineInter;
					VECTOR3D V1= p2-p1;
					VECTOR3D V2 =p4-p3;
					VECTOR3D Vz(0,0,1);
					lineInter.p=p;
					lineInter.line1Num=i;
					lineInter.line2Num=j;
					if (((V1*V2)|Vz)>0)
					{
						lineInter.ifInto=true;
					}
					else
					{
						lineInter.ifInto=false;
					}
					lineInter.beused=false;
					vec_lineInter.push_back(lineInter);
				}
			}
		}

		int m = vec_lineInter.size();	
		///////处理线段重合的交点
		for (int i=0;i<m;i++)
		{
			for (int j=0;j<m;j++)
			{
				if (i!=j)
				{
					POINT3D p1=poly1->m_vecPnts[vec_lineInter[i].line1Num];
					POINT3D p2=poly1->m_vecPnts[vec_lineInter[i].line1Num+1];
					POINT3D p3=poly2->m_vecPnts[vec_lineInter[j].line2Num];
					POINT3D p4=poly2->m_vecPnts[vec_lineInter[j].line2Num+1];
					VECTOR3D v1=p2-p1;
					VECTOR3D v2=p4-p3;
					v1.Normalize();
					v2.Normalize();
					double min_x=min(p3.x,p4.x);
					double min_y=min(p3.y,p4.y);
					double max_x=max(p3.x,p4.x);
					double max_y=max(p3.y,p4.y);
					double Ang=abs(v1|v2);
					if (((1-Ang)<CAD_ZERO))
					{
						if ((p1.x>=min_x&&p1.x<=max_x&&p1.y>=min_y&&p1.y<=max_y)|
							(p2.x>=min_x&&p2.x<=max_x&&p2.y>=min_y&&p2.y<=max_y))
						{
							vec_lineInter.erase(vec_lineInter.begin()+j);
							m--;
							i=0;
							break;
						}
					}
				}
			}
		}
		m=vec_lineInter.size();
		if (m!=0)
		{
			//处理曲线的拼接
			FlattenPoly(poly1,poly2,vec_lineInter,Newpolypoly);
			return true;
		}
	}
	return false;
}

void Coffset::FlattenPoly(PPOLYGON poly1,PPOLYGON poly2,vector<LineInterS> vec_lineInter,PPOLYPOLYGON Newpolypoly)///////曲线拼接
{
	vector<LineInterS>  vec_lineInter2;
	vec_lineInter2=vec_lineInter;
	//冒泡排序
	int m = vec_lineInter.size();	
	for (int i = 0 ;i<m-1;i++)
	{
		for (int j=0;j<m-1-i;j++)
		{
			if (vec_lineInter[j].line1Num>vec_lineInter[j+1].line1Num)
			{
				LineInterS temp = vec_lineInter[j];
				vec_lineInter[j]=vec_lineInter[j+1];
				vec_lineInter[j+1]=temp;
			}
			else
			{
				if (vec_lineInter[j].line1Num==vec_lineInter[j+1].line1Num)
				{
					VECTOR3D v1=vec_lineInter[j].p-poly1->m_vecPnts[vec_lineInter[j].line1Num];
					VECTOR3D v2=vec_lineInter[j+1].p-poly1->m_vecPnts[vec_lineInter[j].line1Num];
					if (v1.GetLength()>v2.GetLength())
					{
						LineInterS temp = vec_lineInter[j];
						vec_lineInter[j]=vec_lineInter[j+1];
						vec_lineInter[j+1]=temp;
					}
				}
			}
		}
	}
	for (int i = 0 ;i<m-1;i++)
	{
		for (int j=0;j<m-1-i;j++)
		{
			if (vec_lineInter2[j].line2Num>vec_lineInter2[j+1].line2Num)
			{
				LineInterS temp = vec_lineInter2[j];
				vec_lineInter2[j]=vec_lineInter2[j+1];
				vec_lineInter2[j+1]=temp;
			}
			else
			{
				if (vec_lineInter2[j].line2Num==vec_lineInter2[j+1].line2Num)
				{
					VECTOR3D v1=vec_lineInter2[j].p-poly2->m_vecPnts[vec_lineInter2[j].line2Num];
					VECTOR3D v2=vec_lineInter2[j+1].p-poly2->m_vecPnts[vec_lineInter2[j].line2Num];
					if (v1.GetLength()>v2.GetLength())
					{
						LineInterS temp = vec_lineInter2[j];
						vec_lineInter2[j]=vec_lineInter2[j+1];
						vec_lineInter2[j+1]=temp;
					}
				}
			}
		}
	}
	for (int i = 0; i<m;i++)
	{
		if (vec_lineInter[i].ifInto==false&&vec_lineInter[i].beused==false)
		{
			int fistNum=i;
			PPOLYGON  poly;
			poly = new POLYGON;
			do 
			{
				int lin1NextNum;
				vec_lineInter[i].beused=true;
				ifPopBack(vec_lineInter[i].p,poly);
				poly->m_vecPnts.push_back(vec_lineInter[i].p);
				if (i==m-1)
				{
					lin1NextNum=0;
				}
				else
				{
					lin1NextNum=i+1;
				}
				vec_lineInter[lin1NextNum].beused=true;

				int line1Num1=vec_lineInter[i].line1Num;
				int line1Num2=vec_lineInter[lin1NextNum].line1Num;
				/////处理
				if (line1Num1==line1Num2)
				{
					VECTOR3D v1=vec_lineInter[lin1NextNum].p-vec_lineInter[i].p;
					VECTOR3D v2=poly1->m_vecPnts[line1Num1+1]-poly1->m_vecPnts[line1Num1];
					v1.Normalize();
					v2.Normalize();
					if (1-(v1|v2)<CAD_ZERO)
					{
						poly->m_vecPnts.push_back(vec_lineInter[lin1NextNum].p);
					}
					else
					{
						ExtractPoint(line1Num1,line1Num2,poly1,poly);
					}
				}
				else
				{
					ExtractPoint(line1Num1,line1Num2,poly1,poly);
				}
				//////寻找曲线2的点
				int lin2NextNum;
				for (int j=0; j<m;j++)
				{
					if ((vec_lineInter2[j].line1Num==vec_lineInter[lin1NextNum].line1Num)&&
						(vec_lineInter2[j].line2Num==vec_lineInter[lin1NextNum].line2Num))
					{
						
						ifPopBack(vec_lineInter2[j].p,poly);
						poly->m_vecPnts.push_back(vec_lineInter2[j].p);
						if (j==m-1)
						{
							lin2NextNum=0;
						}
						else
						{
							lin2NextNum=j+1;
						}
						int line2Num1=vec_lineInter2[j].line2Num;
						int line2Num2=vec_lineInter2[lin2NextNum].line2Num;
						if (line2Num1==line2Num2)
						{
							VECTOR3D v1=vec_lineInter2[lin2NextNum].p-vec_lineInter2[j].p;
							VECTOR3D v2=poly2->m_vecPnts[line2Num1+1]-poly2->m_vecPnts[line2Num1];
							v1.Normalize();
							v2.Normalize();
							if (1-(v1|v2)<CAD_ZERO)
							{
								poly->m_vecPnts.push_back(vec_lineInter2[lin2NextNum].p);
							}
							else
							{
								ExtractPoint(line2Num1,line2Num2,poly2,poly);
							}
						}
						else
						{
							ExtractPoint(line2Num1,line2Num2,poly2,poly);
						}
						break;
					}
				}
				////////返回寻找曲线1
				for (int u=0;u<m;u++)
				{
					if ((vec_lineInter[u].line1Num==vec_lineInter2[lin2NextNum].line1Num)&&
						(vec_lineInter[u].line2Num==vec_lineInter2[lin2NextNum].line2Num))
					{
						i=u;
						break;
					}
				}
			} while (i!=fistNum);
			//////处理曲线末端，直线用一条线段表示////
			if (poly->m_vecPnts[0]!=poly->m_vecPnts.back())
			{
				POINT3D p3=poly->m_vecPnts[1];
				VECTOR3D v1=p3-poly->m_vecPnts[0];
				VECTOR3D v2=poly->m_vecPnts[0]-poly->m_vecPnts.back();
				v1.Normalize();
				v2.Normalize();
				float Ang=abs(v1|v2);
				if ((1-Ang)<CAD_ZERO)
				{
					poly->m_vecPnts.erase(poly->m_vecPnts.begin());
				}
				poly->m_vecPnts.push_back(poly->m_vecPnts[0]);
			}
			else
			{
				poly->m_vecPnts.pop_back();
				POINT3D p3=poly->m_vecPnts[1];
				VECTOR3D v1=p3-poly->m_vecPnts[0];
				VECTOR3D v2=poly->m_vecPnts[0]-poly->m_vecPnts.back();
				v1.Normalize();
				v2.Normalize();
				float Ang=abs(v1|v2);
				if ((1-Ang)<CAD_ZERO)
				{
					poly->m_vecPnts.erase(poly->m_vecPnts.begin());
				}
				poly->m_vecPnts.push_back(poly->m_vecPnts[0]);
			}
			 Newpolypoly->m_vecPPolygons.push_back(poly);
		}
	}
}

void Coffset::ifPopBack(POINT3D p1,PPOLYGON poly)/////////是否删除最后一点
{
	int sz=poly->m_vecPnts.size();
	if(sz>0)
	{
		POINT3D p2=poly->m_vecPnts.back();
		if (abs(p1.x-p2.x)<CAD_ZERO&&abs(p1.y-p2.y)<CAD_ZERO)
		{
			poly->m_vecPnts.pop_back();
		}
		if (sz>1)
		{
			POINT3D p3=poly->m_vecPnts[sz-2];
			VECTOR3D v1=p2-p3;
			VECTOR3D v2=p1-p2;
			v1.Normalize();
			v2.Normalize();
			float Ang=abs(v1|v2);
			if ((1-Ang)<CAD_ZERO)
			{
				poly->m_vecPnts.pop_back();
			}
		}
	}
	
}

void Coffset::ExtractPoint(int Num1,int Num2,PPOLYGON poly1,PPOLYGON poly)////提取点
{
	if (Num1==Num2)
	{
		for (int u=Num1+1;u<poly1->m_vecPnts.size();u++)
		{

			ifPopBack(poly1->m_vecPnts[u],poly);
			poly->m_vecPnts.push_back(poly1->m_vecPnts[u]);

		}
		for (int v=1;v<Num1+1;v++)
		{
			ifPopBack(poly1->m_vecPnts[v],poly);
			poly->m_vecPnts.push_back(poly1->m_vecPnts[v]);
		}
	}
	else
	{
		if (Num1>Num2)
		{
			for (int u=Num1+1;u<poly1->m_vecPnts.size();u++)
			{
				ifPopBack(poly1->m_vecPnts[u],poly);
				poly->m_vecPnts.push_back(poly1->m_vecPnts[u]);

			}
			for (int v=1;v<Num2+1;v++)
			{
				ifPopBack(poly1->m_vecPnts[v],poly);
				poly->m_vecPnts.push_back(poly1->m_vecPnts[v]);
			}
		}
		else
		{
			for (int u=Num1+1;u<Num2+1;u++)
			{

				ifPopBack(poly1->m_vecPnts[u],poly);
				poly->m_vecPnts.push_back(poly1->m_vecPnts[u]);
			}
		}
	}
}

PPOLYGON  Coffset::offsetpiont(double d,PPOLYGON poly)//求单个曲线的偏置曲线函数
{
		PPOLYGON     afteroffsetpoly;
		afteroffsetpoly = new POLYGON;
		double offsetdv;
		int m=poly->m_vecPnts.size();
		for (int j=0;j<m-1;j++)
		{
			if (j==0)
			{
				bool   select1=false;
				bool   select2=false;
				VECTOR2D offsetNorm;
				VECTOR2D offsetNorm1;
				VECTOR2D offsetVect1;
				VECTOR2D offsetNorm2;
				VECTOR2D offsetVect2;
				offsetVect1.dx= poly->m_vecPnts[0].x-poly->m_vecPnts[m-2].x;
				offsetVect1.dy= poly->m_vecPnts[0].y-poly->m_vecPnts[m-2].y;
				offsetVect1.Normalize();
				offsetNorm1.dx=-offsetVect1.dy;
				offsetNorm1.dy=offsetVect1.dx;
				offsetVect2.dx= poly->m_vecPnts[1].x-poly->m_vecPnts[0].x;
				offsetVect2.dy= poly->m_vecPnts[1].y-poly->m_vecPnts[0].y;
				offsetVect2.Normalize();
				offsetNorm2.dx=-offsetVect2.dy;
				offsetNorm2.dy=offsetVect2.dx;
				double x =offsetVect1.dx-offsetVect2.dx;
				double y =offsetVect1.dy-offsetVect2.dy; 
				offsetVect1.dx=-offsetVect1.dx;
				offsetVect1.dy=-offsetVect1.dy;
				offsetNorm = offsetNorm1+offsetNorm2;
				if ((offsetNorm|(offsetVect1+offsetVect2))<=0)
				{
					POINT3D  offsetP;
					double mn = offsetVect1|offsetVect2;
					if (mn>1)
					{
						mn=1;
					}
					if (mn<-1)
					{
						mn=-1;
					}
					double a=acos(mn);
					offsetdv=d/sin(acos(mn)/2);
					offsetNorm.Normalize();
					offsetP.x= poly->m_vecPnts[0].x+offsetdv*offsetNorm.dx;
					offsetP.y= poly->m_vecPnts[0].y+offsetdv*offsetNorm.dy;
					offsetP.z= poly->m_vecPnts[0].z;
					if (poly->m_vecPnts[0].m_bIsSelected==true)  offsetP.m_bIsSelected=true;
					afteroffsetpoly->m_vecPnts.push_back(offsetP);
				} 
				else
				{

					if (abs(x)<NC_ZERO&&abs(y)<NC_ZERO)
					{
						POINT3D  offsetP;
						double mn = offsetVect1|offsetVect2;
						if (mn>1)
						{
							mn=1;
						}
						if (mn<-1)
						{
							mn=-1;
						}
						double a=acos(mn);
						offsetdv=d/sin(acos(mn)/2);
						offsetNorm.Normalize();
						offsetP.x= poly->m_vecPnts[0].x+offsetdv*offsetNorm.dx;
						offsetP.y= poly->m_vecPnts[0].y+offsetdv*offsetNorm.dy;
						offsetP.z= poly->m_vecPnts[0].z;
						if (poly->m_vecPnts[0].m_bIsSelected==true)  
							offsetP.m_bIsSelected=true;
						afteroffsetpoly->m_vecPnts.push_back(offsetP);
					} 
					else
					{
						POINT3D  offsetP1;
						POINT3D  offsetP2;
						offsetP1.x=poly->m_vecPnts[0].x+d*offsetNorm1.dx;
						offsetP1.y=poly->m_vecPnts[0].y+d*offsetNorm1.dy;
						offsetP1.z=poly->m_vecPnts[0].z;
						offsetP2.x=poly->m_vecPnts[0].x+d*offsetNorm2.dx;
						offsetP2.y=poly->m_vecPnts[0].y+d*offsetNorm2.dy;
						offsetP2.z=poly->m_vecPnts[0].z;
						offsetP2.m_bIsOperated=true;
						if (poly->m_vecPnts[0].m_bIsSelected==true) 
						{
							offsetP1.m_bIsSelected=true;
							offsetP2.m_bIsSelected=true;
						}
						afteroffsetpoly->m_vecPnts.push_back(offsetP1);
						if (abs(offsetP1.x-offsetP2.x)>NC_ZERO|abs(offsetP1.y-offsetP2.y)>NC_ZERO)
						{
							afteroffsetpoly->m_vecPnts.push_back(offsetP2);
						}
					}
				}

			}
			else
			{
				bool   select1=false;
				bool   select2=false;
				VECTOR2D offsetNorm;
				VECTOR2D offsetNorm1;
				VECTOR2D offsetVect1;
				VECTOR2D offsetNorm2;
				VECTOR2D offsetVect2;
				offsetVect1.dx= poly->m_vecPnts[j].x-poly->m_vecPnts[j-1].x;
				offsetVect1.dy= poly->m_vecPnts[j].y-poly->m_vecPnts[j-1].y;
				offsetVect1.Normalize();
				offsetNorm1.dx=-offsetVect1.dy;
				offsetNorm1.dy=offsetVect1.dx;
				offsetVect2.dx= poly->m_vecPnts[j+1].x-poly->m_vecPnts[j].x;
				offsetVect2.dy= poly->m_vecPnts[j+1].y-poly->m_vecPnts[j].y;
				offsetVect2.Normalize();
				offsetNorm2.dx=-offsetVect2.dy;
				offsetNorm2.dy=offsetVect2.dx;
				double x =offsetVect1.dx-offsetVect2.dx;
				double y =offsetVect1.dy-offsetVect2.dy; 
				offsetVect1.dx=-offsetVect1.dx;
				offsetVect1.dy=-offsetVect1.dy;
				offsetNorm = offsetNorm1+offsetNorm2;
				if ((offsetNorm|(offsetVect1+offsetVect2))<=0)
				{
					POINT3D  offsetP;
					double mn = offsetVect1|offsetVect2;
					if (mn>1)
					{
						mn=1;
					}
					if (mn<-1)
					{
						mn=-1;
					}
					double a=acos(mn);
					offsetdv=d/sin(acos(mn)/2);
					offsetNorm.Normalize();
					offsetP.x= poly->m_vecPnts[j].x+offsetdv*offsetNorm.dx;
					offsetP.y= poly->m_vecPnts[j].y+offsetdv*offsetNorm.dy;
					offsetP.z= poly->m_vecPnts[j].z;
					if (poly->m_vecPnts[j].m_bIsSelected==true) 
						offsetP.m_bIsSelected=true;
					if (abs(offsetP.x-afteroffsetpoly->m_vecPnts.back().x)>NC_ZERO|
						abs(offsetP.y-afteroffsetpoly->m_vecPnts.back().y)>NC_ZERO)
					{
						afteroffsetpoly->m_vecPnts.push_back(offsetP);
			
					}					
				} 
				else
				{
					if (abs(x)<NC_ZERO&&abs(y)<NC_ZERO)
					{
						POINT3D  offsetP;
						double mn = offsetVect1|offsetVect2;
						if (mn>1)
						{
							mn=1;
						}
						if (mn<-1)
						{
							mn=-1;
						}
						double a=acos(mn);
						offsetdv=d/sin(acos(mn)/2);
						offsetNorm.Normalize();
						offsetP.x= poly->m_vecPnts[j].x+offsetdv*offsetNorm.dx;
						offsetP.y= poly->m_vecPnts[j].y+offsetdv*offsetNorm.dy;
						offsetP.z= poly->m_vecPnts[j].z;
						if (poly->m_vecPnts[j].m_bIsSelected==true)  offsetP.m_bIsSelected=true;
						afteroffsetpoly->m_vecPnts.push_back(offsetP);	
					} 
					else
					{
						POINT3D  offsetP1;
						POINT3D  offsetP2;
						offsetP1.x=poly->m_vecPnts[j].x+d*offsetNorm1.dx;
						offsetP1.y=poly->m_vecPnts[j].y+d*offsetNorm1.dy;
						offsetP1.z=poly->m_vecPnts[j].z;
						offsetP2.x=poly->m_vecPnts[j].x+d*offsetNorm2.dx;
						offsetP2.y=poly->m_vecPnts[j].y+d*offsetNorm2.dy;
						offsetP2.z=poly->m_vecPnts[j].z;
						offsetP2.m_bIsOperated=true;
						if (poly->m_vecPnts[j].m_bIsSelected==true)  
						{
							offsetP1.m_bIsSelected=true;
							offsetP2.m_bIsSelected=true;
						}
						if (abs(offsetP1.x-afteroffsetpoly->m_vecPnts.back().x)>NC_ZERO|
							abs(offsetP1.y-afteroffsetpoly->m_vecPnts.back().y)>NC_ZERO)
						{
							afteroffsetpoly->m_vecPnts.push_back(offsetP1);
						}		
						if (abs(offsetP1.x-offsetP2.x)>NC_ZERO|abs(offsetP1.y-offsetP2.y)>NC_ZERO)
						{
							afteroffsetpoly->m_vecPnts.push_back(offsetP2);
						}
					}
					
				}
			}
		}
		if (afteroffsetpoly->m_vecPnts.back()!=afteroffsetpoly->m_vecPnts[0])
		{
			afteroffsetpoly->m_vecPnts.push_back(afteroffsetpoly->m_vecPnts[0]);
		}
		afteroffsetpoly->m_IoriClass=poly->m_IoriClass;
		return afteroffsetpoly;
	}
int Coffset::selfing(PPOLYGON beoffsetpoly,PPOLYGON poly ,PPOLYPOLYGON pPolyPoly)//去除自交
{
	
	bool ifori = KitTool::judeorientation(beoffsetpoly);
	int offsetsz=0;
	int sz = 0 ;
	POINT3D p;
	for (int i = 0;i< poly->m_vecPnts.size()-1 ;i++)
	{
		for(int j=i-2;j>-1;j-- )
		{
			if (!(i==poly->m_vecPnts.size()-2&&j==0))//防止最后一条线段和第一条线段相交
			{
				POINT3D p1,p2,p3,p4;
				p1=poly->m_vecPnts[i];
				p2=poly->m_vecPnts[i+1];
				p3=poly->m_vecPnts[j];
				p4=poly->m_vecPnts[j+1];
				if (KitTool::lineSegInter(p1,p2,p3,p4,p))
				{
					POINT3D  insertP=p;
					//p.m_bIsNew = true;
					PPOLYGON   poly_P;
					poly_P = new POLYGON;
					poly_P->m_vecPnts.push_back(insertP);
					//提取出封闭曲线
					for (int u=j+1;u<i+1;u++)
					{
						poly_P->m_vecPnts.push_back(poly->m_vecPnts[u]);

					}
					if (poly_P->m_vecPnts[0]!=poly_P->m_vecPnts.back())
					{
						poly_P->m_vecPnts.push_back(insertP);
					}
					if (poly_P->m_vecPnts.size()>3)
					{
						bool ori = KitTool::judeorientation(poly_P);
						if (ori ==ifori)
						{
							bool ifpush=true;
							for (int v=0;v<poly_P->m_vecPnts.size();v++)
							{
								if (poly_P->m_vecPnts[v].m_bIsOperated==true)
								{
									ifpush=false;
									//break;
								}
								if (poly_P->m_vecPnts[v].m_bIsSelected==true)
								{
									insertP.m_bIsSelected=true;
								}
							}
							if (ifpush==true)
							{
								pPolyPoly->m_vecPPolygons.push_back(poly_P);
								offsetsz++;
							}
							else delete poly_P;
						} 
						else 
						{
							for (int v=0;v<poly_P->m_vecPnts.size();v++)
							{
								if (poly_P->m_vecPnts[v].m_bIsSelected==true)
								{
									insertP.m_bIsSelected=true;
								}
							}
							delete poly_P;
						}
					}
					else delete poly_P;
					
					poly->m_vecPnts.erase(poly->m_vecPnts.begin()+j+1,poly->m_vecPnts.begin()+i+1);
					sz =i-(j+1);//每次减少的个数
					if (insertP!=poly->m_vecPnts[j])
					{
						if (insertP!=poly->m_vecPnts[i-sz])
						{
							poly->m_vecPnts.insert(poly->m_vecPnts.begin()+j+1,insertP);
							i=i-sz-2;
							break;
						}
						i=i-sz-2;
						break;
						//控制下标
					}
					i=i-sz-2;
					break;
				}
			} 
		}	
	}
	if (poly->m_vecPnts[0]!=poly->m_vecPnts.back())
	{
		poly->m_vecPnts.push_back(poly->m_vecPnts[0]);
	}
	if (poly->m_vecPnts.size()>2)
	{
		bool ori = KitTool::judeorientation(poly);
		if (ori == ifori)
		{
			bool ifpush=true;
			for (int v=0;v<poly->m_vecPnts.size();v++)
			{
				if (poly->m_vecPnts[v].m_bIsOperated==true)
				{
					ifpush=false;
					break;
				}
			}
			if (ifpush==true)
			{
				pPolyPoly->m_vecPPolygons.push_back(poly);
				offsetsz++;
			}
			else delete poly;
		}
		else delete poly;
	} else delete poly;
	return offsetsz-1;
}


