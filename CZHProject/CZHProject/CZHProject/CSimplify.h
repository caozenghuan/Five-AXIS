#pragma once
#include "STLModell.h"
#include "datastructure/mesh.h"
#include "datastructure/solidMesh.h"
#include "datastructure/solidPolygon.h"
#include "KitTool.h"
#include "datastructure/GeomBase.h"
#include "time.h" 
#include <omp.h>
#include "eigen\Eigen"
#include "viewspace\QRoundProgressBar.h"

#include "Timer.h"
using namespace Eigen;
using Eigen::MatrixXd;

////////////////////接口类
#define    EDGESUB         0
#define    ME           1
#define    SMOOTH       2
#define    ZHANGXING    3
class CSimplify
{
public:
	CSimplify();
	virtual ~CSimplify();
protected:

	//预先定义边与边的比较运算法则
	struct Compedge
	{
		bool operator ()(const PEDGE pE1, const PEDGE pE2)
		{
			if (pE1->pVertEnd->m_id == pE2->pVertStart->m_id)
			{
				if (pE1->pVertStart->m_id != pE2->pVertEnd->m_id)
				{
					return true;
				}
				else{ return false; }
			}
			else
			{
				return false;
			}
			return false;
		}
	};
	struct ErrorValue
	{
		double Valchoose;
		PVERT P0;
		ErrorValue()
		{
			Valchoose = 0;
			P0 = new VERT;
		}
	};

public:
	double featureTimeTemp, TriangleTimeTemp;
	double featureTime, TriangleTime;
	vec_POINT3D edgepoint;

public:
	KitTool *Kit;
	//////////////网格简化////////////////////////

	void GetsimpleResult(STLModell *pMeshTri, STLModell *pSimple, int simpway, int R, int ratepercent, QProgressBar  *progressBar);//进行迭代获得简化结果
	void EvaluatemeshQuality(STLModell *pMeshTri, double &neq);//网格正则度
	void PushUntreatedFace(vector<PFACETTRI>  vec_Temp, STLModell *pSimple, map<VERT, int>map_Vert,
		map<VERT, int>::iterator iter, int VetxID, int Onetime);//将未处理的面片存入容器

	////////////////边折叠简化（碟形）方式/////////////////////
	void EdgesimpleResult(STLModell *pMeshTri, STLModell *pSimple, float R);//获得边简化结果
	BOOL Judgelegal(PEDGE Pedge, vector<PFACETTRI> &vecper);//判断一条边是否可以被折叠
	void AssortTriangle(vector<PEDGE>  vecPedge);//识别碟形三角面片并进行标记
	void CalerrorLittleValue(PHEDGE Ph0, PVERT p0);///保存三角形折叠误差和保存顶点的函数
	MatrixXd Planeeuate(PFACETTRI PFc0);//求一个三角面片所在的平面方程
	void Planeeuate(PFACETTRI PFc0, vector<double> &facepare);//最小二乘拟合求平面方程
	void CalValue_pointplane(PHEDGE Ph0, vec_PFACETTRI vecper);
	//////////////////////三角面片简化方式///////////////////////
	/*我的简化算法*/
	void TrianglesimpleResultMe(STLModell *pMeshTri, STLModell *pSimple, float R, int Onetime);//获得面片简化结果
	void MarkparticularArea(STLModell *pMeshTri, STLModell *pSimple, map<VERT, int> &map_Vert,
		map<VERT, int>::iterator &iter, int &VetxID);//SOD搜索算法标记特征区域并折叠特征区域
	BOOL Markboundary(STLModell *pMeshTri, STLModell *pMeshDate);//标记并折叠边界区域、特征区域
	void CalvalueAndpoint(vector<PFACETTRI> vecPfact);//计算三角形折叠后新顶点和折叠代价
	BOOL Judgelegal(POINT3D p0, POINT3D p1, POINT3D p2);//判断边界是否可以被折叠
	BOOL Judgelegal(PFACETTRI PFc0);//判断一个三角形是否可以被折叠

	void AcqnewVertex_curdegree(PFACETTRI Pfact);//曲度插值获取内部三角形新顶点
	void AcqnewVertex_Oneofthree(PFACETTRI Pfact, vec_PFACETTRI vecper);//三点选一获取新顶点
	void AcqnewVertex_Gravity(PFACETTRI Pfact);//重心作为新顶点
	void CalValue_pointplane(PFACETTRI Pfact,vec_PFACETTRI vecper);//二次误差测度改进版
	void CalValue_Wmsa(PFACETTRI pFac);//三点加权平均得法矢，点面距离
	void CalValue_Fitcurve(PFACETTRI pFac);//最小二乘曲面拟合
	void CalValue_Fitplane(PFACETTRI pFac);//最小二乘平面拟合
	void CalValue_QEM(PFACETTRI pFac, vec_PFACETTRI vecper);//二次误差测度


	///////////////一种基于不平滑度的三角形简化算法////////////////
	void TrianglesimplesmoothResult(STLModell *pMeshTri, STLModell *pSimple, float R);//获得边简化结果
	void CalSmoothDegree(PVERT p0, vec_PVERT OneRingvert);//计算网格顶点的不平滑度
	void CalerrorLittleValue(vector<PFACETTRI>  vecPfact);///保存三角形折叠误差和保存顶点的函数
	BOOL Judgelegalsmooth(PFACETTRI PFact, vector<PFACETTRI> &vecper);//判断一个三角形是否可以被折叠
	BOOL EliminatetheReapetedFace(PFACETTRI Pf0, vector<PFACETTRI> vecper);//去除面折叠后可能重复的面

	///////////////一种基于特征保持的的三角形简化算法////////////////
	void TrianglesimpleResult(STLModell *pMeshTri, STLModell *pSimple, float R);
	void AssortTriangle(vector<PFACETTRI> vecPfact, double &Lsmin, double &Lsmax);
	BOOL EliminatetheReapetedFace(PVERT p0, PVERT p1, int j, vector<PFACETTRI> vecper);//去除面折叠后可能重复的面
	void CalerrorLittleValue(PFACETTRI Pf0, PVERT p0);
	void Checkparticular(PFACETTRI &PFc0, double t0);/////////判断一个三角形是否为特征三角形
	BOOL Judgelegal(PFACETTRI PFc0, vector<PFACETTRI> &vecper);//合法性检查
	double Longdegree(PFACETTRI PFact);//计算狭长度
	double SumRingArea(PVERT P0);//计算点的一阶邻域总面积
	double SumRingPfacArea(PFACETTRI PFact);//计算面片的一阶邻域总面积
	double Lshapedegree(PFACETTRI PFact);//计算三角形的局部尖锐度
	void AcqnewVertex(PFACETTRI PFact, double &value);//获取内部三角形新顶点
};

