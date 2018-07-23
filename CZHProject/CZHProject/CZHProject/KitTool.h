#pragma once
#include "STLModell.h"
#include "datastructure/mesh.h"
#include "datastructure/GeomBase.h"
#include "datastructure/solidMesh.h"
#include "datastructure/solidPolygon.h"
#include "K-d tree/Vec.h"
#include <queue>  
#include<iostream>  
#include<queue>  
#include<stdlib.h>  
using namespace std;
class KitTool
{
public:
	KitTool();
	virtual ~KitTool();

public:

	//////////////////排序函数工具///////////////////////
	 static void Ranktwodigit(double &d1, double &d2);//比较两个数的大小
	 static void Rankthreedigit(PFACETTRI &PFc0, double d0, double d1, double d2);//比较三个数的大小
	 static void Rankthreedigit(PFACETTRI &PFc0, vector<double> re,double &value);//比较三个数的大小,并返回一个最小值
	 void Ranktwopoint(PHEDGE &PHEdge_1, PHEDGE &PHEdge_2);
	 void Ranktwoedge(PHEDGE &PHEdge_0, PHEDGE PHEdge_1, PHEDGE PHEdge_2);
	 static bool RankPfact(PFACETTRI Pfact_0, PFACETTRI Pfact_1);//确定排序规则
	 static bool RankPfact1(PFACETTRI Pfact_0, PFACETTRI Pfact_1);//确定排序规则(按照折叠误差大小排序)
	 static void Thelest(PFACETTRI Pfact_0, float &Zmin);
	 static void RankAllfact(STLModell* pMeshTri, vector<PFACETTRI> &vec_fac); //按照三个顶点的最小Z值大小排序
	 static void RankAlledge(vector<PEDGE> &vec_edg);//对边表进行排序
	 static bool RankPedge(PEDGE edge_0,PEDGE edge_1);//确定排序规则
	 static bool Comparepoint(PFACETTRI Pfact_0, POINT3D p0);//比较三个向量是否相等
	 static void  RankCircle(PPOLYGON &ploy);

	 static void closedBoundary(vec_PHEDGE m_vecPHEdge, PPOLYPOLYGON polypoly);//对半边的顶点进行排序
	 /////////////////计算平面与三角面片的交点函数////////////////////
	 static bool CalPlanePointIntPoint(POINT3D p00, POINT3D p10, float Zt, POINT3D &p);//水平面与三角面片的交点
	 static bool CalPlaneLineIntersectPoint(POINT3D linePoint, POINT3D linePointend, float Zt, POINT3D &p);
	 static bool CalPlaneLineIntPoint(POINT3D p00, POINT3D p10, POINT3D plane_point, VECTOR3D dir, POINT3D &p, int &vertType);//任意平面与三角面片的交点
	 static bool CalPlaneLineIntPoint(POINT3D p00, POINT3D p10, POINT3D plane_point, VECTOR3D dir, POINT3D &p);//任意平面与三角面片的交点
	 static VECTOR3D PlanePointDistance(POINT3D p0, POINT3D p1, VECTOR3D u); ///点到平面的距离函数
	 static double  PlanePointDistance(POINT3D p0, double a, double b, double c, double d)//点到平面的距离
	 {
		 return fabs(a*p0.x + b*p0.y + c*p0.z + d) / sqrt(a*a + b*b + c*c);

	 };
	 //////////////三角面片的周长、面积/////////
	 static void Gettriperimeter(PHEDGE ph0, double &cir);//已知一边计算三角形的周长
	 static double AreaTri(PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3);//计算面片的面积
	 static double AreaTri(POINT3D pPoint1, POINT3D pPoint2, POINT3D pPoint3);//计算面片的面积
	 static double AreaTri(PVERT pPoint1, PVERT pPoint2, POINT3D pPoint3);//计算面片的面积
	 static double AreaTri(PVERT pPoint1, POINT3D pPoint2, POINT3D pPoint3);//计算面片的面积
	 static double GetTriArea(PFACETTRI ptri);/////////////////获得三角面片的面积
	 /////////三角面片顶点和空间点之间的转化函数/////////////
	 static POINT3D PVERTtoPOINT3D(PVERT pv);
	 static void POINT3DtoPVERT(POINT3D p, PVERT pv);

	 static double CalGaussCurvature(PVERT pVer);///计算顶点的高斯曲率
	 static double CalcuVerMeanCurvature(PVERT pVer);///计算顶点的平均曲率
	 static double AreaTriMixSum(PVERT pVer);//一阶邻域混合面积总和
	 static double AreaTriMix(PPOINT3D pPoint1, PPOINT3D pPoint2, PPOINT3D pPoint3);//计算顶点一阶领域锐角三角形的混合面积（三角以外心为界分割，与顶点[pPoint1]相邻的部分的面积）
	 static POINT3D Center(PHEDGE trigle);///计算面片的中心点
	 static PVECTOR3D CalcuPfacNormal(PVERT pVer0, PVERT pVer1, PVERT pVer2);///计算面片的法矢
	 static VECTOR3D CalcuPfacNormal(POINT3D pVer0, POINT3D pVer1, POINT3D pVer2);///计算面片的法矢
	 static VECTOR3D CalcuVerNormal(PVERT pVer);////计算顶点的法矢,以点相邻面的面积和相邻夹角为权值（三角面片）
	 static VECTOR3D CalcuVerNormal(PVERT pVer, PFACETTRI pFac);////估算空间点的法矢

