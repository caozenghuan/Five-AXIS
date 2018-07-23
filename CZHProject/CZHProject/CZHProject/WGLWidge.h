#pragma  once
#include "QGLWidget"
#include "datastructure/GCamera.h"
#include "fenceng.h"
#include "MeshBlock.h"
//#include"Optisequence.h"
#include "DlgBlockSeq.h"

#include "BackGround.h"
#include "viewspace/ShowShader.h"
#include <QOpenGLFunctions_3_3_Core >
#include<QMessageBox>
#include <memory>
#define BUFFER_LENGTH 64

#define VIEW_FRONT         0
#define VIEW_BACK          1
#define VIEW_TOP           2
#define VIEW_BOTTOM        3
#define VIEW_RIGHT         4
#define VIEW_LEFT          5
#define VIEW_SW_ISOMETRIC  6
#define VIEW_SE_ISOMETRIC  7
#define VIEW_NE_ISOMETRIC  8
#define VIEW_NW_ISOMETRIC  9

#define ZOOM_ALL           10
#define ZOOM_IN            11
#define ZOOM_OUT           12

using namespace ViewSpace;
class WGLWidge :public QGLWidget,protected QOpenGLFunctions_3_3_Core
{
	Q_OBJECT
public:
	WGLWidge();
	virtual ~WGLWidge();

	int BkMode;
	bool m_box,m_board, fullscreen;
	vector<vec_POINT3D> p_Row;//平台点集
	vector<vec_POINT3D> vec_Plat;//平台面片

	vector<vec_POINT3D> p_RowRotate;//平台点集
	vector<vec_POINT3D> vec_PlatRotate;//平台面片

	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();                             //绘图函数
	void DrawCoord();                          //绘制用户坐标系

private:

	COLORREF m_clrBk;       //窗口背景色
	COLORREF m_clrHighlight;//用于高亮显示时的模型颜色，如拾取一个物体时需要高亮度显示
	COLORREF m_clrMaterial; //材料的颜色
	BOOL     m_bShading;    //是否采用着色显示
	GLfloat  m_vecLight[3]; //光源方向

	BOOL     m_bSelectionMode;            //当前是否是选择模式

	float *positionData;
	GLuint vaoHandle;
	GLuint m_VertexBuffer, m_NormalBuffer, m_IDBuffer;
	GLuint   m_selectBuff[BUFFER_LENGTH]; //选择缓存区

	ShaderModel     m_ShaderModel;
	GCamera *m_camera; drawTool *pDC;
	
	
public:
	void Zoom(double dScale);     // 缩放场景
	void ZoomAll(BOX3D box);           // 计算一个合适的缩放比，以将模型全部显示在场景中
	void OnViewType(UINT type);   // 使用典型视角来观察模型
	void MoveView(double dpx, double dpy);// 按当前场景尺寸的百分比移动场景，参数dpx、dpy的范围是0~1
	void RotateView(double drot, short snXyz);

	/////////////OPENGL类的设置////////////////////////////
protected:
	void ClearBKground(); //清除背景颜色
	void OnShading();     //光照/非光照模式设置

public:
	void GLSetupRC();                 //设置渲染场境
	//////////////////对光照与颜色的操作函数//////////////////////

	void Shading(BOOL bShading);        //光照/非光照模式切换
	BOOL IsShading();                   //当前是否是着色模式

	void Lighting(BOOL bLighting);      //是否使用光源
	BOOL IsLighting();

	void CreatLight();                 //刘斌组创建光源
	void SetLightDirection(float dx, float dy, float dz);  //设置与获取光源方向
	void GetLightDirection(float& dx, float& dy, float& dz);

	void SetBkColor(COLORREF rgb);                 //设置与获取背景颜色
	void GetBkColor(COLORREF& rgb);

	void SetHighlightColor(COLORREF clr);         //设置与获取高亮度显示的颜色
	void GetHighlightColor(COLORREF& clr);

	void Highlight(BOOL bLight = TRUE);           //高亮度/正常显示切换

