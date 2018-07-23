#include "Line.h"
CLine::CLine(void)
{
	
}
CLine::~CLine(void)
{

}

void CLine::GetLine(vec_PPOLYPOLYGON m_vecPPolyPolygons, VECTOR3D m_VecDir, vec_PPOLYPOLYGON &m_vecFillP, double FillLineWidth)
{
	if (m_vecPPolyPolygons.size()<1)
		return;
	//VECTOR3D DirZ(0,0,1);
	//CSlice::RotatePolygon(pSlice->m_vecPPolyPolygons,m_VecDir,DirZ);
	Coffset::pretreatment(m_vecPPolyPolygons);
	//double YN;
	//POINT3D TDp;
	int cenN=m_vecPPolyPolygons.size();
	for(int z=0;z<cenN;z++)
	{
		vec_PPOLYPOLYGON  vec_polypoly;
		int sameCN = m_vecPPolyPolygons[z]->m_vecPPolygons.size();
		int maxClassfiy=m_vecPPolyPolygons[z]->maxclassfiy;
		//vec_polypoly.resize(maxClassfiy);
		for (int i=0;i<maxClassfiy;i++)
		{
			vec_polypoly.push_back(new POLYPOLYGON);
		}
		for (int classfiy=1;classfiy<maxClassfiy+1;classfiy++)
		{
			for (int i=0;i<sameCN;i++)
			{
				if (m_vecPPolyPolygons[z]->m_vecPPolygons[i]->m_IoriClass==classfiy)
				{
					vec_polypoly[classfiy-1]->m_vecPPolygons.push_back(m_vecPPolyPolygons[z]->m_vecPPolygons[i]);
				}
			}
		}
		vec_PPOLYPOLYGON  m_vecFillPTemp;
		GetContourLine(vec_polypoly,m_VecDir,m_vecFillPTemp,FillLineWidth);
		for (int i=1;i<m_vecFillPTemp.size();i++)
		{
			m_vecFillPTemp[0]->m_vecPPolygons.insert(m_vecFillPTemp[0]->m_vecPPolygons.end(),
				m_vecFillPTemp[i]->m_vecPPolygons.begin(),m_vecFillPTemp[i]->m_vecPPolygons.end());
			//delete m_vecFillPTemp[i];
		}
		m_vecFillP.push_back(m_vecFillPTemp[0]);
		//double first_YN=FillLineWidth;
		//YN=first_YN;
		//set<SORTX>setSortX;  //自定义排序方式找填充点set
		//PPOLYPOLYGON  SameCLine;
		//SameCLine = new POLYPOLYGON;
		//PPOLYPOLYGON  TempLine;
		//TempLine = new POLYPOLYGON;
		//pSlice->m_vecPPolyPolygons[z]->UpdateBox();
		//double startpoint=pSlice->m_vecPPolyPolygons[z]->m_BoxPolyPolygon.y0;
		//double ymax=pSlice->m_vecPPolyPolygons[z]->m_BoxPolyPolygon.y1;
		//do
		////for(int p=1;p<PaddN;p++)
		//{
		//	PPOLYGON  singleLine;
		//	singleLine = new POLYGON;
		//	for(int i =0;i<sameCN;i++)
		//	{
		//		int fcN =  pSlice->m_vecPPolyPolygons[z]->m_vecPPolygons[i]->m_vecPnts.size();
		//		for (int j=0;j<fcN-1;j++)
		//		{
		//			POINT3D a1;
		//			POINT3D a2;
		//			a1 = pSlice->m_vecPPolyPolygons[z]->m_vecPPolygons[i]->m_vecPnts[j];
		//			a2 = pSlice->m_vecPPolyPolygons[z]->m_vecPPolygons[i]->m_vecPnts[j+1];
		//			if (CrossY(a1,a2,startpoint + YN,TDp))
		//			{   	
		//				singleLine->m_vecPnts.push_back(TDp);
		//			}
		//		}				
		//	}
		//	SortXmin(singleLine);
		//	TempLine->m_vecPPolygons.push_back(singleLine);
		//	YN+=first_YN;
		//	if ((startpoint + YN)>ymax)
		//	{
		//		break;
		//	}
		//}while(1==1);
		//becomePadLine(SameCLine,TempLine);
		//m_vecFillP.push_back(SameCLine);
		//delete TempLine;
		//m_vecpaddN.push_back(setSortX);
	}
	//CSlice::RotatePolygon(m_vecFillP,DirZ,m_VecDir);
}





