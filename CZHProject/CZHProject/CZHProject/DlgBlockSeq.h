#pragma once
#include "ui_DlgBlockshow.h"
#include "KitTool.h"
#include"Optisequence.h"
class DlgBlockSeq :public QDialog, public Ui::BlockSeqshow
{
	Q_OBJECT

public:
	DlgBlockSeq(QWidget *parent = 0);
	virtual ~DlgBlockSeq();

	Ui::BlockSeqshow *ui;

public:
	COptisequence pseq;

	vector<vector<PFACETTRI>>  SimALLBlockTemp;
	vector<vec_PPOLYPOLYGON> BlockFillTemp;

	int index;
	int SumNumb;
	MATRIX3D TM0;
	MATRIX3D TM;

public:
	void LastBlock(int Index, vector<vector<PFACETTRI>>  &SimALLBlock);
	void NextBlock(vector<vector<PFACETTRI>>  ALLBlock, vector<VECTOR3D> All_Dir, POINT3D Plow, int &Index, vector<vector<PFACETTRI>>  &SimALLBlock);

	void LastBlockPath(int Index, vector<vec_PPOLYPOLYGON> &BlockFill);
	void NextBlockPath(vector<vec_PPOLYPOLYGON> PathFill, vector<VECTOR3D> All_Dir, POINT3D Plow, int &Index, vector<vec_PPOLYPOLYGON> &BlockFill);


	void BackBlcokdate(vector<PFACETTRI> TempBlock, vector<PFACETTRI> &OneBlock);
	void ClearBlockPPOLY(vector<PFACETTRI> &OneBlock);

	void BackBlcokFilldate(vec_PPOLYPOLYGON TempFill, vec_PPOLYPOLYGON &BlockFill);
	void ClearBlockPathPPOLY(vec_PPOLYPOLYGON &oneBlock);//清空子块轨迹容器

	bool GettheBlock(vector<PFACETTRI> &OnelRotateBlock, POINT3D Plow, MATRIX3D TM);
	void Getallblcokpath(vector<vec_PPOLYPOLYGON> ALLblockFill, vector<VECTOR3D> All_Dir, vector<vec_PPOLYPOLYGON> &PathFill);

	void FindTheLow(vec_PPOLYPOLYGON MainBlock, POINT3D &Plow);//求主子块底面的中心点
	void FindTheLow(vector<PFACETTRI> OneBlock, POINT3D &Plow);//求主子块底面的中心点

	void UpdateBoarddate(vector<VECTOR3D> All_Dir, vector<vec_POINT3D> PlatFace, vector<vec_POINT3D> &PlatFaceTemp, vector<vec_POINT3D> p_Row, vector<vec_POINT3D> &p_RowTemp, POINT3D Plow, int &Index);//更新平台板数据
private slots :

	
};

