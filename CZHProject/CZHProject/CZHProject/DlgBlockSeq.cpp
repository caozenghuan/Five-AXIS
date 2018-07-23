#include "DlgBlockSeq.h"


DlgBlockSeq::DlgBlockSeq(QWidget *parent) :
ui(new Ui::BlockSeqshow)
{
	ui->setupUi(this);
	index = 0;
}


DlgBlockSeq::~DlgBlockSeq()
{
}
void DlgBlockSeq::LastBlock(int Index, vector<vector<PFACETTRI>>  &SimALLBlock)
{

	if (Index > 1)
	{
		int nub = SimALLBlock.size() - 1;
		ClearBlockPPOLY(SimALLBlock[nub]);
		BlockFillTemp.clear();
		for (int i = 0; i < SimALLBlock.size() - 1; i++)
		{
			SimALLBlockTemp.push_back(SimALLBlock[i]);
		}
		SimALLBlock.clear();
		SimALLBlock = SimALLBlockTemp;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
	}
}
void DlgBlockSeq::NextBlock(vector<vector<PFACETTRI>>  ALLBlock, vector<VECTOR3D> All_Dir, POINT3D Plow, int &Index, vector<vector<PFACETTRI>>  &SimALLBlock)
{
	for (int i = 0; i < SimALLBlock.size(); i++)
	{
		ClearBlockPPOLY(SimALLBlock[i]);
	}
	SimALLBlock.clear();

	TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;
	TM.A[0][1] = TM.A[0][2] = 0;
	TM.A[1][0] = TM.A[1][2] = 0;
	TM.A[2][0] = TM.A[2][1] = 0;
	for (int i = 1; i<Index + 1; i++)
	{
		pseq.CalRotateMaix(All_Dir[i], TM);
	}

	if (Index >All_Dir.size() - 1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
		/*vec_PPOLYPOLYGON OneBlock;
		BackFilldate(PathFill[0], OneBlock);
		BlockFillTemp.push_back(OneBlock);
		Index = 0;*/
	}
	else
	{
		if (Index == 0)
		{
			TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;//初始化变换矩阵
			vector<PFACETTRI> OneBlock;
			BackBlcokdate(ALLBlock[0], OneBlock);
			SimALLBlockTemp.push_back(OneBlock);
		}
		else
		{
			for (int i = 0; i < Index + 1; i++)
			{
				vector<PFACETTRI> OneBlock;
				BackBlcokdate(ALLBlock[i], OneBlock);
				SimALLBlockTemp.push_back(OneBlock);
				//TM0 = TM;//矩阵备份
				GettheBlock(SimALLBlockTemp[i], Plow, TM);
			}

		}
	}
	for (int i = 0; i < SimALLBlockTemp.size(); i++)
	{
		SimALLBlock.push_back(SimALLBlockTemp[i]);
	}
	//BlockFill = BlockFillTemp;
}
bool DlgBlockSeq::GettheBlock(vector<PFACETTRI> &OnelRotateBlock, POINT3D Plow, MATRIX3D TM)
{
	//平移
	VECTOR3D Tran_Dir;
	//POINT3D pTheLow = POINT3D(141.9, 105.9, z0);
	Tran_Dir = POINT3D(0, 0, 0) - Plow;
	MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);

	//平移回原来位置
	Tran_Dir = Plow - POINT3D(0, 0, 0);
	MATRIX3D TM1 = TM1.CreateTransfMatrix(Tran_Dir);

	//平移填充轮廓///
	for (int j = 0; j < OnelRotateBlock.size(); j++)
	{
		PFACETTRI Ptri = OnelRotateBlock[j];
		for (int k = 0; k < 3; k++)
		{
			PVERT p0 = Ptri->m_PVerts[j];
			POINT3D Temp;
			Temp.x = p0->x;
			Temp.y = p0->y;
			Temp.z = p0->z;
			Temp = Temp*TM0;//平移轮廓
			Temp = Temp*TM;//旋转轮廓
			Temp = Temp*TM1;//平移回去轮廓
			p0->x = Temp.x;
			p0->y = Temp.y;
			p0->z = Temp.z;
			Ptri->m_PVerts[j]=p0;
		}
		
		Ptri->m_PFacetNorm = KitTool::CalcuPfacNormal(Ptri->m_PVerts[0], Ptri->m_PVerts[1], Ptri->m_PVerts[2]);
	}
	return true;
}
void DlgBlockSeq::LastBlockPath(int Index, vector<vec_PPOLYPOLYGON> &BlockFill)
{
	if (Index > 0)
	{
		int nub = BlockFill.size() - 1;
		ClearBlockPathPPOLY(BlockFill[nub]);
		BlockFillTemp.clear();
		for (int i = 0; i < BlockFill.size()-1;i++)
		{
			BlockFillTemp.push_back(BlockFill[i]);
		}
		BlockFill.clear();
		BlockFill = BlockFillTemp;
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
	}
}
void DlgBlockSeq::NextBlockPath(vector<vec_PPOLYPOLYGON> PathFill, vector<VECTOR3D> All_Dir,  POINT3D Plow, int &Index, vector<vec_PPOLYPOLYGON> &BlockFill)
{
	for (int i = 0; i < BlockFill.size(); i++)
	{
		ClearBlockPathPPOLY(BlockFill[i]);
	}
	BlockFill.clear();

	TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;
	TM.A[0][1] = TM.A[0][2] = 0;
	TM.A[1][0] = TM.A[1][2] = 0;
	TM.A[2][0] = TM.A[2][1] = 0;
	for (int i = 1; i<Index + 1; i++)
	{
		pseq.CalRotateMaix(All_Dir[i], TM);
	}
	if (Index >All_Dir.size()-1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
		/*vec_PPOLYPOLYGON OneBlock;
		BackFilldate(PathFill[0], OneBlock);
		BlockFillTemp.push_back(OneBlock);
		Index = 0;*/
	}
	else
	{
		if (Index == 0)
		{
			TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;//初始化变换矩阵
			vec_PPOLYPOLYGON OneBlock;
			BackBlcokFilldate(PathFill[0], OneBlock);
			BlockFillTemp.push_back(OneBlock);
		}
		else
		{
			for (int i = 0; i < Index+1;i++)
			{
				vec_PPOLYPOLYGON OneBlock;
				BackBlcokFilldate(PathFill[i], OneBlock);
				BlockFillTemp.push_back(OneBlock);
				//TM0 = TM;//矩阵备份
				pseq.GettheBlockpath(BlockFillTemp[i],Plow, TM);
			}
			
		}
	}
	for (int i = 0; i < BlockFillTemp.size();i++)
	{
		BlockFill.push_back(BlockFillTemp[i]);
	}
	//BlockFill = BlockFillTemp;
}

