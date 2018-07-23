
#include "unicursal.h"
Cunicursal::Cunicursal(void)
{

}


Cunicursal::~Cunicursal(void)
{

}


void Cunicursal::Getunicursal(vec_PPOLYPOLYGON m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth)
{
	VECTOR3D DirZ(0,0,1);
	KitTool::RotatePolygon(m_vecPPolyPolygons,m_VecDir,DirZ);
	Coffset::pretreatment(m_vecPPolyPolygons);
	int m_PPPsz =m_vecPPolyPolygons.size();
	for (int z=0;z<m_PPPsz;z++)
	{
		int ppsz=m_vecPPolyPolygons[z]->m_vecPPolygons.size();
		int maxclass=m_vecPPolyPolygons[z]->maxclassfiy;
		vec_PPOLYPOLYGON vec_polypoly;
		for (int i=0;i<maxclass;i++)
		{
			PPOLYPOLYGON polypolynew=new POLYPOLYGON;
			vec_polypoly.push_back(polypolynew);
		}
		int classfiy=m_vecPPolyPolygons[z]->m_vecPPolygons[0]->m_IoriClass;
		PPOLYGON  polyNew=new POLYGON;
		vec_polypoly[0]->m_vecPPolygons.push_back(m_vecPPolyPolygons[z]->m_vecPPolygons[0]);
		for (int i=1;i<ppsz;i++)
		{
			for(int j=1;j<maxclass+1;j++)
			{
				if (m_vecPPolyPolygons[z]->m_vecPPolygons[i]->m_IoriClass==j)
				{
					vec_polypoly[j-1]->m_vecPPolygons.push_back(m_vecPPolyPolygons[z]->m_vecPPolygons[i]);
					break;
				}
			}
		}
		PPOLYPOLYGON  polypoly=new POLYPOLYGON;
		for (int i=0;i<maxclass;i++)
		{
			//PPOLYPOLYGON  polypoly=new POLYPOLYGON;
			PPOLYGON  poly =becomeUnicursal(vec_polypoly[i],FillLineWidth);
			//m_vecFillP.push_back(polypoly);
			if (poly!=0)
			{
				polypoly->m_vecPPolygons.push_back(poly);
			}
		}
		m_vecFillP.push_back(polypoly);
	}
	KitTool::RotatePolygon(m_vecFillP,DirZ,m_VecDir);
}



