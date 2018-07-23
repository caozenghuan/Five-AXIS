#pragma once
#include "DrawTool.h"
#include "STLModell.h"
#include "KitTool.h"
#include "DrawTool.h"
#include "DlgInterveResult.h"
#include <time.h>
#define  AuxZero  0.000001
//////////自定义个体数据结构////////
typedef struct SUnit
{
	vector<int> Sequence;
	double f1Sum;/////第一目标函数,最小摆角
	double f2Sum;/////第二目标函数,最小移动距离
	int  n;////被支配个数
	vector<SUnit*>  S;//支配集合
	int level;//////支配等级
	float distance; /////拥挤距离
	double v;/////粒子群速度
	SUnit()
	{
		f1Sum = 0;
		f2Sum = 0;
		n = 0;
		level = 0;
		distance = 0;
		v = 0;
	};

	bool operator <(const SUnit& v1)const
	{
		if (f1Sum < v1.f1Sum) return true;
		return false;
	};

}SUNIT, *pSUNIT;
//自定义干涉块数据
typedef struct IntervBlock
{
	vector<vec_POINT3D> ModelDate;
	vector<vec_POINT3D> BoxDate;
	bool status;
	IntervBlock()
	{
		status = false;
	};

}SInterB, *pSInterB;


struct SCompUNIT
{
	bool operator ()(const SUNIT p1, const SUNIT p2)
	{
		if (p1.f1Sum > p2.f1Sum) return true;
		if (p1.f1Sum < p2.f1Sum) return true;
		if (p1.f2Sum > p2.f2Sum) return true;
		if (p1.f2Sum < p2.f2Sum) return true;
		return false;
	}
};
class COptisequence
{
public:
	KitTool Pkit;
	COptisequence();
	 ~COptisequence();
	 vector<SUNIT>  MOPSO(vector<MyBlock> Rbox, vector<VECTOR3D> &All_VecDir,DlgInterveResult &InterveResult, QProgressBar *MyProgressBar);
	 vector<vector<int>> RandomDir(int SZ, vector<MyBlock> Rbox, vector<VECTOR3D> &All_VecDir, DlgInterveResult &InterveResult, QProgressBar *MyProgressBar);//初始种群
	 vector<SUNIT> non_inferior(vector<vector<int>> &MyDirS, vector<VECTOR3D> &All_VecDir, vector<MyBlock> Rbox);//非支配等级排序
	 //void IfAcuteAngle(VECTOR3D &Dir, VECTOR3D Diz);//查看最优方向是否与Z轴方向为锐角，否则就反向。
	 double TheLestAngle(vector<int> Sqence, vector<VECTOR3D> All_VecDir);//以摆动角度最小为目标的优化函数
	 double TheLestDistance(vector<int> Sqence, vector<VECTOR3D> All_VecDir, vector<MyBlock> Rbox);//以喷头移动距离最小为目标的优化函数
	 int RC1(vector<int> Sqence, vector<MyBlock> Rbox);//以邻接原则为目标的优化函数
	 int RC2(vector<int> Sqence, vector<MyBlock> Rbox);//以上下原则为目标的优化函数
	 void UpdateBlockBox(vector<PFACETTRI> pMeshTri, BOX3D &ONE);
	 void SaveOptiResult(vector<MyBlock> &Rbox, vector<VECTOR3D> &All_VecDir, vector<int> Sequence);//保存优化结果
	 
	 //干涉检查函数
	 void UpdateBox(vector<MyBlock> Rbox);//更新块的包围盒信息
	 void Creatboarddate(vector<vec_POINT3D> &vec_face, POINT3D pcenter);//创建平台板面片数据
	 POINT3D CalCenterPoint(MyBlock CurrnetBlock,VECTOR3D DirV);//计算当前块的最底层中心点
	 void CreatHeadboxdate(vector<vec_POINT3D> &vec_face, POINT3D Pcenter,VECTOR3D DirV);//创建喷头方块包围盒数据
	 void JoinCurrentBlock(vector<SInterB>&AllModelInfor, MyBlock CurrnetBlock);//将当前块面片信息加入总面片集
	 void RotateHeadBox(vector<vec_POINT3D> &vec_face, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转打印头包围盒

	 void RotateBlock(MyBlock &OneBlock, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转分割块
	 void RotateDir(vector<VECTOR3D> &Vec_Dir, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转最优成型方向
	 bool JudetheInsert(vec_POINT3D Pfac1, vec_POINT3D Pfac2,POINT3D pCenter,VECTOR3D Dir);//判断两个面片有无交点
	 bool GetIntereResult(vector<SInterB> &AllModelInfor, MyBlock CurrnetBlock, vector<vec_POINT3D> vec_Head, POINT3D pCenter, VECTOR3D Dir,int &InteBlock);//获取干涉结果
	 
	 void DrawResult(vector<MyBlock> Rbox, vector<VECTOR3D> All_VecDir, drawTool *pDC, double len);

	 bool GettheBlockpath(vec_PPOLYPOLYGON &OnelRotateBlock, POINT3D Plow, MATRIX3D TM);
	 void CalRotateMaix(VECTOR3D &Dir_now, MATRIX3D &TM);
	 void TranRbox(vector<MyBlock> &Rbox,POINT3D plow);//平移块分割模型
	 void TranRbox(vector<vec_PPOLYPOLYGON> &BlockFill, POINT3D plow);//平移块填充结果
};

