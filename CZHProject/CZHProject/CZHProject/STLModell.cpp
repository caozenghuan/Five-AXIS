
#include "STLModell.h"


//声明点的SET容器变量及累加器类型
map<VERT, PVERT> mapVert;
map<VERT, PVERT>::iterator iter;
pair<map<VERT, PVERT>::iterator, bool > mapV;//返回值指向一个累加器和描述是否存入点的bool值


static STLModell * ms_pStlmodell = NULL;
STLModell* STLModell::GetInstance()
{
	if (ms_pStlmodell == NULL)
		ms_pStlmodell = new STLModell;
	return ms_pStlmodell;
}
STLModell::STLModell()
{
	pdc = new drawTool;
	m_Tmesh_KD = NULL;
}


STLModell::~STLModell()
{
	if (pdc||m_Tmesh_KD)
	{
		delete pdc;
		delete m_Tmesh_KD;
		m_Tmesh_KD = NULL;
		pdc = NULL;
	}
	ms_pStlmodell = NULL;
}



////读二进制格式的文件

bool STLModell::LoadSTLFile(const char *ffile, QProgressBar  *progressBar)
{
	FILE *f = fopen(ffile, "rb");
	bool need_swap = !we_are_little_endian();
	char header[80];
	COND_READ(true, header, 80);
	int nfacets, VetxID = 0;
	COND_READ(true, nfacets, 4);
	if (need_swap)
		swap_int(nfacets);
	progressBar->setRange(0, nfacets - 1);
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;

	for (int i = 0; i < nfacets; i++)
	{
		progressBar->setValue(i);
		float fbuf[12];
		COND_READ(true, fbuf, 48);
		if (need_swap) {
			for (int j = 3; j < 12; j++)
			swap_float(fbuf[j]);
		}
		PFACETTRI ptri = new FACETTRI;
		PVERT point;
		for (int j = 0; j < 3; j++)
		{
			point = new VERT;
			point->VertexID = VetxID; 
			point->x = fbuf[3 * j + 3];
			point->y = fbuf[3 * j + 4];
			point->z = fbuf[3 * j + 5];
			iter = map_Vert.find(*point);
			if (iter != map_Vert.end())//已存在该点
			{
				ptri->m_PVerts[j] = m_vecPVert[iter->second];
				m_vecPVert[iter->second]->vec_OneFac.push_back(ptri);///构建一阶邻域
				delete point;
				point = NULL;
			}
			else
			{
				map_Vert.insert(pair<VERT, int>(*point, m_vecPVert.size()));
		
				m_vecPVert.push_back(point);
				point->vec_OneFac.push_back(ptri);////构建一阶邻域
				ptri->m_PVerts[j] = point;
				VetxID++;
			}
		}
		ptri->Fnumber = nfacets;
		ptri->num = m_vecPFacetTri.size();
		m_vecPFacetTri.push_back(ptri);
		////////求法矢////////
		VECTOR3D v1 = *ptri->m_PVerts[1] - *ptri->m_PVerts[0];
		VECTOR3D v2 = *ptri->m_PVerts[2] - *ptri->m_PVerts[1];
		PVECTOR3D v3 = new VECTOR3D;
		*v3 = v1*v2;
		v3->Normalize();
		ptri->m_PFacetNorm = v3;
		m_vecFacetNorm.push_back(v3);
		unsigned char att[2];
		COND_READ(true, att, 2);
	}
	fclose(f);//需要将文件关闭，一定要注意，否则会造成文件所占用内存泄露和在下次访问文件时出现问题
	f = NULL;//需要将文件指针指向空，这样做会防止出现游离指针，而对整个工程造成不必要的麻烦
	return true;
}