void DlgBlockSeq::Getallblcokpath(vector<vec_PPOLYPOLYGON> ALLblockFill, vector<VECTOR3D> All_Dir, vector<vec_PPOLYPOLYGON> &PathFill)
{
	TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;
	TM.A[0][1] = TM.A[0][2] = 0;
	TM.A[1][0] = TM.A[1][2] = 0;
	TM.A[2][0] = TM.A[2][1] = 0;

	PathFill.push_back(ALLblockFill[0]);
	for (int i = 1; i < ALLblockFill.size(); i++)
	{
		//平移
		VECTOR3D Tran_Dir;
		POINT3D pTheLow = POINT3D(141.9, 105.9, ALLblockFill[i][0]->m_vecPPolygons[0]->m_vecPnts[0].z);
		Tran_Dir = POINT3D(0, 0, 0) - pTheLow;
		MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);

		//平移回原来位置
		Tran_Dir = pTheLow - POINT3D(0, 0, 0);
		MATRIX3D TM1 = TM1.CreateTransfMatrix(Tran_Dir);

		pseq.CalRotateMaix(All_Dir[i], TM);
		vec_PPOLYPOLYGON Oneblcok;
		//平移填充轮廓///
		for (int j = 0; j < ALLblockFill[i].size(); j++)
		{
			PPOLYPOLYGON OneLayer = new POLYPOLYGON;
			
			for (int k = 0; k < ALLblockFill[i][j]->m_vecPPolygons.size(); k++)
			{
				PPOLYGON Intu = new POLYGON;
				vec_POINT3D OneCircleTemp;
				OneCircleTemp = ALLblockFill[i][j]->m_vecPPolygons[k]->m_vecPnts;
				for (int m = 0; m < OneCircleTemp.size(); m++)
				{
					POINT3D Temp;
					Temp.x = OneCircleTemp[m].x;
					Temp.y = OneCircleTemp[m].y;
					Temp.z = OneCircleTemp[m].z;
					Temp = Temp*TM0;//平移轮廓
					Temp = Temp*TM;//旋转轮廓
					Temp = Temp*TM1;//平移回去轮廓
					Intu->m_vecPnts.push_back(Temp);
				}
				OneLayer->m_vecPPolygons.push_back(Intu);
			}
			Oneblcok.push_back(OneLayer);
		}
		PathFill.push_back(Oneblcok);
	}
}