PPOLYGON Cunicursal::becomeUnicursal(PPOLYPOLYGON polypoly,double FillLineWidth)
{

	PPOLYGON beforepoly=becomeOnePoly(polypoly,FillLineWidth);
	beforepoly->m_Iclassify=1;
	PPOLYPOLYGON  afteroffset=new POLYPOLYGON;
	afteroffset->m_vecPPolygons.push_back(beforepoly);
	int m=1;
	Coffset::ifdeleOffset(m,afteroffset,FillLineWidth);
	int originalNum=m;
	vector<Offsetclassfiy> vec_myClassfiy;
	int OffsetNum=0;
	/////进行偏置
	do 
	{
		int m_ppsz=afteroffset->m_vecPPolygons.size();
		PPOLYPOLYGON pPolyPoly;
		pPolyPoly = new POLYPOLYGON;
		OffsetNum++;
		int u=0;
		for (int j =0;j<m;j++)
		{
			PPOLYGON  pPoly  = Coffset::offsetpiont(FillLineWidth,afteroffset->m_vecPPolygons[m_ppsz-1-j]);
			POLYGON  ploy =  *afteroffset->m_vecPPolygons[m_ppsz-1-j];
			int n =Coffset::selfing(afteroffset->m_vecPPolygons[m_ppsz-1-j],pPoly,pPolyPoly);
			Coffset::offsetClassify(originalNum,n, ploy,pPolyPoly,-1);
			//////////处理第一层连接部分的距离！
			if (OffsetNum==1)
			{

				for (int i=0;i<pPolyPoly->m_vecPPolygons.size();i++)
				{
					int PolySz=pPolyPoly->m_vecPPolygons[i]->m_vecPnts.size();
					for (int j=0;j<PolySz;j++)
					{
						if (pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j].m_bIsSelected==true)
						{
							VECTOR3D v=pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j+1]-pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j];
							v.Normalize();
							POINT3D p1=pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j]+v*VECTOR3D(0,0,1)*0.5*FillLineWidth;
							POINT3D p2=pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j+1]+v*VECTOR3D(0,0,1)*0.5*FillLineWidth;
							pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j]=p1;
							pPolyPoly->m_vecPPolygons[i]->m_vecPnts[j+1]=p2;
						}
					}
				}
			}
			//return pPolyPoly->m_vecPPolygons[0];
			//////处理旋向
			if (originalNum!=1)
			{
				for (int i=0;i<pPolyPoly->m_vecPPolygons.size();i++)
				{
					pPolyPoly->m_vecPPolygons[i]->m_bIsClosed=!(afteroffset->m_vecPPolygons[m_ppsz-1-j]->m_bIsClosed);///只是拿了m_bIsClosed这个变量做个标记，不想再弄新变量。
				}
			}
			else
			{
				for (int i=0;i<pPolyPoly->m_vecPPolygons.size();i++)
				{
					pPolyPoly->m_vecPPolygons[i]->m_bIsClosed=true;
				}
			}
			////////////////////////
			if (n>0)
			{
				Offsetclassfiy  myClassfiy;
				myClassfiy.classfiy=ploy.m_Iclassify;
				for (int i=0;i<pPolyPoly->m_vecPPolygons.size();i++)
				{
					myClassfiy.underClassfiy.push_back(pPolyPoly->m_vecPPolygons[i]->m_Iclassify);
				}
				vec_myClassfiy.push_back(myClassfiy);
			}
			afteroffset->m_vecPPolygons.insert(afteroffset->m_vecPPolygons.end(),pPolyPoly->m_vecPPolygons.begin(),
				pPolyPoly->m_vecPPolygons.end());
			u=u+n;
			pPolyPoly->m_vecPPolygons.clear();
		}
		m=m+u;
		Coffset::ifdeleOffset(m,afteroffset,FillLineWidth);
	} while (m!=0);

	for (int i=0;i<afteroffset->m_vecPPolygons.size();i++)
	{
		if (afteroffset->m_vecPPolygons[i]->m_bIsClosed==false)
		{
			reverse(afteroffset->m_vecPPolygons[i]->m_vecPnts.begin(),
				afteroffset->m_vecPPolygons[i]->m_vecPnts.end());
		}
	}

	afteroffset->m_vecPPolygons.erase(afteroffset->m_vecPPolygons.begin());
	int classfiy=afteroffset->m_vecPPolygons[0]->m_Iclassify;
	int firstClass=classfiy;
	vector<int>  SortNum;
	set<int>   set_Num;
	PPOLYGON  OnePloy=new POLYGON;
	do 
	{ 
		pair<set<int>::iterator,bool> pr=set_Num.insert(classfiy);////用于判断该分类是否被查找过
		if (pr.second)
		{
			PPOLYPOLYGON  NewPolypoly=new POLYPOLYGON;
			for (int i=0;i<afteroffset->m_vecPPolygons.size();i++)
			{
				if (afteroffset->m_vecPPolygons[i]->m_Iclassify==classfiy)
				{
					NewPolypoly->m_vecPPolygons.push_back(afteroffset->m_vecPPolygons[i]);
					afteroffset->m_vecPPolygons.erase(afteroffset->m_vecPPolygons.begin()+i);
					i--;
				}
			}
			if (classfiy==firstClass)
			{
				IntoNoOut(NewPolypoly,OnePloy,FillLineWidth);
			}
			else
			{
				for(int i=0;i<NewPolypoly->m_vecPPolygons.size();i++)
				{
					PPOLYGON  firstPoly=NewPolypoly->m_vecPPolygons[i];
					int  num=findOutPoint(firstPoly,OnePloy,FillLineWidth);
					//PPOLYGON  newPoly=new POLYGON;
					//newPoly=firstPoly;
					//IntoOut(NewPolypoly,newPoly,FillLineWidth);
					if (num>0)
					{
						OnePloy->m_vecPnts.insert(OnePloy->m_vecPnts.begin()+num+1,firstPoly->m_vecPnts.begin(),firstPoly->m_vecPnts.end());
					}
				}
			}
			//delete NewPolypoly;
		}

		/////寻找下一个偏置分类
		if (!afteroffset->m_vecPPolygons.empty())
		{

			bool  iffind=false ;
			for(int i=0;i<vec_myClassfiy.size();i++)
			{
				if (vec_myClassfiy[i].classfiy==classfiy)
				{
					iffind=true;
					if (vec_myClassfiy[i].underClassfiy.empty())
					{
						vec_myClassfiy.erase(vec_myClassfiy.begin()+i);
						classfiy=SortNum.back();
						SortNum.pop_back();
						
					} 
					else
					{
						SortNum.push_back(classfiy);
						classfiy=vec_myClassfiy[i].underClassfiy[0];
						vec_myClassfiy[i].underClassfiy.erase(vec_myClassfiy[i].underClassfiy.begin());
					}
					break;
				}
			}
			if (iffind==false)
			{
				classfiy=SortNum.back();
				SortNum.pop_back();
			}
		}
	}while (!afteroffset->m_vecPPolygons.empty());
	return OnePloy;
}