	POINT3D BeginSelection(int xPos, int yPos);
	int EndSelection(UINT* items);
	void initVBO(STLModell  * pSTLTemp);         //新的OPENGL显示函数
	void  deleteVBO();
	void deleteModel(STLModell  *MySTL);

	bool ceshi;
	vector<vec_POINT3D> vec_Ceshi0; vector<vec_POINT3D> vec_Ceshi1;
	vector<vec_POINT3D> vec_HeadBox;//打印头实体模型
	vector<vec_POINT3D> vec_PlanBoard;
	void CreatBoxcir(POINT3D Pcenter, VECTOR3D DirV);//创建打印喷头模型
	POINT3D CalCenterPoint(MyBlock CurrnetBlock, VECTOR3D DirV);//计算当前块的最底层中心点
	void RotateHeadBox(vector<vec_POINT3D> &vec_face, VECTOR3D curr_Dir, VECTOR3D tager_Dir);//旋转打印头包围盒

private:
	
    void GeneratePlatform(POINT3D pr0);//绘制平台板
	void DrawBlockDirResult();//绘制块方向优化结果
	void DrawSliceResult();//绘制切片结果
	void DrawFillResult();//绘制填充结果
	void DrawSeqLayer();//绘制层显示效果
public:
	bool m_Headbox,m_Dirback;
	bool m_bIfShowSTL,m_IFSimple,m_Block,m_optiSeQ,m_slice, m_Fill;
	bool m_SliceSeqShow,m_Alltractory;//按照块加工顺序分层显示加工轨迹
	bool m_cutline, Projectway = false;
	
	int Nublcok;
	POINT3D Plow;//当前子块的最低点
	POINT3D PMainlow;//主子块的最低点
	vector<vector<PFACETTRI>>  ALLBlock;


	STLModell  *pSTL; STLModell *pMeshZoom; STLModell *PSimple;
	fenceng *pslice; MeshBlock Pmeshblock;
	STLModell  *Pmblock; STLModell *PmblockTemp;//追踪投影法保存分割数据
	COptisequence pSEQ;  DlgBlockSeq dlg;

	int NCurrentlayer;
	vec_PPOLYPOLYGON lunkuo, Fill;
	vector<vec_PPOLYPOLYGON> Blocklunkuo;//子块切片轨迹
	vector<vec_PPOLYPOLYGON> BlockFill;//子块填充轨迹

	vec_PPOLYPOLYGON SeqlunkuoLayer;//顺序显示所有子块切片轨迹
	vec_PPOLYPOLYGON SeqFill;//顺序显示所有子块填充轨迹


	vector<vector<PFACETTRI>>  SimALLBlock;//仿真所有子块位姿
	vector<vec_PPOLYPOLYGON> ToolpathLayer;//顺序显示子块切片控制程序轨迹
	vector<vec_PPOLYPOLYGON> ToolpathFill;//顺序显示子块填充控制程序轨迹
	////////////分块优化变量////////////
	int nBlock,nShowBdir;
	vector<vector<int>> All_PairBlock;
	vector<vec_PVERT> CFpoint; bool CF;
	vector<vec_PVERT> FeaPointAll;
	///////////////方向优化变量///////////////
	bool       m_bOptiDir;
	VECTOR3D  m_VecDir;
	vector<VECTOR3D> All_VecDir;
	vector<vector<int>> 	m_vecFacetNum;
	vector<vector<vector<int>>> All_vecFacetNum;
	bool m_IfshowDir, m_Beoptir;//方向优化开关

	bool m_SwichBlock,Sblock;
	vector<vec_POINT3D> modelPOINT;
	vec_POINT3D  mPOINTTemp;//拾取点

	vector<vector<PVERT>>path;
	vector<vector<Insepoint>> pathTrace;//追踪投影法轨迹线
	vector<MyBlock> Rbox;//保存模型分割后的结果
	vector<vector<PVERT>> RBLOOP;//分割线光顺数据保存

	vec_PPOLYPOLYGON BlockEdge;//存储块边界顶点
};