//void DlgBlockSeq::Getallblcokpath()
//{
//	//清空填充轨迹模型数据
//	for (int i = 0; i < BlockFill.size(); i++)
//	{
//		ClearPPOLY(BlockFill[i]);
//		BlockFill.clear();
//	}
//	for (int i = 0; i < Sonopgl->BlockFill.size(); i++)
//	{
//		vec_PPOLYPOLYGON OneBlock;
//		BackFilldate(Sonopgl->BlockFill[i], OneBlock);
//		BlockFill.push_back(OneBlock);
//	}
//	TM.A[0][0] = TM.A[1][1] = TM.A[2][2] = 1;
//	TM.A[0][1] = TM.A[0][2] = 0;
//	TM.A[1][0] = TM.A[1][2] = 0;
//	TM.A[2][0] = TM.A[2][1] = 0;
//
//	for (int i = 0; i < BlockFill.size(); i++)
//	{
//		//平移
//		VECTOR3D Tran_Dir;
//		POINT3D pTheLow = POINT3D(141.9, 105.9, BlockFill[i][0]->m_vecPPolygons[0]->m_vecPnts[0].z);
//		Tran_Dir = POINT3D(0, 0, 0) - pTheLow;
//		MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);
//
//		//平移回原来位置
//		Tran_Dir = pTheLow - POINT3D(0, 0, 0);
//		MATRIX3D TM1 = TM1.CreateTransfMatrix(Tran_Dir);
//
//		pseq.CalRotateMaix(All_Dir[i], TM);
//		//平移填充轮廓///
//		for (int j = 0; j < BlockFill[i].size(); j++)
//		{
//			PPOLYPOLYGON OneLayer = BlockFill[i][j];
//			for (int k = 0; k < OneLayer->m_vecPPolygons.size(); k++)
//			{
//				vec_POINT3D OneCircle = OneLayer->m_vecPPolygons[k]->m_vecPnts;
//				for (int m = 0; m < OneCircle.size(); m++)
//				{
//					POINT3D Temp;
//					Temp.x = OneCircle[m].x;
//					Temp.y = OneCircle[m].y;
//					Temp.z = OneCircle[m].z;
//					Temp = Temp*TM0;//平移轮廓
//					Temp = Temp*TM;//旋转轮廓
//					Temp = Temp*TM1;//平移回去轮廓
//					OneCircle[m].x = Temp.x;
//					OneCircle[m].y = Temp.y;
//					OneCircle[m].z = Temp.z;
//				}
//				OneLayer->m_vecPPolygons[k]->m_vecPnts = OneCircle;
//			}
//			BlockFill[i][j] = OneLayer;
//		}
//	}
//	Sonopgl->ToolpathFill = BlockFill;
//}
void DlgBlockSeq::BackBlcokdate(vector<PFACETTRI> TempBlock, vector<PFACETTRI> &OneBlock)
{
	vector<PVERT> vec_pver;

	map<VERT, int>map_Vert; 
	map<VERT, int>::iterator iter;
	for (int i = 0; i < TempBlock.size(); i++)
	{
		PFACETTRI Ptri=TempBlock[i];
		PFACETTRI PtriE = new FACETTRI;
		PVERT p0;
		for (int j = 0; j < 3; j++)
		{
			p0 = new VERT;
			p0->x = Ptri->m_PVerts[j]->x; p0->y = Ptri->m_PVerts[j]->y; p0->z = Ptri->m_PVerts[j]->z;
			iter = map_Vert.find(*p0);
			if (iter == map_Vert.end())//不存在
			{
				map_Vert.insert(pair<VERT, int>(*p0, vec_pver.size()));
				vec_pver.push_back(p0);
			}
			PtriE->m_PVerts[j] = p0;
		}
		PtriE->m_PFacetNorm = KitTool::CalcuPfacNormal(PtriE->m_PVerts[0], PtriE->m_PVerts[1], PtriE->m_PVerts[2]);
		OneBlock.push_back(PtriE);
	}
}
void DlgBlockSeq::ClearBlockPPOLY(vector<PFACETTRI> &OneBlock)
{
	for (int i = 0; i < OneBlock.size(); i++)
	{
		PFACETTRI Ptri = OneBlock[i];
		for (int j = 0; j < 3; j++)
		{
			if (Ptri->m_PVerts[j]!=NULL)
			{
				delete Ptri->m_PVerts[j];
				Ptri->m_PVerts[j] = NULL;
			}
		}
		delete OneBlock[i]->m_PFacetNorm;
		OneBlock[i]->m_PFacetNorm = NULL;
		delete OneBlock[i];
		OneBlock[i] = NULL;
	}
	OneBlock.clear();
}