bool STLModell::LoadTextSTLFile(const char *ffile, vector<VECTOR3D> &All_VecDir, vector<vector<int>> &All_PairBlock,QProgressBar  *progressBar)
{
	FILE* file;
	int type = 0,VetxID = 0;
	FILE* fileTemp;
	if ((fileTemp = fopen(ffile, "r")) == NULL)
		return false;
	char str[80];
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	int  nfacets = 0;
	while (fscanf(fileTemp, "%s", str) == 1)
	{
		if (strncmp(str, "normal", 6) == 0)
		{
			nfacets++;
		}
	}
	progressBar->setRange(0, nfacets - 1);
	nfacets = 0;
	int mf = 0;
	fclose(fileTemp);
	file = fopen(ffile, "r");
	int nblock = -1, Pblock = -1;vector<int> PairBlock;
	All_VecDir.clear(); All_PairBlock.clear();
	while (fscanf(file, "%s", str) == 1)
	{
		VECTOR3D DirNormal;
		if (strncmp(str, "block", 5) == 0)
		{
			fscanf(file, "%d", &nblock);
		}
		if (strncmp(str, "DirOpti", 7) == 0)
		{
			fscanf(file, "%lf %lf %lf", &(DirNormal.dx), &(DirNormal.dy), &(DirNormal.dz));
		}
		if (strncmp(str, "PairBlock", 9) == 0)
		{
			fscanf(file, "%d", &Pblock);
			PairBlock.push_back(Pblock);
		}
		if (strncmp(str, "normal", 6) == 0)
		{
			if (PairBlock.size()!=0)
			{
				All_PairBlock.push_back(PairBlock);
				PairBlock.clear();
			}
			progressBar->setValue(nfacets);
			PFACETTRI ptri = new FACETTRI;
			PVECTOR3D normal;
			normal = new VECTOR3D;
			fscanf(file, "%lf %lf %lf", &(normal->dx), &(normal->dy), &(normal->dz));
			ptri->m_PFacetNorm = normal;
			fscanf(file, "%*s %*s");
			PVERT point = NULL;
			for (int i = 0; i < 3; i++)
			{
				point = new VERT;
				point->VertexID = VetxID;
				fscanf(file, "%*s %lf %lf %lf", &(point->x), &(point->y), &(point->z));
				iter = map_Vert.find(*point);
				if (iter != map_Vert.end())//已存在该点
				{
					int Num = iter->second;
					ptri->m_PVerts[i] = m_vecPVert[Num];
					m_vecPVert[Num]->vec_OneFac.push_back(ptri);////构建一阶邻域
					delete point;
					point = NULL;
				}
				else
				{
					map_Vert.insert(pair<VERT, int>(*point, m_vecPVert.size()));
					m_vecPVert.push_back(point);
					point->vec_OneFac.push_back(ptri);
					ptri->m_PVerts[i] = point;
					VetxID ++; 
				}
			}//3个点读完 
			//读入面片所属块信息
			if (nblock!=-1)
			ptri->Nblock = nblock;
			ptri->Fnumber = nfacets;
			m_vecPFacetTri.push_back(ptri);
			////////求法矢////////
			VECTOR3D v1 = *ptri->m_PVerts[1] - *ptri->m_PVerts[0];
			VECTOR3D v2 = *ptri->m_PVerts[2] - *ptri->m_PVerts[1];
			PVECTOR3D v3 = new VECTOR3D;
			*v3 = v1*v2;
			v3->Normalize();
			ptri->m_PFacetNorm = v3;
			m_vecFacetNorm.push_back(v3);
			nfacets++;
		}
		if (DirNormal.dx != 0 && DirNormal.dx != 0 && DirNormal.dx != 0)
		{
			All_VecDir.push_back(DirNormal);
		}

	}
	m_bModified = true;
	fclose(file);
	return true;
}
void STLModell::Write_FillResult(vec_PPOLYPOLYGON ModelFill, vec_PPOLYPOLYGON Modellunkuo, VECTOR3D Dir, const QString _filepath)
{
	QString file_name;
	string nlayer = "layer ";
	string nblock = "block ";
	string DirOpti = "DirOpti";
	string ncircle = "circle";
	string noutcircle = "outcircle";
	string vertex = "vertex ";
	string outvertex = "outvertex ";
	string outlunkuo = "lunkuo";
	string endloop = "endloop";
	string endfacet = "endfacet";
	string solid = "solid ";
	string endsolid = "endsolid ";
	string nr = "\r\n";

	QFileInfo fi(_filepath);
	file_name = fi.baseName();
	std::ofstream ofs(qPrintable(_filepath));
	ofs.precision(20);
	ofs << solid << file_name.toStdString() << nr;
	vector<int> OnePair;

	vec_PPOLYPOLYGON vec_oneFill = ModelFill;
	vec_PPOLYPOLYGON vec_oneblock = Modellunkuo;

	ofs << "  " << nblock << 0 << nr;//输出面片所属块
	ofs << "  " << DirOpti << " " << Dir.dx << " " << Dir.dy << " " << Dir.dz << nr;//保存最优方向

	for (unsigned j = 0; j < vec_oneFill.size(); j++)
	{
		const PPOLYPOLYGON onelayer = vec_oneFill[j];
		const PPOLYPOLYGON outonelayer = vec_oneblock[j];
		//保存填充信息
		ofs << "    " << nlayer << nr;
		for (unsigned k = 0; k < onelayer->m_vecPPolygons.size(); k++)
		{
			ofs << "    " << ncircle << nr;
			PPOLYGON Circle = onelayer->m_vecPPolygons[k];
			for (unsigned m = 0; m < Circle->m_vecPnts.size(); m++)
			{
				ofs << "  " << vertex << Circle->m_vecPnts[m].x << " " << Circle->m_vecPnts[m].y << " " << Circle->m_vecPnts[m].z << nr;
			}
		}
		//保存切片信息
		ofs << "    " << outlunkuo << nr;
		for (unsigned k = 0; k < outonelayer->m_vecPPolygons.size(); k++)
		{
			ofs << "    " << noutcircle << nr;
			PPOLYGON outCircle = outonelayer->m_vecPPolygons[k];
			for (unsigned m = 0; m < outCircle->m_vecPnts.size(); m++)
			{
				ofs << "  " << outvertex << outCircle->m_vecPnts[m].x << " " << outCircle->m_vecPnts[m].y << " " << outCircle->m_vecPnts[m].z << nr;
			}
		}

		ofs << "    " << endloop << nr;
		ofs << "  " << endfacet << nr;
	}
	ofs << endsolid << file_name.toStdString();
}
void STLModell::Write_BlockFillResult(vector<vec_PPOLYPOLYGON> BlockFill, vector<vec_PPOLYPOLYGON> Blocklunkuo, vector<VECTOR3D> All_VecDir, const QString _filepath)
{
	QString file_name;
	string nlayer = "layer ";
	string nblock = "block ";
	string DirOpti = "DirOpti";
	string ncircle = "circle";
	string noutcircle = "outcircle";
	string vertex = "vertex ";
	string outvertex = "outvertex ";
	string outlunkuo = "lunkuo";
	string endloop = "endloop";
	string endfacet = "endfacet";
	string solid = "solid ";
	string endsolid = "endsolid ";
	string nr = "\r\n";

	QFileInfo fi(_filepath);
	file_name = fi.baseName();
	std::ofstream ofs(qPrintable(_filepath));
	ofs.precision(20);
	ofs << solid << file_name.toStdString() << nr;
	vector<int> OnePair;
	if (All_VecDir.size() != 0)
	{
		for (int i = 0; i < BlockFill.size(); i++)
		{
			vec_PPOLYPOLYGON vec_oneFill = BlockFill[i];
			vec_PPOLYPOLYGON vec_oneblock = Blocklunkuo[i];

			ofs << "  " << nblock << i << nr;//输出面片所属块
			ofs << "  " << DirOpti << " " << All_VecDir[i].dx << " " << All_VecDir[i].dy << " " << All_VecDir[i].dz << nr;//保存最优方向

			for (unsigned j = 0; j < vec_oneFill.size(); j++)
			{
				const PPOLYPOLYGON onelayer = vec_oneFill[j];
				const PPOLYPOLYGON outonelayer = vec_oneblock[j];
				//保存填充信息
				ofs << "    " << nlayer << nr;
				for (unsigned k = 0; k < onelayer->m_vecPPolygons.size(); k++)
				{
					ofs << "    " << ncircle << nr;
					PPOLYGON Circle = onelayer->m_vecPPolygons[k];
					for (unsigned m = 0; m < Circle->m_vecPnts.size(); m++)
					{
						ofs << "  " << vertex << Circle->m_vecPnts[m].x << " " << Circle->m_vecPnts[m].y << " " << Circle->m_vecPnts[m].z << nr;
					}
				}
				//保存切片信息
				ofs << "    " << outlunkuo << nr;
				for (unsigned k = 0; k < outonelayer->m_vecPPolygons.size(); k++)
				{
					ofs << "    " << noutcircle << nr;
					PPOLYGON outCircle = outonelayer->m_vecPPolygons[k];
					for (unsigned m = 0; m < outCircle->m_vecPnts.size(); m++)
					{
						ofs << "  " << outvertex << outCircle->m_vecPnts[m].x << " " << outCircle->m_vecPnts[m].y << " " << outCircle->m_vecPnts[m].z << nr;
					}
				}

				ofs << "    " << endloop << nr;
				ofs << "  " << endfacet << nr;
			}
		}

	}
	else
	{
		return;
	}
	ofs << endsolid << file_name.toStdString();
}
void STLModell::Write_STLSEG(vector<MyBlock> &Rbox, vector<VECTOR3D> All_VecDir, const QString _filepath)
{
	QString file_name;
	string facetNormal = "facet normal ";
	string nblock = "block ";
	string DirOpti = "DirOpti";
	string PairBlock = "PairBlock";
	string outerloop = "outer loop";
	string vertex = "vertex ";
	string endloop = "endloop";
	string endfacet = "endfacet";
	string solid = "solid ";
	string endsolid = "endsolid ";
	string nr = "\r\n";

	QFileInfo fi(_filepath);
	file_name = fi.baseName();
	std::ofstream ofs(qPrintable(_filepath));
	ofs.precision(20);
	ofs << solid << file_name.toStdString() << nr;
	vector<int> OnePair;
	if (All_VecDir.size()!=0)
	{
		for (int i = 0; i < Rbox.size(); i++)
		{
			vector<PFACETTRI> vec_face = Rbox[i].OneBlock;
			 OnePair = Rbox[i].vec_PairBlock;
			ofs << "  " << nblock << i << nr;//输出面片所属块
			ofs << "  " << DirOpti << " " << All_VecDir[i].dx << " " << All_VecDir[i].dy << " " << All_VecDir[i].dz << nr;//保存最优方向
			for (unsigned j = 0; j < OnePair.size(); j++)
			{
				ofs << "  " << PairBlock << " " << OnePair[j] << nr;//输出块邻接信息
			}
			for (unsigned j = 0; j < vec_face.size(); j++)
			{
				const PFACETTRI pf = vec_face[j];
				const PVECTOR3D mFaceNormal = vec_face[i]->m_PFacetNorm;

				ofs << "  " << facetNormal << mFaceNormal->dx << " " << mFaceNormal->dy << " " << mFaceNormal->dz << nr;
				ofs << "    " << outerloop << nr;
				for (unsigned k = 0; k < 3; k++)
				{
					PVERT pt = pf->m_PVerts[k];
					ofs << "      " << vertex << pt->x << " " << pt->y << " " << pt->z << nr;
				}
				ofs << "    " << endloop << nr;
				ofs << "  " << endfacet << nr;
			}
		}
	}
	else
	{
		for (int i = 0; i < Rbox.size(); i++)
		{
			vector<PFACETTRI> vec_face = Rbox[i].OneBlock;
			OnePair = Rbox[i].vec_PairBlock;
			ofs << "  " << nblock << i << nr;//输出面片所属块
			for (unsigned j = 0; j < OnePair.size(); j++)
			{
				ofs << "  " << PairBlock << OnePair[j] << nr;//输出块邻接信息
			}
			for (unsigned j = 0; j < vec_face.size(); j++)
			{
				const PFACETTRI pf = vec_face[j];
				const PVECTOR3D mFaceNormal = vec_face[i]->m_PFacetNorm;

				ofs << "  " << facetNormal << mFaceNormal->dx << " " << mFaceNormal->dy << " " << mFaceNormal->dz << nr;
				ofs << "    " << outerloop << nr;
				for (unsigned k = 0; k < 3; k++)
				{
					PVERT pt = pf->m_PVerts[k];
					ofs << "      " << vertex << pt->x << " " << pt->y << " " << pt->z << nr;
				}
				ofs << "    " << endloop << nr;
				ofs << "  " << endfacet << nr;
			}
		}
	}
	
	ofs << endsolid << file_name.toStdString();
}
bool STLModell::write_stl(STLModell* pMeshTri, FILE *f)
{
	bool need_swap = !we_are_little_endian();

	char header[80];
	memset(header, ' ', 80);
	fwrite(header, 80, 1, f);

	int nfaces = pMeshTri->m_vecPFacetTri.size();
	if (need_swap)
		swap_int(nfaces);

	fwrite(&nfaces, 4, 1, f);

	for (size_t i = 0; i < pMeshTri->m_vecPFacetTri.size(); i++)
	{
		float fbuf[12];
		PVECTOR3D tn = pMeshTri->m_vecPFacetTri[i]->m_PFacetNorm;
		fbuf[0] = tn->dx; fbuf[1] = tn->dy; fbuf[2] = tn->dz;
		fbuf[3] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[0]->x;
		fbuf[4] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[0]->y;
		fbuf[5] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[0]->z;
		fbuf[6] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[1]->x;
		fbuf[7] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[1]->y;
		fbuf[8] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[1]->z;
		fbuf[9] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[2]->x;
		fbuf[10] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[2]->y;
		fbuf[11] = pMeshTri->m_vecPFacetTri[i]->m_PVerts[2]->z;
		if (need_swap) {
			for (int j = 0; j < 12; j++)
				swap_float(fbuf[j]);
		}
		fwrite(fbuf, 48, 1, f);

		unsigned char att[2] = { 0, 0 }; 
		fwrite(att, 2, 1, f);
	}
	fclose(f);
	return true;
}

