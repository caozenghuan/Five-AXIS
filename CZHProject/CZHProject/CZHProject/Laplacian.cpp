
#include "Laplacian.h"
using namespace Eigen;//命名空间很重要

void CLaplacian::change(vec_PVERT& m_vecLpvert, STLModell* &pCSTLModel, vector<POINT3D>& m_vecLp, vec_VECTOR3D& m_vecV)
{
	//////////保存原面片信息
	for (int i=0;i<pCSTLModel->m_vecPVert.size();i++)
	{
		POINT3D temp;
		temp.x=pCSTLModel->m_vecPVert[i]->x;
		temp.y=pCSTLModel->m_vecPVert[i]->y;
		temp.z=pCSTLModel->m_vecPVert[i]->z;
		m_vecLp.push_back(temp);
	}
	for (int i=0;i<pCSTLModel->m_vecPFacetTri.size();i++)
	{
		VECTOR3D temp;
		temp=*pCSTLModel->m_vecPFacetTri[i]->m_PFacetNorm;
		m_vecV.push_back(temp);
	}
	//////////////////////////////
	int numN=pCSTLModel->m_vecPVert.size();
	//构造增广矩阵
	vector<int> m_vecSL;
	selectP(pCSTLModel,m_vecSL);//选取控制顶点
	vec_PVERT vecpVer;
	int numM=m_vecSL.size();
	//构造系数矩阵L
	vector<T> tripletList;
	for (int i=0;i<numN;i++)
	{
		KitTool::FindOneRing(pCSTLModel->m_vecPVert[i],vecpVer);
		//Ccalcubase::FindTwoRing(pCSTLModel->m_vecPVert[i],vecpVer);
		for (int j=0;j<numN;j++)
		{
			if (j==i)
			{
				tripletList.push_back(T(i,j,1));
			}
			else
			{
				int num=0;
				for (int k=0;k<vecpVer.size();k++)
				{
					if (pCSTLModel->m_vecPVert[j]==vecpVer[k])
					{
						tripletList.push_back(T(i,j,-((double)1/(double)vecpVer.size())));
						break;
					}
				}
			}
		}
	}
	for (int i=0;i<numM;i++)
	{
		for (int j=0;j<numN;j++)
		{
			if (j==m_vecSL[i])
			{
			   tripletList.push_back(T(numN+i,j,0.05));
			}
		}
	}
	SparseMatrix<double> A(numN + numM, numN), AT(numN, numN + numM), LA(numN, numN);
	A.setFromTriplets(tripletList.begin(), tripletList.end());//// 初始化稀疏矩阵
	AT=A.transpose();
	LA=AT*A;
	//右边
	VectorXd bx(numN+numM), by(numN+numM),bz(numN+numM);
	VectorXd Lbx(numN+numM), Lby(numN+numM),Lbz(numN+numM);
	for (int i=0;i<numN;i++)
	{
		bx(i)=0;
		by(i)=0;
		bz(i)=0;
	}
	for (int i=0;i<numM;i++)
	{
		bx(numN+i)=pCSTLModel->m_vecPVert[m_vecSL[i]]->x;
		by(numN+i)=pCSTLModel->m_vecPVert[m_vecSL[i]]->y;
		bz(numN+i)=pCSTLModel->m_vecPVert[m_vecSL[i]]->z;
	}
	Lbx=AT*bx;Lby=AT*by;Lbz=AT*bz;
	//求解
	SimplicialCholesky<SpMat> chol(LA);
	VectorXd xx= chol.solve(Lbx); 
	VectorXd xy= chol.solve(Lby); 
	VectorXd xz= chol.solve(Lbz); 
	for (int i=0;i<numN;i++)
	{
		m_vecLpvert[i]->x=xx(i);
		m_vecLpvert[i]->y=xy(i);
		m_vecLpvert[i]->z=xz(i);
	}
	//改变面片矢量
	VECTOR3D n1,n2,n3;
	for (int i=0;i<pCSTLModel->m_vecPFacetTri.size();i++)
	{
		n1.dx=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->x-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[0]->x;
		n1.dy=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->y-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[0]->y;
		n1.dz=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->z-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[0]->z;

		n2.dx=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[2]->x-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->x;
		n2.dy=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[2]->y-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->y;
		n2.dz=pCSTLModel->m_vecPFacetTri[i]->m_PVerts[2]->z-pCSTLModel->m_vecPFacetTri[i]->m_PVerts[1]->z;
		n3=n1*n2;
		n3.Normalize();
		//n3.dx=-n3.dx;n3.dy=-n3.dy;n3.dz=-n3.dz;
		*pCSTLModel->m_vecPFacetTri[i]->m_PFacetNorm=n3;
	}
}
void CLaplacian::selectP(STLModell* &pCSTLModel, vector<int>& m_vecSL)
{
	//根据平均曲率排序
	set<double> set_point;//所有点的平均曲率
	map<double,int> map_PinttoID;//平均曲率对应点
	for (int i=0;i<pCSTLModel->m_vecPVert.size();i++)
	{
		double ki=0;
		//ki=Ccalcubase::CalcuVerMeanCurvature(pCSTLModel->m_vecPVert[i]);
		//ki=Ccalcubase::calcuPOINTV(pCSTLModel->m_vecPVert[i]);
		ki = KitTool::CalcuPOINTV(pCSTLModel->m_vecPVert[i]);
		map_PinttoID.insert(pair<double,int>(ki,i));
		set_point.insert(ki);
	}
	set<double>::iterator it_set;
	it_set=set_point.begin();
	int num=0;
	int allnum=0.3*pCSTLModel->m_vecPVert.size();//百分之三十控制顶点
	//int allnum=0.2*pCSTLModel->m_vecPVert.size();
	while(num<=allnum)
	{
		m_vecSL.push_back(map_PinttoID[*it_set]);
		it_set++;
		num++;
	}
}
void CLaplacian::back(vec_PVERT& m_vecLpvert, STLModell* &pCSTLModel, vector<POINT3D>& m_vecLp, vec_VECTOR3D& m_vecV)
{
	for (int i=0;i<m_vecLpvert.size();i++)
	{
		m_vecLpvert[i]->x=m_vecLp[i].x;
		m_vecLpvert[i]->y=m_vecLp[i].y;
		m_vecLpvert[i]->z=m_vecLp[i].z;
	}
	for (int i=0;i<pCSTLModel->m_vecPFacetTri.size();i++)
	{
		pCSTLModel->m_vecPFacetTri[i]->m_PFacetNorm->dx=m_vecV[i].dx;
		pCSTLModel->m_vecPFacetTri[i]->m_PFacetNorm->dy=m_vecV[i].dy;
		pCSTLModel->m_vecPFacetTri[i]->m_PFacetNorm->dz=m_vecV[i].dz;
	}
}