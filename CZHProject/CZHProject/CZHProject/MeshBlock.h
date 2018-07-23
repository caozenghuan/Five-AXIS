#pragma once
#include "DijkstraGeodesic.h"
#include"Boundary.h"
#include "Laplacian.h"
#include "CSimplify.h"
#include "DlgOpti.h"
#include <math.h>
class MeshBlock
{
public:
	MeshBlock();
	virtual ~MeshBlock();
public:
	KitTool PkiTool;
	Cboundary *pcboundary; CLaplacian *pLaplacian; 
	CSimplify *pSimply; CDijkstraGeodesic pcedie;

public:

	//拉普拉斯变形前的参数
	vec_POINT3D m_vecLp;
	vec_VECTOR3D m_vecV;
	bool m_Laplacian;
	vector<EDGE> vec_edge;
	

public:
	void SharpEdgeDetection(STLModell *pMeshTri, vec_EDGE &vec_edge);//尖锐边识别
	void featurPOINT(vec_PFACETTRI& m_vecPFacetTri, vec_PHEDGE& m_vecPHEdge, vec_PVERT& m_vecPVert, double &NVlimit);//找到特征点域

	bool CutLineTraceProjection(vector<POINT3D> pt, vector<PVERT>&path, vector<Insepoint> &pathTemp, STLModell* m_Tmesh);//追踪投影法求分割线
	void CutTraceProjection(vector<Insepoint> pathTemp, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert, STLModell* m_Tmesh);
	bool CalVetexNormal(PVERT p0, PVERT p1, PVERT p2, PVECTOR3D FaceNormal);//通过求法矢改变顶点顺序
	void PickPoint(vector<POINT3D> pt, vector<PVERT>&path, STLModell* m_Tmesh);//测地距离法拾取分割线
	bool CalDispath(PVERT& PVer,PVERT Rpoint, double &Sdist,vector<PVERT> path);//给顶点到环的最小距离
	//优化分割线
	bool CutLineDir(vector<vec_PVERT> &FeaPointAll, vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace, STLModell* m_Tmesh);//优化求分割线（给定一个点和最优成型方向）
	bool CutLineDir1(vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace);//优化求分割线（给定环所经过的两个点）
	bool CutLineDir2(vector<vec_PVERT> &CFpoint, vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, vector<vector<PVERT>>&path, vector<vector<Insepoint>> &pathTrace, vec_PVERT m_vecPVert);//优化求分割线（给定环所经过的三个点）

	bool IfNeedLoop(vector<Insepoint> path);//查看是否需要进行细分
	void Updatecutline(vector<vector<PVERT>> &path, vector<vector<Insepoint>>  pathTemp, vec_PVERT m_vecPVert);//更新未被处理的边界线顶点

	//绘图函数/////////////////////////
	void showfeatureEdge(drawTool *pDC);//顺序连接并显示特征边界
	void showEdge(drawTool *pDC, vec_EDGE vec_Edge);//特征边显示
	void Render(drawTool* pDC, vector<PVERT> m_Path_Point);//绘制分割线

	void DrawPoint(drawTool* pDC, vector<vec_PVERT> CFpoint);//优化分割线最优凹点显示
	void DrawFeapoint(drawTool* pDC, vector<vec_PVERT> FeaPointAll);//分割线凹特征点显示
	void Drawblock(drawTool* pDC, vector<MyBlock> &Rbox);//绘制块分割结果
	void Drawblock(drawTool* pDC, COLORREF newcolor, vec_PFACETTRI& m_vecPFacetTri);//绘制块分割结果

	void DrawSwichBlock(drawTool *pDC, int nBlock, vector<MyBlock> &Rbox);//绘制分割子块
	void Getreadblock(vec_PFACETTRI& m_vecPFacetTri, vector<vector<int>> All_PairBlock, vector<MyBlock> &Rbox);//识别读入模型分割块
	void DrawPlat(drawTool* pDC, vector<vec_POINT3D> PlatFace, vector<vec_POINT3D> p_Row);//绘制平台
};