void CLine::GetContourLine(vec_PPOLYPOLYGON vec_contour,VECTOR3D m_VecDir,vec_PPOLYPOLYGON &m_vecFillP,double FillLineWidth)
{
	if (vec_contour.empty())
		return;
	VECTOR3D DirZ(0,0,1);
	KitTool::RotatePolygon(vec_contour,m_VecDir,DirZ);
	double YN;
	POINT3D TDp;
	int cenN=vec_contour.size();
	for(int z=0;z<cenN;z++)
	{

		for (int i=0;i<vec_contour[z]->m_vecPPolygons.size();i++)
		{
			vec_contour[z]->m_vecPPolygons[i]->m_Iclassify=i+1;
		}
		double first_YN=FillLineWidth;
		YN=0;
		set<SORTX>setSortX;  //自定义排序方式找填充点set
		int sameCN = vec_contour[z]->m_vecPPolygons.size();
		PPOLYPOLYGON  SameCLine;
		SameCLine = new POLYPOLYGON;
		PPOLYPOLYGON  TempLine;
		TempLine = new POLYPOLYGON;
		vec_contour[z]->UpdateBox();
		double startpoint=vec_contour[z]->m_BoxPolyPolygon.y0;
		double ymax=vec_contour[z]->m_BoxPolyPolygon.y1;
		do
		{
			PPOLYGON  singleLine;
			singleLine = new POLYGON;
			for(int i =0;i<sameCN;i++)
			{
				int fcN =  vec_contour[z]->m_vecPPolygons[i]->m_vecPnts.size();
				for (int j=0;j<fcN-1;j++)
				{
					POINT3D a1;
					POINT3D a2;
					a1 = vec_contour[z]->m_vecPPolygons[i]->m_vecPnts[j];
					a2 = vec_contour[z]->m_vecPPolygons[i]->m_vecPnts[j+1];
					if (CrossY(a1,a2,startpoint + YN,TDp))
					{   
						TDp.m_classfiy=vec_contour[z]->m_vecPPolygons[i]->m_Iclassify;
						singleLine->m_vecPnts.push_back(TDp);
					}
				}				
			}
			SortXmin(singleLine);
			TempLine->m_vecPPolygons.push_back(singleLine);
			YN+=first_YN;
			if ((startpoint + YN)>ymax)
			{
				break;
			}
		}while(1==1);
		becomePadLine(SameCLine,TempLine,FillLineWidth);
		m_vecFillP.push_back(SameCLine);
		//m_vecFillP[z]->m_vecPPolygons.insert(m_vecFillP[z]->m_vecPPolygons.end(),SameCLine->m_vecPPolygons.begin(),
			//SameCLine->m_vecPPolygons.end());
		delete TempLine;
	}
	KitTool::RotatePolygon(m_vecFillP,DirZ,m_VecDir);
}