void STLModell::Write_STL(STLModell* pMeshTri, const QString _filepath)
{
	QString file_name;
	string facetNormal = "facet normal ";
	string outerloop = "outer loop";
	string vertex = "vertex ";
	string endloop = "endloop";
	string endfacet = "endfacet";
	string solid = "solid ";
	string endsolid = "endsolid ";
	string nr = "\r\n";

	QFileInfo fi(_filepath);
	file_name = fi.baseName();
	std::ofstream ofs(qPrintable(_filepath));
	ofs.precision(20);
	ofs << solid << file_name.toStdString() << nr;
	for (unsigned i = 0; i < pMeshTri->m_vecPFacetTri.size(); i++)
	{
		const PFACETTRI pf = pMeshTri->m_vecPFacetTri[i];
		const PVECTOR3D mFaceNormal = pMeshTri->m_vecPFacetTri[i]->m_PFacetNorm;
		ofs << "  " << facetNormal << mFaceNormal->dx << " " << mFaceNormal->dy << " " << mFaceNormal->dz << nr;
		ofs << "    " << outerloop << nr;
		for (unsigned k = 0; k < 3; k++)
		{
			PVERT pt = pf->m_PVerts[k];
			ofs << "      " << vertex << pt->x << " " << pt->y << " " << pt->z << nr;
		}
		ofs << "    " << endloop << nr;
		ofs << "  " << endfacet << nr;
	}
	ofs << endsolid << file_name.toStdString();
}

