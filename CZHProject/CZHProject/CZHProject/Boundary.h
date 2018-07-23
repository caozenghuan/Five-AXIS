#include "STLModell.h"
#include "eigen/Eigen"
#include <Eigen/Eigenvalues>
#include <Eigen/Sparse>

#include "KitTool.h"
//#include "octree.h"
#include <queue>
typedef Eigen::SparseMatrix<double> SpMat;
typedef Eigen::Triplet<double> T;
using namespace Eigen;
using Eigen::MatrixXd;
class Cboundary
{
public:
	vec_PVERT vecPcut;
	//Octree* pOctree;
	KitTool Kit;

public:
	//////////////////////////////////////////////////////显式抽取算法
	void featurPOINT(vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert,double &NVlimit);//找到特征点域
	void featurPOINT_new(vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert);
	void Skeletonizing(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert
		,vec_PEDGE& m_vecPEdge);//HU的方法获取骨架线
	void  Skeletonizing_new(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge,vec_PEDGE& m_vecPEdge);//参数化方法找骨架线
	void  Skeletonizing_new_1(vector<vector<PVERT>>& RBLOOP,vec_PVERT& m_vecPVert);//空心域收缩法
	void Skeletonizing_close(vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge,double &Ang);//形成封闭的一圈先
	void Skeletonizing_boundary(vec_PEDGE patchT,queue<PHEDGE>& patchB);//找到一片区域的边界边
	void Skeletonizing_link(vector<PVERT>& BLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge,double &Ang);//通过骨架线闭合
	void Skeletonizing_link_new(vector<PVERT>& BLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert);//通过骨架线闭合
	void Skeletonizing_smoothing(vector<MyBlock> &Rbox, vector<vector<PVERT>>path, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert);//去锯齿光顺
	void Skeletonizing_find_boundary(vec_PVERT ALOOP,queue<PHEDGE>& patchB,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert);//找到一片区域的边界边
	BOOL Skeletonizing_delet(PHEDGE BIAN1,PHEDGE BIAN2);//判断Skeletonizing边界是否该移除
	void Skeletonizing_partlize(vector<MyBlock> &Rbox, vector<vector<PVERT>>&path, vec_PFACETTRI& m_vecPFacetTri, vec_PVERT& m_vecPVert);//种子生长法来分区域
	void Skeletonizing_combine(vector<vector<PFACETTRI>>& Rbox,vec_PFACETTRI& m_vecPFacetTri);
	//BOOL IntersectLinePlane(POINT3D LinePt,VECTOR3D LineNor, PFACETTRI pFacTri, POINT3D& InterP);//射线求交点
	void link_cone(vector<PVERT>& BLOOP,double angA,double angB,vec_PFACETTRI& m_vecPFacetTri,vec_PVERT& m_vecPVert,vec_PHEDGE& m_vecPHEdge);//可视锥连接起来
	void sort_smooth(vec_PVERT& templine);
	/////////////////利用谱聚类找骨架线
	void Laboundary(vector<vector<PFACETTRI>>& Rbox,vector<vector<PVERT>>& RBLOOP,vec_PFACETTRI& m_vecPFacetTri,vec_PHEDGE& m_vecPHEdge,vec_PVERT& m_vecPVert);
	void Laboundary_L(vector<vector<PFACETTRI>>& Rbox,vec_PFACETTRI& facelist);
	void get_dis(PHEDGE A,double& theDis,double& theAng);//得到面距离和角距离
	void get_avg(double& avgDis,double& avgAng,vec_PFACETTRI& vec_face);//得到平均值
	void LaMeans(vector<vector<PFACETTRI>>& Rbox,vector<vector<double>>& theVector,vec_PFACETTRI& facelist);//K-MEANS聚类
	double LaMeans_dis(vector<double>& A,vector<double>& B);//两个K维点之间的距离
	/////////////////////////////////////////////参数化snak光顺
	void snke_parameter(vec_PVERT& vec_pot,vec_PFACETTRI& facelist);//参数化
	 /////////////////////////////////////
	 //////////////////////自己的方法找骨架线
	void snke_contract(vec_PVERT& Bloop,vec_PVERT& m_vecPVert);
	void snake_move(PVERT& Apot,PVERT& Ppot,PVERT& Npot);
	double snake_inside(PVERT& Apot,PVERT& Ppot,PVERT& Npot);
	double snake_exter(PVERT& Apot);
	BOOL snake_link(vector<PVERT>& Bloop,PVERT& ST,PVERT& EN,vector<PVERT>& m_vecPVert);
	void snake_smooth(vector<PVERT>& Bloop);//去锯齿光顺
	void snake_smooth_close(vector<PVERT>& Bloop);//去锯齿光顺
	void snake_delete(vector<vector<PVERT>>& deLoop);//
	double snak_energe(vector<PVERT>& Bloop);//计算一条线总能量
	void snaking(vector<PVERT>& m_Pst,vector<PVERT>& m_vecPVert);

public:
	//////////////绘图函数//////////////////////
	void showVetex(drawTool *pDC, vec_PVERT	m_vecPVert);//顺序连接并显示边界边
protected:
private:
};