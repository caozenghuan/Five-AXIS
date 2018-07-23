#pragma once
#include "DrawTool.h"
#include "STLModell.h"
#include "KitTool.h"
//////自定义数据结构用于保留最优方向
typedef struct i2Sum
{
	int i;
	double sum;
	bool operator <(const i2Sum& v1)const
	{
		if (sum<v1.sum) return true;
		return false;
	};
};


//////////自定义个体数据结构////////
typedef struct Unit
{
	VECTOR2D  Dir;////方向
	double f1Sum;/////第一目标函数
	double f2Sum;/////第二目标函数
	int  n ;////被支配个数
	vector<Unit*>  S;//支配集合
	int level;//////支配等级
	float distance; /////拥挤距离
	double v;/////粒子群速度
	Unit()
	{
		f1Sum=0;
		f2Sum=0;
		n=0;
		level =0;
	    distance =0;
		v=0;
	};

	bool operator <(const Unit& v1)const
	{
		if (f1Sum<v1.f1Sum) return true;
		return false;
	};

}UNIT,*pUNIT;

struct CompUNIT
{
	bool operator ()(const UNIT p1, const UNIT p2)
	{
		if (p1.f1Sum > p2.f1Sum) return true;
		if (p1.f1Sum < p2.f1Sum) return true;
		if (p1.f2Sum > p2.f2Sum) return true;
		if (p1.f2Sum < p2.f2Sum) return true;
		return false;
	}
};



class COptDir   
{
public:
	COptDir();
    ~COptDir();
public:
	vector<vector<int>> 	m_vecPFacetTri;
public:
	//////////////////////////显示函数////////////////
	void  VisualResult(vec_PVECTOR3D m_vecPFacetNorm, VECTOR3D  DirV);
	static void DrawResult(vec_PFACETTRI m_vecBlockFacet, vector<vector<int>> m_vecPFacetTri, drawTool *pDC);//画出各子块在最优成型方向上的支撑情况
	static void  DrawResult(STLModell* pSTLModel, vector<vector<int>> 	m_vecPFacetTri, drawTool *pDC);
	static void  DrawInfo(drawTool *pDC);//画标尺
	static void  DrawOptiDir(drawTool *pDC, VECTOR3D m_VecDir, BOX3D ONE,double len);//画出最优成型方向

	//////////////////////基本遗传算法(GA) 单目标/////////
	double SA(vec_PVECTOR3D m_vecPFacetNorm,VECTOR2D  DirV,vector<double>vec_Area);
	VECTOR2D GA(STLModell *pSTL);
	vec_VECTOR2D RandomDir(int SZ);
	vector<int> GA_selectDir(vec_VECTOR2D AllDir,vector<double> AllSum);
	vec_VECTOR2D GA_Crossover(VECTOR2D v1,VECTOR2D v2);
	VECTOR2D GA_Mutation();
	//double GetTriArea(PFACETTRI ptri);
	
	////////////////////带精英策略的非支配排序遗传算法(NSGA_II)//////
	vector<UNIT> NSGA_ii(STLModell *pSTL);
	double SE(vec_PVECTOR3D m_vecPFacetNorm,VECTOR2D  DirV,vector<double>vec_Area);
	vector<vector<UNIT>> non_dominatedSet(vec_VECTOR2D MyDirS,vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area);//////用于非支配排序
	void Distance(vector<UNIT> &vec_dominated);//////计算拥挤距离 目标函数增加必须改变
	vector<UNIT> TournamentSelection(vector<vector<UNIT>> vec_MyDominated,int SZ);////锦标赛选择
	vector<vector<UNIT>>  NSGA_ii_Sort(vector<vector<UNIT>> vec_MyDominated,int SZ);//////排序为下次循环
	///////////////////多目标粒子群算法(MOPSO)/////////////////////////////
	vector<UNIT> MOPSO(STLModell *pSTL, QProgressBar *MyProgressBar);
	vector<UNIT>  MOPSO(vec_PFACETTRI m_vecPFacetTri, QProgressBar *MyProgressBar);
	vec_VECTOR2D MOPSO_upDate(VECTOR2D   gBest,vec_VECTOR2D ALLDir,vec_VECTOR2D  vec_Velocity,
								double &v1max,double & v2max,vec_VECTOR2D &vec_pBest);
	VECTOR2D    MOPSO_Select_gbest(vec_VECTOR2D ALLDir,vector<UNIT> Bestnums,vector<UNIT>  &ElitePopulation,
								vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area);
	void     MOPSO_UpdatePbest(vec_PVECTOR3D m_vecPFacetNorm,vec_VECTOR2D AllDir,vector<double>vec_Area,vec_VECTOR2D &vec_pBest);
	vector<UNIT> non_inferior(vec_VECTOR2D MyDirS,vec_PVECTOR3D m_vecPFacetNorm,vector<double>vec_Area);
};



class CGlobal
{

public:
	CGlobal(void);
	~CGlobal(void);
public:
	static 	int  m_myCtrlSet;
};

