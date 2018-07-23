#include "helical_path.h"


ChelicalPath::ChelicalPath(void)
{

}
ChelicalPath::~ChelicalPath(void)
{

}

void ChelicalPath::GetheilcalPath(vec_PPOLYPOLYGON   &m_vecFillP)
{  
	vec_PPOLYPOLYGON   tempFillP;
	int Censz =m_vecFillP.size();
	for (int i =0;i<Censz;i++)
	{
		PPOLYPOLYGON	NewPPolygons;
		NewPPolygons = new POLYPOLYGON;
		int OffsetSz=m_vecFillP[i]->m_vecPPolygons.size();
		int classifySZ = m_vecFillP[i]->m_vecPPolygons.back()->m_Iclassify;
		for (int classifyNum =1;classifyNum<=classifySZ;classifyNum++)
		{
			vec_PPOLYGON	vecTempPPolygons;	
			for (int j =0; j<OffsetSz;j++ )
			{
				if (m_vecFillP[i]->m_vecPPolygons[j]->m_Iclassify==classifyNum)
				{
					vecTempPPolygons.push_back(m_vecFillP[i]->m_vecPPolygons[j]);
				}
			}
			becomeheilcalPath(vecTempPPolygons,NewPPolygons);
		}
		tempFillP.push_back(NewPPolygons);
	}
	m_vecFillP=tempFillP;
}




void ChelicalPath::becomeheilcalPath(vec_PPOLYGON vecTempPPolygons,PPOLYPOLYGON NewPPolygons)
{

	PPOLYGON ploy;
	ploy=new POLYGON;
	int TempSz = vecTempPPolygons.size();
	double offsetdv;
	int FirstPolySZ= vecTempPPolygons[0]->m_vecPnts.size();
	for (int i=0;i<FirstPolySZ;i++ )
	{
		ploy->m_vecPnts.push_back(vecTempPPolygons[0]->m_vecPnts[i]);

	}
	int sz=vecTempPPolygons.size();
	for (int i=0;i<sz-1;i++)
	{
		int sz1=vecTempPPolygons[i]->m_vecPnts.size();
		int sz2=vecTempPPolygons[i+1]->m_vecPnts.size();
		if (sz1>=sz2)
		{
			double Perimeter=GetChordLength(vecTempPPolygons[i],sz1-1);
			for (int j =0;j<sz1;j++)
			{
				if (j==0)
				{
					ploy->m_vecPnts.push_back(vecTempPPolygons[i]->m_vecPnts[0]);
				} 
				else
				{
					if (j==sz1)
					{
						ploy->m_vecPnts.push_back(vecTempPPolygons[i+1]->m_vecPnts[0]);
					}
					else
					{
						double ChordLength=GetChordLength(vecTempPPolygons[i],j);
						double t=ChordLength/Perimeter;
						POINT3D Pmin=MinDofPoint2piont(vecTempPPolygons[i]->m_vecPnts[j],vecTempPPolygons[i+1]);
						VECTOR2D offsetNorm;
						offsetNorm.dx=Pmin.x-vecTempPPolygons[i]->m_vecPnts[j].x;
						offsetNorm.dy=Pmin.y-vecTempPPolygons[i]->m_vecPnts[j].y;
						POINT3D p;
						p.x=vecTempPPolygons[i]->m_vecPnts[j].x+t*offsetNorm.dx;
						p.y=vecTempPPolygons[i]->m_vecPnts[j].y+t*offsetNorm.dy;
						p.z=Pmin.z;
						ploy->m_vecPnts.push_back(p);
					}
				}			
			}
		}
		else
		{
			double Perimeter=GetChordLength(vecTempPPolygons[i+1],sz2-1);
			for (int j =0;j<sz2;j++)
			{
				if (j==0)
				{
					ploy->m_vecPnts.push_back(vecTempPPolygons[i]->m_vecPnts[0]);
				} 
				else
				{
					if (j==sz2)
					{
						ploy->m_vecPnts.push_back(vecTempPPolygons[i+1]->m_vecPnts[0]);
					}
					else
					{
						double ChordLength=GetChordLength(vecTempPPolygons[i+1],j);
						double t=1-ChordLength/Perimeter;
						POINT3D Pmin=MinDofPoint2piont(vecTempPPolygons[i+1]->m_vecPnts[j],vecTempPPolygons[i]);
						VECTOR2D offsetNorm;
						offsetNorm.dx=Pmin.x-vecTempPPolygons[i+1]->m_vecPnts[j].x;
						offsetNorm.dy=Pmin.y-vecTempPPolygons[i+1]->m_vecPnts[j].y;
						POINT3D p;
						p.x=vecTempPPolygons[i+1]->m_vecPnts[j].x+t*offsetNorm.dx;
						p.y=vecTempPPolygons[i+1]->m_vecPnts[j].y+t*offsetNorm.dy;
						p.z=Pmin.z;
						ploy->m_vecPnts.push_back(p);
					}
				}
			}
		}
	}
	int lastPolySZ= vecTempPPolygons.back()->m_vecPnts.size();
	for (int i=0;i<lastPolySZ;i++ )
	{
		ploy->m_vecPnts.push_back(vecTempPPolygons.back()->m_vecPnts[i]);

	}
	//DeleteSelfing(ploy);
	NewPPolygons->m_vecPPolygons.push_back(ploy);
}



double ChelicalPath::GetChordLength(PPOLYGON poly,int StopI)
{
	double sum=0;
	for (int i =0;i<StopI;i++)
	{
		VECTOR3D v= poly->m_vecPnts[i+1]-poly->m_vecPnts[i];
		double S=v.GetLength();
		sum=sum+S;
	}
	return sum;
}

//void ChelicalPath::DrawheilcalPath(COpenGLDC* pDC)
//{
//
//	int CN = m_vecALLhelical.size();
//	COLORREF old_clr;
//	pDC->GetColor(old_clr);
//	pDC->GetMaterialColor(old_clr);
//	pDC->SetMaterialColor(RGB(0,0,0));
//	pDC->SetColor(RGB(0,0,0));//紫色
//	for (int z=0;z<CN;z++)
//	{
//		int sameZfcN = m_vecALLhelical[z]->m_vecPPolygons.size();
//		for (int i=0;i<sameZfcN;i++)
//		{
//			int fcN=m_vecALLhelical[z]->m_vecPPolygons[i]->m_vecPnts.size();
//			for (int j =0 ; j<fcN-1;j++ )
//			{
//				POINT3D a1;
//				POINT3D a2;
//				a1= m_vecALLhelical[z]->m_vecPPolygons[i]->m_vecPnts[j];
//				a2= m_vecALLhelical[z]->m_vecPPolygons[i]->m_vecPnts[j+1];
//				pDC->DrawLine(a1,a2);//变色，画线
//			}
//		}
//
//	}
//	pDC->SetMaterialColor(old_clr);
//	pDC->SetColor(old_clr);
//}


POINT3D ChelicalPath::MinDofPoint2piont(POINT3D p,PPOLYGON ploy)
{

	POINT3D p1=ploy->m_vecPnts[0];
	VECTOR3D v1=p-ploy->m_vecPnts[0];
	double MinD=v1.GetLength();
	int Sz= ploy->m_vecPnts.size();
	for (int i =1;i<Sz-1;i++)
	{
		VECTOR3D v1=p-ploy->m_vecPnts[i];
		double TempD=v1.GetLength();
		if (TempD<MinD)
		{
			MinD=TempD;
			p1=ploy->m_vecPnts[i];
		}
	}
	return p1;
}