PPOLYGON Cunicursal::becomeOnePoly(PPOLYPOLYGON polypoly,double FillLineWidth)
{
	if (polypoly->m_vecPPolygons.size()==1)
	{
		return polypoly->m_vecPPolygons[0];
	}
	PPOLYPOLYGON Newpolypoly=new POLYPOLYGON;
	/////复制数据
	for (int i=0;i<polypoly->m_vecPPolygons.size();i++)
	{
		PPOLYGON poly=new POLYGON;
		poly->m_vecPnts=polypoly->m_vecPPolygons[i]->m_vecPnts;
		poly->m_Iclassify=polypoly->m_vecPPolygons[i]->m_Iclassify;
		Newpolypoly->m_vecPPolygons.push_back(poly);
	}
	PPOLYGON  poly=Newpolypoly->m_vecPPolygons.front();
	Newpolypoly->m_vecPPolygons.erase(Newpolypoly->m_vecPPolygons.begin());
	////////以下处理交线是否干涉
	do 
	{
		vector<vector<int>>   vecvec_num;
		///////查找最近点的
		for (int i=0;i<Newpolypoly->m_vecPPolygons.size();i++)
		{
			vecvec_num.push_back(findPolyMinD(poly,Newpolypoly->m_vecPPolygons[i]));
		}
		/////取多边形中距离最小的多边形
		VECTOR3D v=poly->m_vecPnts[vecvec_num[0][0]]-Newpolypoly->m_vecPPolygons[0]->m_vecPnts[vecvec_num[0][1]];
		double mind=v.GetLength();
		int best=0;
		for (int i=0;i<vecvec_num.size();i++)
		{
			v=poly->m_vecPnts[vecvec_num[i][0]]-Newpolypoly->m_vecPPolygons[i]->m_vecPnts[vecvec_num[i][1]];
			double d=v.GetLength();
			if (d<mind) best=i;
		}
		PPOLYGON  poly2 =Newpolypoly->m_vecPPolygons[best];
		Newpolypoly->m_vecPPolygons.erase(Newpolypoly->m_vecPPolygons.begin()+best);
		//////拼接多边形
		poly2->m_vecPnts.pop_back();
		PPOLYGON  Newpoly2=new POLYGON;
		poly2->m_vecPnts[vecvec_num[best][1]].m_bIsSelected=true;
		Newpoly2->m_vecPnts.insert(Newpoly2->m_vecPnts.begin(),poly2->m_vecPnts.begin()+vecvec_num[best][1],
			poly2->m_vecPnts.end());
		Newpoly2->m_vecPnts.insert(Newpoly2->m_vecPnts.end(),poly2->m_vecPnts.begin(),poly2->m_vecPnts.begin()+vecvec_num[best][1]+1);

		//POINT3D p1=poly->m_vecPnts[vecvec_num[best][0]];
		//POINT3D p2=Newpoly2->m_vecPnts[0];
		//VECTOR3D v1=(p2-p1);
		//v1.Normalize();
		//POINT3D p11=p1+v1*VECTOR3D(0,0,1)*0.5*FillLineWidth;
		//POINT3D p12=p1+VECTOR3D(0,0,1)*v1*0.5*FillLineWidth;
		//POINT3D p21=p2+v1*VECTOR3D(0,0,1)*0.5*FillLineWidth;
		//POINT3D p22=p2+VECTOR3D(0,0,1)*v1*0.5*FillLineWidth;

		/*Newpoly2->m_vecPnts.insert(Newpoly2->m_vecPnts.begin(),p21);
		Newpoly2->m_vecPnts.push_back(p22);*/
		poly->m_vecPnts[vecvec_num[best][0]].m_bIsSelected=true;
		POINT3D p=poly->m_vecPnts[vecvec_num[best][0]];

		poly->m_vecPnts.insert(poly->m_vecPnts.begin()+vecvec_num[best][0]+1,p);
		//poly->m_vecPnts.insert(poly->m_vecPnts.begin()+vecvec_num[best][0]+1,p11);
		//poly->m_vecPnts.insert(poly->m_vecPnts.begin()+vecvec_num[best][0]+2,p12);

		//POINT3D  p=p1;
		//poly->m_vecPnts[vecvec_num[best][0]].m_bIsSelected=true;
		//p.m_bIsSelected=true;
		//poly->m_vecPnts.insert(poly->m_vecPnts.begin()+vecvec_num[best][0],p);
		poly->m_vecPnts.insert(poly->m_vecPnts.begin()+vecvec_num[best][0]+1,Newpoly2->m_vecPnts.begin(),
			Newpoly2->m_vecPnts.end());
		delete poly2;
	} while (!Newpolypoly->m_vecPPolygons.empty());

	return poly;
}