bool STLModell::we_are_little_endian()
{
	char buf[4];
	*(int *)(&buf[0]) = 1;
	return (buf[0] == 1);
}

void STLModell::swap_int(int &x)
{
	swap_unsigned(*(unsigned *)(&x));
}

void STLModell::swap_float(float &x)
{
	swap_unsigned(*(unsigned *)(&x));
}


void STLModell::swap_unsigned(volatile unsigned &x)
{
	x = (x << 24u) |
		((x << 8u) & 0x00ff0000u) |
		((x >> 8u) & 0x0000ff00u) |
		(x >> 24u);
}


bool STLModell::IsTextFile(const char *strFileName)
{
	if (strFileName == NULL) return false;
	struct stat f_stat;
	if (stat(strFileName, &f_stat) == -1)
	{
		return false;
	}
	long lngFilesize = 0;
	lngFilesize = (long)f_stat.st_size;
	if (lngFilesize <= 0) return false;
	FILE *ft = fopen(strFileName, "rb");
	if (ft == NULL)
	{
		return false;
	}
	char btData[1];
	long lngReadPos;
	long lngMaxRead = 40960;
	if (lngMaxRead > lngFilesize) lngMaxRead = lngFilesize;
	for (lngReadPos = 1; lngReadPos <= lngMaxRead; lngReadPos++)
	{
		fread(&btData, 1, 1, ft);
		if (btData[0] == '\0')
		{
			fclose(ft);
			return false;
		}
	}
	fclose(ft);
	return true;
}