void CLine::becomePadLine(PPOLYPOLYGON SameCLine,PPOLYPOLYGON TempLine,double FillLineWidth)
{
	do
	{
		if (!TempLine->m_vecPPolygons.front()->m_vecPnts.empty())
		{
			PPOLYGON poly=new POLYGON;
			poly->m_vecPnts.push_back(TempLine->m_vecPPolygons.front()->m_vecPnts[0]);
			poly->m_vecPnts.push_back(TempLine->m_vecPPolygons.front()->m_vecPnts[1]);
			TempLine->m_vecPPolygons.front()->m_vecPnts.erase(TempLine->m_vecPPolygons.front()->m_vecPnts.begin());
			TempLine->m_vecPPolygons.front()->m_vecPnts.erase(TempLine->m_vecPPolygons.front()->m_vecPnts.begin());
			for (int i=1;i<TempLine->m_vecPPolygons.size();i++)
			{
				vector<PntAndNum>   vec_p_num;
				for (int j=0;j<TempLine->m_vecPPolygons[i]->m_vecPnts.size()/2;j++)
				{
					PntAndNum p_num;
					if (i%2==1)
					{
						p_num.p=TempLine->m_vecPPolygons[i]->m_vecPnts[j*2+1];
						p_num.num=j*2+1;
					}
					else
					{
						 p_num.p=TempLine->m_vecPPolygons[i]->m_vecPnts[j*2];
						 p_num.num=j*2;
					}
					if (p_num.p.m_classfiy==poly->m_vecPnts.back().m_classfiy)
						vec_p_num.push_back(p_num);
				}

				if (!vec_p_num.empty())
				{
					VECTOR3D v=vec_p_num[0].p-poly->m_vecPnts.back();
					double mind=v.GetLength();
					int best=0;
					for (int j=1;j<vec_p_num.size();j++)
					{
						v=vec_p_num[j].p-poly->m_vecPnts.back();
						if (v.GetLength()<mind)
						{
							mind=v.GetLength();
							best=j;
						}
					}
					v=vec_p_num[best].p-poly->m_vecPnts.back();
					double d=v.GetLength();
					if (d>(5*FillLineWidth))
					{
						goto Imbreask;
					} 
					else
					{
						if (i%2==1)
						{
							poly->m_vecPnts.push_back(TempLine->m_vecPPolygons[i]->m_vecPnts[vec_p_num[best].num]);
							poly->m_vecPnts.push_back(TempLine->m_vecPPolygons[i]->m_vecPnts[vec_p_num[best].num-1]);
							TempLine->m_vecPPolygons[i]->m_vecPnts.erase(TempLine->m_vecPPolygons[i]->m_vecPnts.begin()+vec_p_num[best].num-1,
								TempLine->m_vecPPolygons[i]->m_vecPnts.begin()+vec_p_num[best].num+1);

						} 
						else
						{
							poly->m_vecPnts.push_back(TempLine->m_vecPPolygons[i]->m_vecPnts[vec_p_num[best].num]);
							poly->m_vecPnts.push_back(TempLine->m_vecPPolygons[i]->m_vecPnts[vec_p_num[best].num+1]);
							TempLine->m_vecPPolygons[i]->m_vecPnts.erase(TempLine->m_vecPPolygons[i]->m_vecPnts.begin()+vec_p_num[best].num,
								TempLine->m_vecPPolygons[i]->m_vecPnts.begin()+vec_p_num[best].num+2);
						}
					}
				}
				else
				{
				Imbreask:
					SameCLine->m_vecPPolygons.push_back(poly);
					break;
				}

				if (i==TempLine->m_vecPPolygons.size()-1)
				{
					goto Imbreask;
				}
			}
		}
		else
		{
			TempLine->m_vecPPolygons.erase(TempLine->m_vecPPolygons.begin());
		}		
	}while(!TempLine->m_vecPPolygons.empty());
	//OptiLine(SameCLineTemp,SameCLine,FillLineWidth);
}