vector<int> Cunicursal::findPolyMinD(PPOLYGON poly1,PPOLYGON poly2)
{
	vector<int>   vec_Num;
	VECTOR3D v=poly1->m_vecPnts[0]-poly2->m_vecPnts[0];
	double dmin=v.GetLength();
	vec_Num.resize(2);
	vec_Num[0]=0;
	vec_Num[1]=0;
	for (int i=0;i<poly1->m_vecPnts.size();i++)
	{
		for (int j=0;j<poly2->m_vecPnts.size();j++)
		{
			VECTOR3D v=poly1->m_vecPnts[i]-poly2->m_vecPnts[j];
			double d=v.GetLength();
			if (d<dmin)
			{
				dmin=d;
				vec_Num[0]=i;
				vec_Num[1]=j;
			}
		}
	}
	return vec_Num;
}





int Cunicursal::findClosestPoint(POINT3D p,PPOLYGON poly,double FillLineWidth)
{
	/////KD树出问题！！！！！
	/*vector<vec> vec_vec;
	int sz=poly->m_vecPnts.size();
	for (int i=0;i<sz;i++)
	{
	vec pp;
	pp[0]=poly->m_vecPnts[i].x;
	pp[1]=poly->m_vecPnts[i].y;
	pp[2]=poly->m_vecPnts[i].z;
	vec_vec.push_back(pp);
	}
	vec pp;
	int vn=vec_vec.size();
	int my_id=-1;
	pp[0]=p.x;
	pp[1]=p.y;
	pp[2]=p.z;
	float	*v0 = &vec_vec[0][0];
	KDtree	*kd = new KDtree(v0, vn);
	const float *match = kd->closest_to_pt(pp,5000000);
	if (match)
	{
	my_id=(match - v0) / 3;;
	}
	kd=NULL;
	delete kd;
	return my_id;*/
	int  best_id=0;
	VECTOR3D  v=poly->m_vecPnts[0]-p;
	int  bestD=v.GetLength();
	for (int i=1;i<poly->m_vecPnts.size();i++)
	{
		v=poly->m_vecPnts[i]-p;
		if (v.GetLength()<bestD)
		{
			best_id=i;
			bestD=v.GetLength();
		}
	}

	return best_id;
}




//void Cunicursal::IntoOut(PPOLYPOLYGON polypoly,PPOLYGON poly,double FillLineWidth)
//{
//
//	PPOLYPOLYGON  polypoly2=new POLYPOLYGON;
//	int PolySZ=polypoly->m_vecPPolygons.size();
//	for (int i=0;i<PolySZ;i++)
//	{
//		PPOLYGON  donePoly = polypoly->m_vecPPolygons[i];
//		if (i!=PolySZ-1)
//		{
//			int sz=donePoly->m_vecPnts.size();
//			PPOLYGON  donePoly1=new POLYGON;
//			PPOLYGON  donePoly2=new POLYGON;
//			int insert_id;
//			//////
//			if (i!=PolySZ-2)
//			{
//				insert_id=findinsertPoint(donePoly,polypoly->m_vecPPolygons[i+1],FillLineWidth);
//			}
//			else
//			{
//				polypoly->m_vecPPolygons[i+1]->UpdateBox();
//				float  dx=(polypoly->m_vecPPolygons[i+1]->m_BoxPolygon.x1)-
//					(polypoly->m_vecPPolygons[i+1]->m_BoxPolygon.x0);
//
//				float  dy=(polypoly->m_vecPPolygons[i+1]->m_BoxPolygon.y1)-
//					(polypoly->m_vecPPolygons[i+1]->m_BoxPolygon.y0);
//				if (dx<2*FillLineWidth&&dy<2*FillLineWidth)
//				{
//					insert_id=sz/2;
//					LineWidthPop(donePoly,FillLineWidth,sz/2);
//					int num=findClosestPoint(donePoly->m_vecPnts[sz/2],polypoly->m_vecPPolygons[i+1],FillLineWidth);
//					vector<POINT3D>  tempP=polypoly->m_vecPPolygons[i+1]->m_vecPnts;
//					tempP.pop_back();
//					polypoly->m_vecPPolygons[i+1]->m_vecPnts.clear();
//					polypoly->m_vecPPolygons[i+1]->m_vecPnts.insert(polypoly->m_vecPPolygons[i+1]->m_vecPnts.begin(),tempP.begin()+num,tempP.end());
//					polypoly->m_vecPPolygons[i+1]->m_vecPnts.insert(polypoly->m_vecPPolygons[i+1]->m_vecPnts.end(),tempP.begin(),tempP.begin()+num);
//					//////////////////////
//					polypoly->m_vecPPolygons[i+1]->m_vecPnts.erase(polypoly->m_vecPPolygons[i+1]->m_vecPnts.begin());				
//				}
//				else
//				{
//					insert_id=findinsertPoint(donePoly,polypoly->m_vecPPolygons[i+1],FillLineWidth);
//				}
//			}
//			donePoly1->m_vecPnts.insert(donePoly1->m_vecPnts.begin(),donePoly->m_vecPnts.begin(),donePoly->m_vecPnts.begin()+insert_id+1);
//			donePoly2->m_vecPnts.insert(donePoly2->m_vecPnts.begin(),donePoly->m_vecPnts.begin()+insert_id+1,donePoly->m_vecPnts.end());
//			polypoly2->m_vecPPolygons.push_back(donePoly2);
//			poly->m_vecPnts.insert(poly->m_vecPnts.end(),donePoly1->m_vecPnts.begin(),donePoly1->m_vecPnts.end());
//			delete donePoly;
//
//		}
//		else
//		{
//			poly->m_vecPnts.insert(poly->m_vecPnts.end(),donePoly->m_vecPnts.begin(),donePoly->m_vecPnts.end());
//			delete donePoly;
//		}
//	}
//
//
//	int Poly2sz=polypoly2->m_vecPPolygons.size();
//	for (int i=0;i<Poly2sz;i++)
//	{
//		PPOLYGON  donePoly = polypoly2->m_vecPPolygons.back();
//		poly->m_vecPnts.insert(poly->m_vecPnts.end(),donePoly->m_vecPnts.begin(),donePoly->m_vecPnts.end());
//		polypoly2->m_vecPPolygons.pop_back();
//	}
//}