void STLModell::Zooommesh(STLModell* pMeshTri, STLModell* pMeshTemp, float _xzoom, float _yzoom, float _zzoom)
{
	map<VERT, int>map_Vert;
	map<VERT, int>::iterator iter;
	for (int i = 0; i < pMeshTri->m_vecPFacetTri.size(); i++)//将三角面片放入容器中
	{
		PFACETTRI PFat; PFat = new FACETTRI; PVERT p0;
		for (int j = 0; j < 3; j++)
		{
			p0 = new VERT;
			p0->x = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->x*_xzoom;
			p0->y = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->y*_yzoom;
			p0->z = pMeshTri->m_vecPFacetTri[i]->m_PVerts[j]->z*_zzoom;
			iter = map_Vert.find(*p0);
			if (iter != map_Vert.end())//已存在该点
			{
				int Num = iter->second;
				PFat->m_PVerts[j] = pMeshTemp->m_vecPVert[Num];
			}
			else
			{
				map_Vert.insert(pair<VERT, int>(*p0, pMeshTemp->m_vecPVert.size()));
				pMeshTemp->m_vecPVert.push_back(p0);
				PFat->m_PVerts[j] = p0;
			}
		}
		////////求法矢////////
		VECTOR3D v1 = *PFat->m_PVerts[1] - *PFat->m_PVerts[0];
		VECTOR3D v2 = *PFat->m_PVerts[2] - *PFat->m_PVerts[1];
		PVECTOR3D v3 = new VECTOR3D;
		*v3 = v1*v2;
		v3->Normalize();
		PFat->m_PFacetNorm = v3;
		pMeshTemp->m_vecPFacetTri.push_back(PFat);
	}
}
void STLModell::Draw(drawTool *pDC)
{
	COLORREF old;
	pDC->GetMaterialColor(old);
	for (int i = 0; i < m_vecPFacetTri.size(); i++)
	{
		pDC->SetColor(old);
		pDC->SetMaterialColor(old);
		pDC->DrawTriChip(m_vecPFacetTri[i]->m_PFacetNorm->dx, m_vecPFacetTri[i]->m_PFacetNorm->dy, m_vecPFacetTri[i]->m_PFacetNorm->dz,
			m_vecPFacetTri[i]->m_PVerts[0]->x, m_vecPFacetTri[i]->m_PVerts[0]->y, m_vecPFacetTri[i]->m_PVerts[0]->z,
			m_vecPFacetTri[i]->m_PVerts[1]->x, m_vecPFacetTri[i]->m_PVerts[1]->y, m_vecPFacetTri[i]->m_PVerts[1]->z,
			m_vecPFacetTri[i]->m_PVerts[2]->x, m_vecPFacetTri[i]->m_PVerts[2]->y, m_vecPFacetTri[i]->m_PVerts[2]->z);
	}
	pDC->SetMaterialColor(old);
}
bool STLModell::IsEmpty()
{
	return true;
}