void CLine::OptiLine(PPOLYPOLYGON SameCLineTemp,PPOLYPOLYGON SameCLine,double FillLineWidth)
{
	/*for (int i=0;i<SameCLineTemp->m_vecPPolygons.size();i++)
	{
		for (int j=i+1;j<SameCLineTemp->m_vecPPolygons.size();j++)
		{
			POINT3D p1=SameCLineTemp->m_vecPPolygons[i]->m_vecPnts.back();
			POINT3D p2=SameCLineTemp->m_vecPPolygons[j]->m_vecPnts[0];
			VECTOR3D v=p1-p2;
			int temp=i;
			if (v.GetLength()<1.5*FillLineWidth)
			{
				SameCLineTemp->m_vecPPolygons[i]->m_vecPnts.insert(SameCLineTemp->m_vecPPolygons[i]->m_vecPnts.end(),
					SameCLineTemp->m_vecPPolygons[j]->m_vecPnts.begin(),SameCLineTemp->m_vecPPolygons[j]->m_vecPnts.end());
				SameCLineTemp->m_vecPPolygons.erase(SameCLineTemp->m_vecPPolygons.begin()+j);
				j--;
			}
			else
			{
				POINT3D p1=SameCLineTemp->m_vecPPolygons[i]->m_vecPnts[0];
				POINT3D p2=SameCLineTemp->m_vecPPolygons[j]->m_vecPnts.back();
				VECTOR3D v=p1-p2;
				if (v.GetLength()<2*FillLineWidth)
				{
					PPOLYGON poly;
					poly=SameCLineTemp->m_vecPPolygons[i];
					SameCLineTemp->m_vecPPolygons[i]=SameCLineTemp->m_vecPPolygons[j];
					SameCLineTemp->m_vecPPolygons[j]=poly;
					SameCLineTemp->m_vecPPolygons[i]->m_vecPnts.insert(SameCLineTemp->m_vecPPolygons[i]->m_vecPnts.end(),
						SameCLineTemp->m_vecPPolygons[j]->m_vecPnts.begin(),SameCLineTemp->m_vecPPolygons[j]->m_vecPnts.end());
					SameCLineTemp->m_vecPPolygons.erase(SameCLineTemp->m_vecPPolygons.begin()+j);
					j--;
				}
			}
		}
		SameCLine->m_vecPPolygons.push_back(SameCLineTemp->m_vecPPolygons[i]);
	}*/

}






void CLine::SortXmin(PPOLYGON singleLine)
{
	POINT3D		TempP; 
	int sz = singleLine->m_vecPnts.size();
	for(int i=0;i<sz-1;i++) //冒泡升序
	{ 
		for (int j= 0;j<sz-1-i;j++) 
		{
			if (singleLine->m_vecPnts[j].x>singleLine->m_vecPnts[j+1].x) 
			{ 
				TempP=singleLine->m_vecPnts[j+1]; 
				singleLine->m_vecPnts[j+1]=singleLine->m_vecPnts[j]; 
				singleLine->m_vecPnts[j]=TempP;
			} 
		}	
	} 
}


//void Cpadding::DrawPadding(COpenGLDC* pDC)
//{
//	int m_paddSz=m_vecALLPaddingP.size();
//	for (int i= 0 ; i<m_paddSz;i++)
//	{
//
//		int AreaSZ=m_vecALLPaddingP[i]->m_vecPPolygons.size();
//		for (int j=0; j<AreaSZ;j++)
//		{
//			int PointSZ=m_vecALLPaddingP[i]->m_vecPPolygons[j]->m_vecPnts.size();
//			for (int z = 0 ;z<PointSZ-1;z++)
//			//for (int z=0;z<PointSZ/2;z++)
//			{
//
//				COLORREF old_clr1;
//				COLORREF old_clr2;
//				pDC->GetColor(old_clr1);
//				pDC->GetMaterialColor(old_clr2);
//				pDC->SetMaterialColor(RGB(0,0,0));
//				pDC->SetColor(RGB(0,0,0));//黄色
//				pDC->DrawLine(m_vecALLPaddingP[i]->m_vecPPolygons[j]->m_vecPnts[z],
//					m_vecALLPaddingP[i]->m_vecPPolygons[j]->m_vecPnts[z+1]);
//				/*pDC->DrawLine(m_vecALLPaddingP[i]->m_vecPPolygons[j]->m_vecPnts[z*2],
//							  m_vecALLPaddingP[i]->m_vecPPolygons[j]->m_vecPnts[z*2+1]);*/
//
//				pDC->SetMaterialColor(old_clr2);
//				pDC->SetColor(old_clr1);
//
//			}
//		}
//		//set<SORTX>::iterator iterS;
//		//for (iterS=m_vecpaddN[i].begin();iterS !=m_vecpaddN[i].end();iterS++)
//		//{
//
//		//	POINT3D a1 ;
//		//	POINT3D a2 ;
//		//	a1 = iterS->a;//取出a1、a2两个交点
//		//	iterS++;
//		//	a2 = iterS->a;		
//		//	int i=1;
//		//	COLORREF old_clr;
//		//	pDC->GetColor(old_clr);
//		//	pDC->GetMaterialColor(old_clr);
//		//	pDC->SetMaterialColor(RGB(255,215,0));
//		//	pDC->SetColor(RGB(255,215,0));//黄色
//		//	pDC->DrawLine(a1,a2);//变色，画线
//		//	pDC->SetMaterialColor(old_clr);
//		//	pDC->SetColor(old_clr);			
//		//}
//	}
//}