void Cunicursal::IntoNoOut(PPOLYPOLYGON polypoly, PPOLYGON poly,double FillLineWidth)
{
	//polypoly->m_vecPPolygons[0]->m_vecPnts.pop_back();
	LineWidthPop(polypoly->m_vecPPolygons[0],FillLineWidth);
	poly->m_vecPnts.insert(poly->m_vecPnts.end(),
		polypoly->m_vecPPolygons[0]->m_vecPnts.begin(),polypoly->m_vecPPolygons[0]->m_vecPnts.end ());
	for (int i=1;i<polypoly->m_vecPPolygons.size();i++)
	{
		POINT3D P=polypoly->m_vecPPolygons[i-1]->m_vecPnts.front();
		PPOLYGON donePoly=polypoly->m_vecPPolygons[i];
		donePoly->m_vecPnts.pop_back();
		int  num=findClosestPoint(P,donePoly,FillLineWidth);
		PPOLYGON polynew=new POLYGON;
		polynew->m_vecPnts.insert(polynew->m_vecPnts.end(),donePoly->m_vecPnts.begin()+num,donePoly->m_vecPnts.end());
		polynew->m_vecPnts.insert(polynew->m_vecPnts.end(),donePoly->m_vecPnts.begin(),donePoly->m_vecPnts.begin()+num+1);
		LineWidthPop(polynew,FillLineWidth);
		poly->m_vecPnts.insert(poly->m_vecPnts.end(),polynew->m_vecPnts.begin(),polynew->m_vecPnts.end());
	}
}



void Cunicursal::LineWidthPop(PPOLYGON poly,double FillLineWidth)
{
	bool  bifstop=false;
	POINT3D p=poly->m_vecPnts.back();
	poly->m_vecPnts.pop_back();
	do 
	{
		if (poly->m_vecPnts.size()<2)
		{
			return;
		}
		POINT3D p1=poly->m_vecPnts.front();
		POINT3D p2=poly->m_vecPnts.back();
		VECTOR3D v=p1-p2;
		if (v.GetLength()<FillLineWidth)
		{
			p=poly->m_vecPnts.back();
			poly->m_vecPnts.pop_back();
		}
		else
		{
	///////////二分法求函数值
			POINT3D  pmind;
			VECTOR3D v1;
			do 
			{
				pmind=p2+(p-p2)/2;
				v1=p1-pmind;
				double a=v1.GetLength();
				if (a>FillLineWidth) p2=pmind;
				else p=pmind;
			} while (abs(v1.GetLength()-FillLineWidth)>CAD_ZERO);
			bifstop=true;
			poly->m_vecPnts.push_back(pmind);
		}
	} while (bifstop==false);
}

//void Cunicursal::LineWidthPop(PPOLYGON poly,double FillLineWidth,int insert_id)
//{
//	bool  bifstop=false;
//	POINT3D p=poly->m_vecPnts[insert_id];
//	do 
//	{
//		POINT3D p1=poly->m_vecPnts[insert_id];
//		POINT3D p2=poly->m_vecPnts[insert_id+1];
//		VECTOR3D v=p1-p2;
//		if (v.GetLength()<FillLineWidth)
//		{
//			p=poly->m_vecPnts[insert_id];
//			poly->m_vecPnts.erase(poly->m_vecPnts.begin()+insert_id+1);
//		}
//		else
//		{
//			///////////二分法求函数值
//			POINT3D  pmind;
//			VECTOR3D v1;
//			do 
//			{
//				pmind=p2+(p-p2)/2;
//				v1=p1-pmind;
//				double a=v1.GetLength();
//				if (a>FillLineWidth) p2=pmind;
//				else p=pmind;
//			} while (abs(v1.GetLength()-FillLineWidth)>CAD_ZERO);
//			bifstop=true;
//			poly->m_vecPnts.insert(poly->m_vecPnts.begin()+insert_id+1,pmind);
//		}
//	} while (bifstop==false);
//}