	 static double CalcuPointCurve(PVERT A);////计算顶点曲度值
	 static BOOL FindOneRing(PVERT& pVer, vec_PVERT& vecpVer);////构建顶点的一阶邻域顶点
	 static BOOL FindOneRing_New(PVERT& pVer, vec_PVERT& vecpVer);////构建顶点的一阶邻域顶点(简单算法)
	 static BOOL FindOneRing_Me(PVERT& pVer, vec_PVERT& vecpVer);////构建顶点的一阶邻域顶点(我的算法)

	 static BOOL FindOnePH(PVERT A, vec_PHEDGE& vecpH);//构建顶点的一阶邻域半边
     static BOOL FindOneRFac(PVERT pVer, vec_PFACETTRI& vecpFac);//构建顶点的一阶邻域面片.  这条函数可以去除了。
	 static BOOL FindTwoRing_new(PVERT pVer, vec_PVERT& vecpVerT);//计算网格点的二阶邻域点
	 static BOOL FindTwoRing(PVERT pVer, vec_PFACETTRI& vecpFacT);
	 static void FindedgeOnering(PHEDGE Ph0, vector<PFACETTRI> &vecper);//寻找半边的一阶邻域面片
	 static void FindedgeOneringvertex_Me(PHEDGE Ph0,vec_PVERT& vecpVer);////构建半边的一阶邻域顶点(我的算法)
	 static BOOL FindPOneRFAC(PFACETTRI pFac, vec_PFACETTRI& vecpFacP);///构建面片的一阶邻域面片
	 static BOOL FindPOneRFAC_NEW(PFACETTRI pFac, vec_PFACETTRI& vecpFacP);///构建面片的一阶邻域面片
	 static BOOL FindPOneRVetex(PFACETTRI pFac, vec_PVERT& vecpVer);///构建面片的一阶邻域顶点
	 static void FindPtwoRFAC(PFACETTRI pFac, vec_PFACETTRI& vecpFacP);///构建面片的二阶邻域面片

	 static BOOL FindFourRing(PVERT pVer, vec_PVERT& vecpVerT);//计算网格点的四阶邻域点

	 static BOOL KitTool::FinOneFac_Less(PFACETTRI pfc0);


	 static double CalcuPOINTV(PVERT A);//计算顶点的凹凸度
	 static void create_current_Frenet(MATRIX3D& currentM, VECTOR3D LineNor, POINT3D Center);//建立局部坐标系,返回一个局部坐标系
	 static void create_BACK_Frenet(MATRIX3D& currentB, VECTOR3D LineNor, POINT3D Center);//局部坐标系还原成世界坐标
	 static void create_current_matriX(MATRIX3D& matriX, double a);//绕X旋转a度
	 static void create_current_matriZ(MATRIX3D& matriZ, double a);//建立绕Z轴旋转的坐标
	 void cone_delel(vector<double>& vec_dis);//智能点的搜寻
	 void create_cone_new(vector<VECTOR3D>& m_vecVECTOR3D, VECTOR3D LineNorZ, VECTOR3D LineNor, POINT3D orig, double angA, int numB);
	 BOOL IntersectTriangle(POINT3D orig, VECTOR3D dir, POINT3D v0, POINT3D v1, POINT3D v2);//射线求交
	 void Dijkstra_Point_ST_EN_new(PVERT ST, PVERT EN, map<PVERT, PVERT>& Path, vector<PVERT>& BLOOP, vec_PVERT& m_vecPVert);//两点之间最短路径
	double dis(POINT3D A, POINT3D B);

	void ClearTheList(STLModell *pMeshTriDate);
	static void ClearTheList(Blockpath &OneBlock);
	void StoretheMeshTri(STLModell *pMeshTri, STLModell *pMeshTriDat, int Onetime);
	static void StoreFactandVer(vec_PFACETTRI vec_Fact, vec_PFACETTRI &vec_PFact, vec_PVERT &m_vecPVert, map<VERT, int> &map_Vert, map<VERT, int>::iterator &iter, int &vetexID);
	static void CreateEdgeTopByFctVert(Blockpath &OneBlock);//创建块的拓扑信息
	static bool JudgeFaceDir(POINT3D v00, POINT3D v10, POINT3D v20, VECTOR3D FaceDir);//判断面片旋向是否正确

	static bool judeorientation(PPOLYGON poly);
	static bool pointInpoly(POINT3D p, PPOLYGON poly);
	static double GetPolypolyArea(PPOLYPOLYGON ppolypolygon);
	static double GetPolyArea(PPOLYGON poly);
	static void  RotateAllFacet(vector<PFACETTRI> &OneBlock, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转三角面片容器
	static void  RotatePolygon(vec_PPOLYPOLYGON pployploy, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转多模型
	static void  RotateOneRingPolygon(PPOLYGON ploy, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转环
	static void  RotateTwoRingPolygon(PPOLYPOLYGON vec_poly, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转环
	static 	void FindCircleNearPoint(POINT3D p, POINT3D &NearP, PPOLYGON poly);//求最近距离的点
	static 	void FindCircleNearPoint1(POINT3D p, int &num, PPOLYGON poly);//求最近距离的点

	static double GetPerimeter(POLYGON poly);//计算环的周长
	static bool lineSegInter(POINT3D p1, POINT3D p2, POINT3D p3, POINT3D p4, POINT3D &p);
};

