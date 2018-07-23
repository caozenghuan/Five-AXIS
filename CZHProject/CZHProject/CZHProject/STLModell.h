#pragma once
#include "datastructure/solidMesh.h"
#include "datastructure/polygon.h"
#include "DrawTool.h"
#include "datastructure/mesh.h"
#include "K-d tree/KDtree.h"
#include "sys/stat.h"
#include <map>
//写出用到//
#include <cstdlib>
#include <iostream>
#include <fstream>
#define COND_READ(cond, where, len) do { if ((cond) && !fread((void *)&(where), (len), 1, f)) return false; } while (0)
//预先定义边与边的比较运算法则
struct CompPEDGE
{
	bool operator ()(const PEDGE pE1, const PEDGE pE2)
	{
		if (pE1->pVertStart < pE2->pVertStart) return true;
		if (pE1->pVertStart > pE2->pVertStart) return false;
		if (pE1->pVertEnd < pE2->pVertEnd) return true;
		if (pE1->pVertEnd > pE2->pVertEnd) return false;
		return false;
	}
};


class STLModell :public CSolidMeshTri
{

public:
	static STLModell* GetInstance();
	STLModell();
	~STLModell();
	static  MATRIX3D CreateRotateMatrix(double da, VECTOR3D v);
	static  MATRIX3D CreateTranMatrix(VECTOR3D v);
	static void CreateEdgeTopByFctVert(STLModell* pMeshTri);

	bool LoadTextSTLFile(const char *ffile, vector<VECTOR3D> &All_VecDir, vector<vector<int>> &All_PairBlock, QProgressBar  *progressBar);
	bool LoadSTLFile(const char *ffile, QProgressBar  *progressBar);
	bool IsTextFile(const char *strFileName);
	void Zooommesh(STLModell* pMeshTri, STLModell* pMeshTemp, float _xzoom, float _yzoom, float _zzoom);

	bool write_stl(STLModell* pMeshTri, FILE *f);//写出为二进制格式文件
	void Write_STL(STLModell* pMeshTri, const QString _filepath);//写出为文本格式文件

	void Write_STLSEG(vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, const QString _filepath);//写出分割结果为文本格式文件

	void Write_BlockFillResult(vector<vec_PPOLYPOLYGON> BlockFill, vector<vec_PPOLYPOLYGON> Blocklunkuo, vector<VECTOR3D> All_VecDir, const QString _filepath);//写出分块填充结果
	void Write_FillResult(vec_PPOLYPOLYGON ModelFill,vec_PPOLYPOLYGON Modellunkuo, VECTOR3D Dir, const QString _filepath);//写出模型单一方向填充结果

	bool we_are_little_endian();
	void swap_int(int &x);
	void swap_float(float &x);
	void swap_unsigned(volatile unsigned &x);
	virtual bool	IsEmpty();
	virtual	void	UpdateBox();
	virtual void     Draw(drawTool *pDC);
	void    GetBox();
	void  GetMainBlockBox(vector<PFACETTRI>  OneBlock, BOX3D &ONE);//获取主子块包围盒大小
public:
	BOX3D    box;
	drawTool*pdc;
	KDtree *m_Tmesh_KD;
	BOX3D ONE;
};