//
//int  Cunicursal::findinsertPoint(PPOLYGON ploy1,PPOLYGON ploy2,double FillLineWidth)
//{
//	//int  insert_id=1;
//	//int num;////下一个环最近点
//	//bool  ifBreak=false;
//	//do 
//	//{
//	//	POINT3D  insertP=ploy1->m_vecPnts[insert_id];
//	//	num=findClosestPoint(insertP,ploy2,FillLineWidth);
//	//	VECTOR3D v=insertP-ploy2->m_vecPnts[num];
//	//	VECTOR3D v11=ploy1->m_vecPnts[insert_id+1]-ploy1->m_vecPnts[insert_id];
//	//	VECTOR3D v22;
//	//	if (num==0) v22=ploy2->m_vecPnts[ploy2->m_vecPnts.size()-2]-ploy2->m_vecPnts[0];
//	//	else v22=ploy2->m_vecPnts[num-1]-ploy2->m_vecPnts[num];
//	//	if ((v11|v22)<0&&insert_id!=ploy1->m_vecPnts.size()-2) insert_id++;
//	//	else if (v.GetLength()>2*FillLineWidth&&insert_id!=ploy1->m_vecPnts.size()-2) insert_id++;
//	//	else 
//	//	{
//	//		vector<POINT3D>  vec_p1;
//	//		vector<POINT3D>  vec_p2;
//	//		vec_p1=ploy1->m_vecPnts;
//	//		vec_p2=ploy2->m_vecPnts;
//	//		vector<POINT3D>  vec_temp;
//	//		vec_temp.insert(vec_temp.begin(),vec_p2.begin()+num,vec_p2.end());
//	//		vec_temp.insert(vec_temp.end(),vec_p2.begin(),vec_p2.begin()+num+1);
//	//		PPOLYGON  ploytemp1=new POLYGON;
//	//		ploytemp1->m_vecPnts=vec_p1;
//	//		LineWidthPop(ploytemp1,FillLineWidth,insert_id);
//	//		PPOLYGON  ploytemp2=new POLYGON;
//	//		ploytemp2->m_vecPnts=vec_temp;
//	//		LineWidthPop(ploytemp2,FillLineWidth);
//	//		VECTOR3D  v2=ploytemp1->m_vecPnts[insert_id+1]-ploytemp2->m_vecPnts.back();
//	//		v.GetNormal();
//	//		v2.GetNormal();
//	//		if ((v|v2)>0.7&&insert_id==ploy1->m_vecPnts.size()-2)
//	//		{
//	//			ploy1->m_vecPnts=ploytemp1->m_vecPnts;
//	//			ploy2->m_vecPnts=ploytemp2->m_vecPnts;
//	//			ifBreak=true;
//	//		}
//	//		else insert_id++;
//	//	}
//	//} while (ifBreak==false);
//	//return insert_id;
//
//	typedef struct PntInt{POINT3D p;int num;PntInt(){}  ~PntInt(){}};
//	vector<PntInt>   vec_PntInt1;
//	vector<PntInt>   vec_PntInt2;
//	bool ifbreak=false;
//	int findNum;
//	findNum=1;
//	do 
//	{
//		vec_PntInt1.clear();
//		vec_PntInt2.clear();
//		PPOLYGON ploytemp=new POLYGON;
//		ploytemp->m_vecPnts=ploy1->m_vecPnts;
//		LineWidthPop(ploytemp,FillLineWidth);
//		if (ploytemp->m_vecPnts.size()<2)
//		{
//			return -1;
//		}
//		VECTOR3D  v=ploytemp->m_vecPnts.front()-ploytemp->m_vecPnts.back();
//		VECTOR3D  vd;
//		if (CSlice::judeorientation(ploy1)) vd=v*VECTOR3D(0,0,1);
//		else vd=VECTOR3D(0,0,1)*v;
//		vd.Normalize();
//		int sz=ploy2->m_vecPnts.size();
//		for (int i=0;i<sz-1;i++)
//		{
//			POINT3D p1=ploy2->m_vecPnts[i];
//			POINT3D p2=ploy2->m_vecPnts[i+1];
//			///////求第一个点的交点集
//			VECTOR3D v1=p1-ploytemp->m_vecPnts.front();
//			VECTOR3D v2=p2-ploytemp->m_vecPnts.front();
//			VECTOR3D v11=v1*vd;
//			VECTOR3D v22=v2*vd;
//			v11.Normalize();
//			v22.Normalize();
//			if ((v11|VECTOR3D(0,0,1))*(v22|VECTOR3D(0,0,1))<0)
//			{
//				POINT3D p31=ploytemp->m_vecPnts.front();
//				POINT3D p41=p31+vd*1.5*FillLineWidth;
//				POINT3D p32=ploytemp->m_vecPnts.back();
//				POINT3D p42=p32+vd*1.5*FillLineWidth;
//				POINT3D pp1,pp2;
//				if (Coffset::lineSegInter(p1,p2,p31,p41,pp1))
//				{
//					PntInt  PntI;
//					PntI.p=pp1;
//					PntI.num=i;
//					vec_PntInt1.push_back(PntI);
//					POINT3D p1=vec_PntInt1[0].p;
//				}
//				if (Coffset::lineSegInter(p1,p2,p32,p42,pp2))
//				{
//					PntInt  PntI;
//					PntI.p=pp2;
//					PntI.num=i;
//					vec_PntInt2.push_back(PntI);
//				}
//			}
//		}
//		if (vec_PntInt1.size()==0||vec_PntInt2.size()==0)
//		{
//			goto falsefind;
//		}
//		if (vec_PntInt1.size()!=1)
//		{
//			int best=0;
//			double d=(vec_PntInt1[0].p-ploytemp->m_vecPnts.front()).GetLength();
//			for (int i=1;i<vec_PntInt1.size();i++)
//			{
//				double tempD=(vec_PntInt1[i].p-ploytemp->m_vecPnts.front()).GetLength();
//				if (d>tempD);
//				{
//					d=tempD;
//					best=i;
//				}
//			}
//			vec_PntInt1[0]=vec_PntInt1[best];
//		}
//		if (vec_PntInt2.size()!=1)
//		{
//			int best=0;
//			double d=(vec_PntInt2[0].p-ploytemp->m_vecPnts.back()).GetLength();
//			for (int i=1;i<vec_PntInt1.size();i++)
//			{
//				double tempD=(vec_PntInt2[i].p-ploytemp->m_vecPnts.back()).GetLength();
//				if (d>tempD);
//				{
//					d=tempD;
//					best=i;
//				}
//			}
//			vec_PntInt2[0]=vec_PntInt1[best];
//		} 
//
//		if (CSlice::judeorientation(ploy1))
//		{
//			if (vec_PntInt1[0].num<=vec_PntInt2[0].num)
//			{
//				LineWidthPop(ploy1,FillLineWidth);
//				if (vec_PntInt1[0].num<vec_PntInt2[0].num)
//				{
//					ploy2->m_vecPnts.erase(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1,ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1);
//				}
//				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1,vec_PntInt1[0].p);
//				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+2,vec_PntInt2[0].p);
//				return vec_PntInt1[0].num+1;
//			}
//		}
//		else
//		{
//			if (vec_PntInt1[0].num>=vec_PntInt2[0].num)
//			{
//				LineWidthPop(ploy1,FillLineWidth);
//				if (vec_PntInt1[0].num>vec_PntInt2[0].num)
//				{
//					ploy2->m_vecPnts.erase(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1,ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1);
//				}
//				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1,vec_PntInt1[0].p);
//				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+2,vec_PntInt2[0].p);
//				return vec_PntInt2[0].num+1;
//			}
//
//		}
//falsefind:
//		if(ifbreak==false)
//		{
//			findNum++;
//			if (findNum==ploy1->m_vecPnts.size())  return -1;
//			vector<POINT3D> vec_PntTemp;
//			ploy1->m_vecPnts.pop_back();
//			vec_PntTemp.insert(vec_PntTemp.end(),ploy1->m_vecPnts.begin()+1,ploy1->m_vecPnts.end());
//			vec_PntTemp.push_back(ploy1->m_vecPnts[0]);
//			vec_PntTemp.push_back(vec_PntTemp[0]);
//			ploy1->m_vecPnts=vec_PntTemp;
//		}
//
//	} while (ifbreak==false);
//
//
//
//
//}