bool CLine::CrossY(POINT3D p3,POINT3D p4, double y,POINT3D &p)
{
	double min,max;
	min=p3.y;
	max=p3.y;

	if(p4.y<min)  min = p4.y;
	if(p4.y>max)  max = p4.y;

	double k;
	k=(y - p3.y)/(p4.y - p3.y);
	if((y-min)>=- CAD_ZERO&&(max-y)>=CAD_ZERO)
		//if(min<=y && max>=y)
	{
		p.x = k*(p4.x - p3.x) + p3.x ;
		p.y = y ;
		p.z = k*(p4.z - p3.z) + p3.z ;
		return TRUE;
	}
	else	
		return FALSE;
}

PPOLYPOLYGON CLine::tailor(PPOLYGON contour,PPOLYGON curve)
{

	PPOLYPOLYGON  polypoly;
	polypoly=new POLYPOLYGON;
	vector<int>   vec_InterNum;
	vector<POINT3D>  vec_InterP;
	for (int i=0;i<curve->m_vecPnts.size()-1;i++)
	{
		vector<int>  vec_tempNum;
		vector<POINT3D>  vec_temP;
		for (int j=0;j<contour->m_vecPnts.size()-1;j++)
		{
			POINT3D p1=curve->m_vecPnts[i];
			POINT3D p2=curve->m_vecPnts[i+1];
			POINT3D p3=contour->m_vecPnts[j];
			POINT3D p4=contour->m_vecPnts[j+1];
			POINT3D interP;
			if (KitTool::lineSegInter(p1,p2,p3,p4,interP))
			{
				vec_tempNum.push_back(i);
				vec_temP.push_back(interP);
			}	
		}
		if (vec_tempNum.size()>1)
		{
			VECTOR3D  v1=  vec_temP[0]-curve->m_vecPnts[i] ;
			VECTOR3D  v2=  vec_temP.back()-curve->m_vecPnts[i];
			if (v1.GetLength()>v2.GetLength())
				reverse( vec_temP.begin(), vec_temP.end());
			vec_InterNum.insert(vec_InterNum.end(),vec_tempNum.begin(),vec_tempNum.end());
			vec_InterP.insert(vec_InterP.end(),vec_temP.begin(),vec_temP.end());
		}
		else
		{
			vec_InterNum.push_back(vec_tempNum[0]);
			vec_InterP.push_back(vec_temP[0]);
		}
	}
	for (int i=0;i<vec_InterNum.size();i++)
	{
		if (i%2==0)
		{
			PPOLYGON poly;
			poly = new POLYGON;
			poly->m_vecPnts.push_back(vec_InterP[i]);
			if (i==vec_InterNum.size()-1)
			{
				int num1=vec_InterNum[i];
				for (int j=num1+1;j<curve->m_vecPnts.size();j++)
				{
					poly->m_vecPnts.push_back(curve->m_vecPnts[j]);
				}
			}
			else 
			{
				int num1=vec_InterNum[i];
				int num2=vec_InterNum[i+1];
				for (int j=num1+1;j<num2;j++)
				{
					poly->m_vecPnts.push_back(curve->m_vecPnts[j]);
				}
				poly->m_vecPnts.push_back(vec_InterP[i+1]);
			}
		}
	}
	return polypoly;
}