void STLModell::UpdateBox()
{
	double x0, y0, z0, x1, y1, z1;
	x0 = x1 = m_vecPFacetTri[1]->m_PVerts[0]->x;
	y0 = y1 = m_vecPFacetTri[1]->m_PVerts[0]->y;
	z0 = z1 = m_vecPFacetTri[1]->m_PVerts[0]->z;

	FACETTRI* ptri;
	for (int i = 0; i < m_vecPFacetTri.size(); i++)
	{
		ptri = m_vecPFacetTri[i];
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
	m_Box.x0 = x0;
	m_Box.y0 = y0;
	m_Box.z0 = z0;
	m_Box.x1 = x1;
	m_Box.y1 = y1;
	m_Box.z1 = z1;
	m_bModified = FALSE;

}
void STLModell::GetBox()
{
	UpdateBox();
	box = m_Box;
	ONE.x0 = m_Box.x0;
	ONE.y0 = m_Box.y0;
	ONE.z0 = m_Box.z0;
	ONE.x1 = m_Box.x1;
	ONE.y1 = m_Box.y1;
	ONE.z1 = m_Box.z1;
}
void  STLModell::GetMainBlockBox(vector<PFACETTRI>  OneBlock,BOX3D &ONE)
{
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
}

void STLModell::CreateEdgeTopByFctVert(STLModell* pMeshTri)
{
	int	nFct, nFCnt;
	PFACETTRI pFct;

	int	nCnt;
	int nCntTmp;

	set<PEDGE, CompPEDGE> setEdge;
	pair<set<PEDGE, CompPEDGE>::iterator, bool> pair_setEdge_it_b;

	nCnt = pMeshTri->m_vecPEdge.size();
	for (nCntTmp = 0; nCntTmp < nCnt; nCntTmp++)
	{
		delete pMeshTri->m_vecPEdge[nCntTmp];
	}
	pMeshTri->m_vecPEdge.clear();

	nCnt = pMeshTri->m_vecPHEdge.size();
	for (nCntTmp = 0; nCntTmp < nCnt; nCntTmp++)
	{
		delete pMeshTri->m_vecPHEdge[nCntTmp];
	}
	pMeshTri->m_vecPHEdge.clear();

	nFct = pMeshTri->m_vecPFacetTri.size();
	for (nFCnt = 0; nFCnt < nFct; nFCnt++)
	{
		pFct = pMeshTri->m_vecPFacetTri[nFCnt];

		PHEDGE pHEdge[3];
		pHEdge[0] = new HEDGE; pHEdge[1] = new HEDGE; pHEdge[2] = new HEDGE;

		//pHEdge[0]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri->m_vecPHEdge.push_back(pHEdge[0]);//FOR Lin JunFeng
		//pHEdge[1]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri->m_vecPHEdge.push_back(pHEdge[1]);
		//pHEdge[2]->m_IDHEdge = pMeshTri->m_vecPHEdge.size();
		pMeshTri->m_vecPHEdge.push_back(pHEdge[2]);//FOR Lin JunFeng

		for (nCnt = 0; nCnt < 3; nCnt++)
		{
			nCntTmp = (nCnt + 1) % 3;
			//半边拓扑信息
			pHEdge[nCnt]->pFacetAdj = pFct;
			pHEdge[nCnt]->pHEdgeNext = pHEdge[nCntTmp];
			pHEdge[nCnt]->pHEdgePre = pHEdge[(nCnt + 2) % 3];
			pHEdge[nCnt]->pVertEnd = pFct->m_PVerts[nCntTmp];

			//顶点拓扑信息
			pFct->m_PVerts[nCnt]->pHEdgeOut = pHEdge[nCnt];//这里好像不需要判断pHEdgeOut是否已经被赋值了，你怎么看 ^_^

			//边拓扑信息
			PEDGE	pEdge = new EDGE;
			if (pFct->m_PVerts[nCnt] < pFct->m_PVerts[nCntTmp])
			{
				pEdge->pVertStart = pFct->m_PVerts[nCnt];
				pEdge->pVertEnd = pFct->m_PVerts[nCntTmp];
			}
			else
			{
				pEdge->pVertEnd = pFct->m_PVerts[nCnt];
				pEdge->pVertStart = pFct->m_PVerts[nCntTmp];
			}

			pair_setEdge_it_b = setEdge.insert(pEdge);
			if (pair_setEdge_it_b.second)
			{
				pEdge->pHEdgeAdj = pHEdge[nCnt];

				//pEdge->m_nIDEdge = pMeshTri->m_vecPEdge.size();
				pMeshTri->m_vecPEdge.push_back(pEdge);
			}
			else
			{
				delete pEdge;
				pEdge = 0;

				//完善半边拓扑信息
				pHEdge[nCnt]->pHEdgePair = (*pair_setEdge_it_b.first)->pHEdgeAdj;
				(*pair_setEdge_it_b.first)->pHEdgeAdj->pHEdgePair = pHEdge[nCnt];
			}
		}
		//面片拓扑信息
		pFct->pHEdge = pHEdge[0];

	}

}

MATRIX3D STLModell::CreateRotateMatrix(double da, VECTOR3D v)
{
	MATRIX3D R;
	VECTOR3D bv(v);

	if (IS_ZERO(da)) return R;

	Q_ASSERT(!bv.IsZeroLength());

	double lxy = bv.GetLengthXY();

	if (IS_ZERO(lxy))
	{
		if (bv.dz < 0.0)   da *= -1.0;
		R.A[0][0] = R.A[1][1] = cos(da);
		R.A[0][1] = sin(da);
		R.A[1][0] = -sin(da);
		return R;
	}

	double lyz = bv.GetLengthYZ();
	if (IS_ZERO(lyz))
	{
		if (bv.dx < 0.0)   da *= -1.0;
		R.A[2][2] = R.A[1][1] = cos(da);
		R.A[1][2] = sin(da);
		R.A[2][1] = -sin(da);
		return R;
	}
	double lxz = bv.GetLengthZX();
	if (IS_ZERO(lxz))
	{
		if (bv.dx < 0.0)   da *= -1.0;
		R.A[0][0] = R.A[2][0] = cos(da);
		R.A[0][2] = sin(da);
		R.A[2][1] = -sin(da);
		return R;
	}
	MATRIX3D Rz;
	Rz.A[0][0] = Rz.A[1][1] = bv.dy / lxy;
	Rz.A[0][1] = Rz.A[1][0] = -bv.dx / lxy;

	double len = bv.GetLength();
	MATRIX3D Rx;
	Rx.A[2][2] = Rx.A[1][1] = bv.dy / lxy;
	Rx.A[1][2] = lxy / len; Rx.A[2][1] = len;

	R.A[0][0] = R.A[1][1] = cos(da);
	R.A[0][1] = sin(da); R.A[1][0] = -sin(da);

	MATRIX3D Rxn;
	Rxn.A[2][2] = Rxn.A[1][1] = bv.dz / len;
	Rxn.A[2][1] = lxy / len; Rxn.A[1][2] = -lxy / len;
	MATRIX3D Rzn;
	Rzn.A[0][0] = Rzn.A[1][1] = bv.dy / lxy;
	Rzn.A[1][0] = bv.dx / lxy; Rzn.A[0][1] = -bv.dx / lxy;
	return Rz*Rx*R*Rxn*Rzn;
}
MATRIX3D STLModell::CreateTranMatrix(VECTOR3D v)
{
	MATRIX3D R;
	VECTOR3D bv(v);
	R.A[0][3] =bv.dx;
	R.A[1][3] = bv.dy;
	R.A[2][3] = bv.dz;
	R.A[0][0] = R.A[1][1] = R.A[2][0]=R.A[3][3] = 1;
	return R;
}