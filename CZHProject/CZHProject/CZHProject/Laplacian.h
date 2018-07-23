#include "STLModell.h"
#include"KitTool.h"
#include <Eigen/Sparse>
typedef Eigen::SparseMatrix<double> SpMat;//typedef重定义
typedef Eigen::Triplet<double> T;

class CLaplacian
{
public:
	void change(vec_PVERT& m_vecLpvert,STLModell* &pCSTLModel,vector<POINT3D>& m_vecLp,vec_VECTOR3D& m_vecV);
	void back(vec_PVERT& m_vecLpvert, STLModell* &pCSTLModel, vector<POINT3D>& m_vecLp, vec_VECTOR3D& m_vecV);
	void selectP(STLModell* &pCSTLModel, vector<int>& m_vecSL);//选择控制顶点
protected:
private:
};