int Cunicursal::findOutPoint(PPOLYGON ploy1,PPOLYGON ploy2,double FillLineWidth)
{
	typedef struct PntInt{POINT3D p;int num;PntInt(){}  ~PntInt(){}};
	vector<PntInt>   vec_PntInt1;
	vector<PntInt>   vec_PntInt2;
	bool ifbreak=false;
	int findNum;
	findNum=1;
	do 
	{
		vec_PntInt1.clear();
		vec_PntInt2.clear();
		PPOLYGON ploytemp=new POLYGON;
		ploytemp->m_vecPnts=ploy1->m_vecPnts;
		LineWidthPop(ploytemp,FillLineWidth);
		if (ploytemp->m_vecPnts.size()<2)
		{
			return -1;
		}
		VECTOR3D  v=ploytemp->m_vecPnts.front()-ploytemp->m_vecPnts.back();
		VECTOR3D  vd;
		if (KitTool::judeorientation(ploy1)) vd=v*VECTOR3D(0,0,1);
		else vd=VECTOR3D(0,0,1)*v;
		vd.Normalize();
		int sz=ploy2->m_vecPnts.size();
		for (int i=0;i<sz-1;i++)
		{
			POINT3D p1=ploy2->m_vecPnts[i];
			POINT3D p2=ploy2->m_vecPnts[i+1];
			///////求第一个点的交点集
			VECTOR3D v1=p1-ploytemp->m_vecPnts.front();
			VECTOR3D v2=p2-ploytemp->m_vecPnts.front();
			VECTOR3D v11=v1*vd;
			VECTOR3D v22=v2*vd;
			v11.Normalize();
			v22.Normalize();
			if ((v11|VECTOR3D(0,0,1))*(v22|VECTOR3D(0,0,1))<0)
			{
				POINT3D p31=ploytemp->m_vecPnts.front();
				POINT3D p41=p31+vd*1.5*FillLineWidth;
				POINT3D p32=ploytemp->m_vecPnts.back();
				POINT3D p42=p32+vd*1.5*FillLineWidth;
				POINT3D pp1,pp2;
				if (KitTool::lineSegInter(p1,p2,p31,p41,pp1))
				{
					PntInt  PntI;
					PntI.p=pp1;
					PntI.num=i;
					vec_PntInt1.push_back(PntI);
					POINT3D p1=vec_PntInt1[0].p;
				}
 				if (KitTool::lineSegInter(p1,p2,p32,p42,pp2))
				{
					PntInt  PntI;
					PntI.p=pp2;
					PntI.num=i;
					vec_PntInt2.push_back(PntI);
				}
			}
		}
		if (vec_PntInt1.size()==0||vec_PntInt2.size()==0)
		{
			goto falsefind;
		}
		if (vec_PntInt1.size()!=1)
		{
			int best=0;
			double d=(vec_PntInt1[0].p-ploytemp->m_vecPnts.front()).GetLength();
			for (int i=1;i<vec_PntInt1.size();i++)
			{
				double tempD=(vec_PntInt1[i].p-ploytemp->m_vecPnts.front()).GetLength();
				if (d>tempD);
				{
					d=tempD;
					best=i;
				}
			}
			vec_PntInt1[0]=vec_PntInt1[best];
		}
		if (vec_PntInt2.size()!=1)
		{
			int best=0;
			double d=(vec_PntInt2[0].p-ploytemp->m_vecPnts.back()).GetLength();
			for (int i=1;i<vec_PntInt1.size();i++)
			{
				double tempD=(vec_PntInt2[i].p-ploytemp->m_vecPnts.back()).GetLength();
				if (d>tempD);
				{
					d=tempD;
					best=i;
				}
			}
			vec_PntInt2[0]=vec_PntInt1[best];
		} 

		if (KitTool::judeorientation(ploy1))
		{
			if (vec_PntInt1[0].num<=vec_PntInt2[0].num)
			{
				LineWidthPop(ploy1,FillLineWidth);
				if (vec_PntInt1[0].num<vec_PntInt2[0].num)
				{
					ploy2->m_vecPnts.erase(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1,ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1);
				}
				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1,vec_PntInt1[0].p);
				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+2,vec_PntInt2[0].p);
				return vec_PntInt1[0].num+1;
			}
		}
		else
		{
			if (vec_PntInt1[0].num>=vec_PntInt2[0].num)
			{
				LineWidthPop(ploy1,FillLineWidth);
				if (vec_PntInt1[0].num>vec_PntInt2[0].num)
				{
					ploy2->m_vecPnts.erase(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1,ploy2->m_vecPnts.begin()+vec_PntInt1[0].num+1);
				}
				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+1,vec_PntInt1[0].p);
				ploy2->m_vecPnts.insert(ploy2->m_vecPnts.begin()+vec_PntInt2[0].num+2,vec_PntInt2[0].p);
				return vec_PntInt2[0].num+1;
			}

		}
		falsefind:
		if(ifbreak==false)
		{
			findNum++;
			if (findNum==ploy1->m_vecPnts.size()) 
				return -1;
			vector<POINT3D> vec_PntTemp;
			ploy1->m_vecPnts.pop_back();
			vec_PntTemp.insert(vec_PntTemp.end(),ploy1->m_vecPnts.begin()+1,ploy1->m_vecPnts.end());
			vec_PntTemp.push_back(ploy1->m_vecPnts[0]);
			vec_PntTemp.push_back(vec_PntTemp[0]);
			ploy1->m_vecPnts=vec_PntTemp;
		}
	} while (ifbreak==false);
}