void DlgBlockSeq::BackBlcokFilldate(vec_PPOLYPOLYGON TempFill, vec_PPOLYPOLYGON &BlockFill)
{
	for (int i = 0; i < TempFill.size();i++)
	{
		PPOLYPOLYGON OneLayer = new POLYPOLYGON;
		for (int j = 0; j < TempFill[i]->m_vecPPolygons.size(); j++)
		{
			PPOLYGON Ityun = new POLYGON;
			vec_POINT3D  OneCirTem;
			OneCirTem = TempFill[i]->m_vecPPolygons[j]->m_vecPnts;
			for (int k = 0; k < OneCirTem.size(); k++)
			{
				POINT3D Temp;
				Temp.x = OneCirTem[k].x;
				Temp.y = OneCirTem[k].y;
				Temp.z = OneCirTem[k].z;
				Ityun->m_vecPnts.push_back(Temp);
			}
			
			OneLayer->m_vecPPolygons.push_back(Ityun);
		}
		BlockFill.push_back(OneLayer);
	}
}
void DlgBlockSeq::ClearBlockPathPPOLY(vec_PPOLYPOLYGON &oneBlock)
{
	for (int i = 0; i < oneBlock.size(); i++)
	{
		for (int j = 0; j < oneBlock[i]->m_vecPPolygons.size(); j++)
		{
			oneBlock[i]->m_vecPPolygons[j]->m_vecPnts.clear();
			delete oneBlock[i]->m_vecPPolygons[j];
			oneBlock[i]->m_vecPPolygons[j] = NULL;
		}
		oneBlock[i]->m_vecPPolygons.clear();
		delete oneBlock[i];
		oneBlock[i] = NULL;
	}
	oneBlock.clear();
}
void DlgBlockSeq::FindTheLow(vec_PPOLYPOLYGON MainBlock, POINT3D &Plow)
{
	int SumPoint = 0;
	int Num = MainBlock[0]->m_vecPPolygons.size();
	for (int i = 0; i < Num; i++)
	{
		int Nub = MainBlock[0]->m_vecPPolygons[i]->m_vecPnts.size();
		for (int j = 0; j < Nub; j++)
		{
			Plow.x += MainBlock[0]->m_vecPPolygons[i]->m_vecPnts[j].x;
			Plow.y += MainBlock[0]->m_vecPPolygons[i]->m_vecPnts[j].y;
			SumPoint++;
		}
	}

	Plow.x = Plow.x / SumPoint;
	Plow.y = Plow.y / SumPoint;
	Plow.z = MainBlock[0]->m_vecPPolygons[0]->m_vecPnts[0].z;

}
void DlgBlockSeq::FindTheLow(vector<PFACETTRI> OneBlock, POINT3D &Plow)
{
	BOX3D ONE;
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = OneBlock[1]->m_PVerts[0]->x;
	y0 = y1 = OneBlock[1]->m_PVerts[0]->y;
	z0 = z1 = OneBlock[1]->m_PVerts[0]->z;

	FACETTRI* ptri;
	for (int i = 0; i < OneBlock.size(); i++)
	{
		ptri = OneBlock[i];
		for (int n = 0; n < 3; n++)
		{
			if (ptri->m_PVerts[n]->x < x0)
				x0 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->x > x1)
				x1 = ptri->m_PVerts[n]->x;
			if (ptri->m_PVerts[n]->y < y0)
				y0 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->y > y1)
				y1 = ptri->m_PVerts[n]->y;
			if (ptri->m_PVerts[n]->z < z0)
				z0 = ptri->m_PVerts[n]->z;
			if (ptri->m_PVerts[n]->z > z1)
				z1 = ptri->m_PVerts[n]->z;
		}
	}
	ONE.x0 = x0;
	ONE.y0 = y0;
	ONE.z0 = z0;
	ONE.x1 = x1;
	ONE.y1 = y1;
	ONE.z1 = z1;
	Plow.x = (x0 + x1) / 2; Plow.y = (y0 + y1) / 2; Plow.z = z0;
}
void DlgBlockSeq::UpdateBoarddate(vector<VECTOR3D> All_Dir, vector<vec_POINT3D> PlatFace, vector<vec_POINT3D> &PlatFaceTemp, vector<vec_POINT3D> p_Row, vector<vec_POINT3D> &p_RowTemp, POINT3D Plow, int &Index)
{
	MATRIX3D TMR;
	TMR.A[0][0] = TMR.A[1][1] = TMR.A[2][2] = 1;
	for (int i = 1; i < Index + 1; i++)
	{
		pseq.CalRotateMaix(All_Dir[i], TMR);
	}
	if (Index >All_Dir.size() - 1)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
	}
	else
	{
		//平移
		VECTOR3D Tran_Dir;
		//POINT3D pTheLow = POINT3D(141.9, 105.9, z0);
		Tran_Dir = POINT3D(0, 0, 0) - Plow;
		MATRIX3D TM0 = TM0.CreateTransfMatrix(Tran_Dir);

		//平移回原来位置
		Tran_Dir = Plow - POINT3D(0, 0, 0);
		MATRIX3D TM1 = TM1.CreateTransfMatrix(Tran_Dir);
		for (int i = 0; i < PlatFace.size(); i++)
		{
			vec_POINT3D OneLayer;
			for (int j = 0; j < PlatFace[i].size(); j++)
			{
				POINT3D Temp;
				Temp.x = PlatFace[i][j].x;
				Temp.y = PlatFace[i][j].y;
				Temp.z = PlatFace[i][j].z;
				Temp = Temp*TM0;//平移轮廓
				Temp = Temp*TMR;//旋转轮廓
				Temp = Temp*TM1;//平移回去轮廓

				OneLayer.push_back(Temp);
			}
			PlatFaceTemp.push_back(OneLayer);
		}
		for (int i = 0; i < p_Row.size(); i++)
		{
			vec_POINT3D OneLayer;
			for (int j = 0; j < p_Row[i].size(); j++)
			{
				POINT3D Temp;
				Temp.x = p_Row[i][j].x;
				Temp.y = p_Row[i][j].y;
				Temp.z = p_Row[i][j].z;
				Temp = Temp*TM0;//平移轮廓
				Temp = Temp*TMR;//旋转轮廓
				Temp = Temp*TM1;//平移回去轮廓

				OneLayer.push_back(Temp);
			}
			p_RowTemp.push_back(OneLayer);
		}
	}
}
