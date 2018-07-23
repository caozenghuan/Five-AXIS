#include "myqt.h"
#include <QtGui>
#include <QtCore>
#include <QtOpenGL>
MyQT::MyQT(QWidget *parent)
	: QMainWindow(parent)
{
	ui = new Ui::MyQTClass;
	ui->setupUi(this);
	//fullscreen = false;
	opgl = new WGLWidge;
	opgl->setStyleSheet(QStringLiteral("border:5px black;"));
	ui->LapdeformFeature->setChecked(true);
	ui->actionmenu->setChecked(true);
	ui->scrollArea->setWidget(opgl);

	ui->BoxLayer->setMinimum(0);
	ui->BoxLayer->setMaximum(1000);
	opgl->m_bIfShowSTL = false;
	m_VecDir = VECTOR3D(0, 0, 1);
	opgl->m_VecDir = m_VecDir;
	setprogressbar();//初始化所有进度条
	rate = 30;
	ratepercent = 90;
	m_equdept = 1;
	m_mindept = 0.5;
	Diamer = 0.5;
	m_laplacian = 2;
}

MyQT::~MyQT()
{
	MouseDown = 0;
	if (opgl||ui)
	{
		delete opgl;
		delete ui;
		ui = NULL;
		opgl = NULL;
	}
}

void MyQT::paintEvent(QPaintEvent *e)
{
	
	
}
void MyQT::setprogressbar()
{
	ui->progressBar->hide();
	//ui->Roundbar->hide();
	ui->simplyprogressBar->hide();
	ui->OptiSeQProgressBar->hide();
	///////////////设置分层进度条/////////////////////////
	// donut (customized via QPalette + gradient)
	/*QPalette p1;
	p1.setBrush(QPalette::AlternateBase, Qt::black);
	p1.setColor(QPalette::Text, Qt::yellow);
	ui->SliceRoadbar->setPalette(p1);
	ui->SliceRoadbar->setNullPosition(QRoundProgressBar::PositionLeft);
	ui->SliceRoadbar->setDecimals(0);
	// make a gradient from green over yellow to red
	QGradientStops gradientPoints;
	gradientPoints << QGradientStop(0, Qt::green) << QGradientStop(0.5, Qt::yellow) << QGradientStop(1, Qt::red);*/
	///////////////////////////////////////////////////////
	ui->SliceProgressBar->hide();
	ui->SliceRoadbar->hide();
}
void MyQT::Displayparameter(int choe)
{
	//opgl->OnViewType(VIEW_NE_ISOMETRIC);
	if (choe==1)
	{
		ui->label_before->setText(QString::fromLocal8Bit("  简化前参数"));
		QString Facesum = QString::number(opgl->pSTL->m_vecPFacetTri.size());
		QString Vetexsum = QString::number(opgl->pSTL->m_vecPVert.size());
		ui->label_Face->setText(QString::fromLocal8Bit("面片数："));
		ui->lineEditFACE->setText(Facesum);
		ui->label_Vertex->setText(QString::fromLocal8Bit("顶点数："));
		ui->lineEditVETEX->setText(Vetexsum);
		QString Edgesum = QString::number(opgl->pSTL->m_vecPEdge.size());
		ui->label_Phedge->setText(QString::fromLocal8Bit("边 数："));
		ui->lineEditEDGE->setText(Edgesum);
	}
	else
	{
		ui->label_After->setText(QString::fromLocal8Bit("  简化后参数"));
		QString Facesum = QString::number(opgl->PSimple->m_vecPFacetTri.size());
		QString Vetexsum = QString::number(opgl->PSimple->m_vecPVert.size());
		ui->label_Face1->setText(QString::fromLocal8Bit("面片数："));
		ui->lineEditFACE1->setText(Facesum);
		ui->label_Vertex1->setText(QString::fromLocal8Bit("顶点数："));
		ui->lineEditVETEX1->setText(Vetexsum);
		QString Edgesum = QString::number(opgl->PSimple->m_vecPEdge.size());
		ui->label_Phedge1->setText(QString::fromLocal8Bit("边 数："));
		ui->lineEditEDGE1->setText(Edgesum);
		ui->label_stime->setText(QString::fromLocal8Bit("  简化耗时(s)"));
		QString Siptime = QString::number(simpletime);
		ui->simpletimevalue->setText(Siptime);
	}
}
void MyQT::FileOpen()
{
	QString strName = QFileDialog::getOpenFileName(this, tr("OpenFiles"), "*.stl/*.stl", tr("stlfile（*.stl)::Allfiles(*.*)"));
	if (strName != NULL)
	{
		if (opgl->pSTL==0)
		{
			//opgl->pSTL = STLModell::GetInstance();
			opgl->pSTL = new STLModell();
		}
		else
		{
			KitTool kit;
			kit.ClearTheList(opgl->pSTL);
			opgl->BlockEdge.clear();
			opgl->Rbox.clear(); opgl->All_VecDir.clear();
			opgl->lunkuo.clear(); opgl->Blocklunkuo.clear();
			opgl->Fill.clear(); opgl->BlockFill.clear();
		}
	    QByteArray text = strName.toLocal8Bit();
		ui->progressBar->show();
		const char *data = text.data();
		if (opgl->pSTL->IsTextFile(text))
		{
			opgl->pSTL->LoadTextSTLFile(text, opgl->All_VecDir, opgl->All_PairBlock, ui->progressBar);
			for (int i = 0; i < opgl->pSTL->m_vecPFacetTri.size();i++)
			{
				if (opgl->pSTL->m_vecPFacetTri[i]->Nblock)
				{
					opgl->m_Block = true; 
					
					opgl->Pmeshblock.Getreadblock(opgl->pSTL->m_vecPFacetTri, opgl->All_PairBlock, opgl->Rbox);//按块读入并保存分割模型
					//opgl->dlg.FindTheLow(opgl->Rbox[0].OneBlock, opgl->PMainlow);
					//opgl->pSEQ.TranRbox(opgl->Rbox, opgl->PMainlow);
					break;
				}
			}
		}
		else
		{
			opgl->pSTL->LoadSTLFile(text, ui->progressBar);
		}
		if (!opgl->m_Block)
		{
			opgl->m_bIfShowSTL = true;
		}
		opgl->initVBO(opgl->pSTL);
		opgl->pSTL->GetBox();
		STLModell::CreateEdgeTopByFctVert(opgl->pSTL);
		Displayparameter(1);
	}
	ui->progressBar->hide();
}
void MyQT::keyPressEvent(QKeyEvent *e)
{
	switch (e->key())
	{
	case  Qt::Key_F1:
		opgl->fullscreen = !opgl->fullscreen;
		if (opgl->fullscreen)
		{
			showFullScreen();
		}
		else
		{
			showNormal();
		}
		break;
	case  Qt::Key_Escape:
		close();
	}
}
//////////视图变换函数//////////
void MyQT::ViewBack()
{
	opgl->OnViewType(VIEW_BACK);
}
void MyQT::ViewBottom()
{
	opgl->OnViewType(VIEW_BOTTOM);
}
void MyQT::ViewFront()
{
	opgl->OnViewType(VIEW_FRONT);
}
void MyQT::ViewLeft()
{
	opgl->OnViewType(VIEW_LEFT);
}
void MyQT::ViewRight()
{
	opgl->OnViewType(VIEW_RIGHT);
}
void MyQT::ViewTop()
{
	opgl->OnViewType(VIEW_TOP);
}
void MyQT::ViewIsometric()
{
	QString textchoose=ui->IsometricView->currentText();
	QString n1 = QString::fromLocal8Bit("西南轴侧视图");
	QString n2 = QString::fromLocal8Bit("东南轴侧视图");
	QString n3 = QString::fromLocal8Bit("东北轴侧视图");
	QString n4 = QString::fromLocal8Bit("西北轴侧视图");
	if (textchoose == n1)
	{
		opgl->OnViewType(VIEW_SW_ISOMETRIC);
	}
	if (textchoose == n2)
	{
		opgl->OnViewType(VIEW_SE_ISOMETRIC);
	}
	if (textchoose == n3)
	{
		opgl->OnViewType(VIEW_NE_ISOMETRIC);
	}
	if (textchoose == n4)
	{
		opgl->OnViewType(VIEW_NW_ISOMETRIC);
	}

}
void MyQT::ViewZoomall()
{
	opgl->Zoom(0.1);
}
void MyQT::ViewZoomin()
{
	opgl->Zoom(1.3);
}
void MyQT::ViewShade()
{
	opgl->Shading(!opgl->IsShading());
}
void MyQT::ShowBoard()
{
	opgl->m_board = !opgl->m_board;
}
void MyQT::ShowBox()
{
	if (opgl->pSTL != 0 && !opgl->m_box)
	{
		double x1, y1,z1;
		x1 = opgl->pSTL->box.x1 - opgl->pSTL->box.x0;
		y1 = opgl->pSTL->box.y1 - opgl->pSTL->box.y0;
		z1 = opgl->pSTL->box.z1 - opgl->pSTL->box.z0;
		BoxDLg mybox;
		mybox.showboxsize(x1,y1,z1);
		mybox.exec();
		opgl->m_box = !opgl->m_box;
	}
	else
	{
		opgl->m_box = !opgl->m_box;
	}
}
void MyQT::Viewshow()
{ 
	opgl->m_bIfShowSTL = !opgl->m_bIfShowSTL;

}
void MyQT::ShowBlock()
{
	opgl->m_Block = !opgl->m_Block;
	opgl->m_bIfShowSTL = !opgl->m_bIfShowSTL;
	opgl->Sblock = false;
	opgl->m_SwichBlock = false;
}
void MyQT::ActionMenu()
{
	CBlackgroundDlg Modechoose;
	int res = Modechoose.exec();
	opgl->BkMode = Modechoose.BlackMode;
}
void MyQT::ActionWelcome()
{
	ui->ActionWelcome->setChecked(true);
	ui->actionToolBlock->setChecked(false);
	ui->actionTooldesign->setChecked(false);
	ui->tabWidget->setCurrentIndex(0);
}
void MyQT::ActionBlock()
{
	ui->actionToolBlock->setChecked(true);
	ui->ActionWelcome->setChecked(false);
	ui->actionTooldesign->setChecked(false);
	ui->tabWidget->setCurrentIndex(1);
}
void MyQT::ActionPathdesign()
{
	ui->actionTooldesign->setChecked(true);
	ui->actionToolBlock->setChecked(false);
	ui->ActionWelcome->setChecked(false);
	ui->tabWidget->setCurrentIndex(2);
}

void MyQT::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)
	{
		opgl->Zoom(0.7);
	}
	else
	{
		opgl->Zoom(1.3);
	}
}

void MyQT::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		MouseDown = 1;
		pOld = event->pos();
		if (event->modifiers() ==  Qt::ControlModifier)
		{
			m_bifCtrl = true;
		}
	}
	if (event->button() == Qt::RightButton)//拾取模型空间二维顶点
	{
		mousepickpoint = event->pos();
		double x1, x2, y1, y2, z1, z2;
		x1 = opgl->pSTL->box.x1; x2 = opgl->pSTL->box.x0;
		y1 = opgl->pSTL->box.y1; y2 = opgl->pSTL->box.y0;
		z1 = opgl->pSTL->box.z1; z2 = opgl->pSTL->box.z0;
		POINT3D ptenp = ScreenToPoint(mousepickpoint);
		if (ptenp.x<x1&&ptenp.x>x2&&ptenp.y<y1&&ptenp.y>y2&&ptenp.z<z1&&ptenp.z>z2)
		{
			opgl->mPOINTTemp.push_back(ptenp);
			update();
		}
	}
}
void MyQT::CutLineshow()
{
	if (!opgl->m_Block)
	{
		vector<PVERT> path;
		opgl->modelPOINT.push_back(opgl->mPOINTTemp);
		opgl->Pmeshblock.PickPoint(opgl->mPOINTTemp, path, opgl->pSTL);
		//opgl->Pmeshblock.pcboundary->snake_smooth(path);
		opgl->path.push_back(path);
		opgl->mPOINTTemp.clear();
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("恭喜，求解完成"));
	}
	else
	{
		opgl->m_cutline = !opgl->m_cutline;
	}
}
void MyQT::CutLineTrace()
{
	if (!opgl->m_Block)
	{
		bool FindLine = false;
		vector<PVERT> path;
		opgl->modelPOINT.push_back(opgl->mPOINTTemp);
		vector<Insepoint> Tracetemp;
		FindLine = opgl->Pmeshblock.CutLineTraceProjection(opgl->mPOINTTemp, path, Tracetemp, opgl->pSTL);
		if (FindLine)
		{
			opgl->pathTrace.push_back(Tracetemp);
			Tracetemp.clear();
			opgl->path.push_back(path);
			opgl->mPOINTTemp.clear();
			opgl->Projectway = true;
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("恭喜，求解完成"));
		}
		else
		{
			opgl->mPOINTTemp.clear();
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("您的拾取有误，请重新拾取点"));
		}
	}
	else
	{
		opgl->m_cutline = !opgl->m_cutline;
	}
}
void MyQT::CutLineDir()
{
	if (!opgl->pSTL) return;
	if (opgl->All_VecDir.size()!=0&&opgl->Rbox.size()!=0)
	{
		vector<PVERT> path;
		vector<Insepoint> Tracetemp;
		opgl->Pmeshblock.CutLineDir(opgl->FeaPointAll, opgl->CFpoint, opgl->Rbox, opgl->All_VecDir, opgl->path, opgl->pathTrace, opgl->Pmblock);
		//opgl->Pmeshblock.CutLineDir2(opgl->CFpoint,opgl->Rbox, opgl->All_VecDir, opgl->path, opgl->pathTrace,opgl->Pmblock->m_vecPVert);
		opgl->m_cutline = true; opgl->Projectway = true;
		opgl->CF = true;
	}
}
void MyQT::MeshBlockOpti()
{
	if (opgl->Rbox.size() != 0 && opgl->Projectway)
	{
		opgl->Rbox.clear();
		if (opgl->Projectway)
		{
			STLModell  *PmblockDir; bool Ipt = false;
			for (int i = 0; i < opgl->pathTrace.size(); i++)
			{
				if (i == 0)
				{
					PmblockDir = new STLModell;
					opgl->PmblockTemp = new STLModell;
					opgl->Pmeshblock.PkiTool.StoretheMeshTri(opgl->Pmblock, PmblockDir, 0);
				}
				if (opgl->Pmeshblock.IfNeedLoop(opgl->pathTrace[i])) continue;
				else
				{
					if (Ipt)
					{
						opgl->Pmeshblock.PkiTool.ClearTheList(PmblockDir); 
						opgl->Pmeshblock.PkiTool.StoretheMeshTri(opgl->PmblockTemp, PmblockDir, 0);
						opgl->Pmeshblock.PkiTool.ClearTheList(opgl->PmblockTemp);
					}
					Ipt = true;
				}
				STLModell::CreateEdgeTopByFctVert(PmblockDir);
				opgl->Pmeshblock.CutTraceProjection(opgl->pathTrace[i], opgl->PmblockTemp->m_vecPFacetTri, opgl->PmblockTemp->m_vecPVert, PmblockDir);
			}
			//未被更新的边界线用新顶点替换
		
			opgl->Pmeshblock.PkiTool.ClearTheList(PmblockDir);
			opgl->Pmeshblock.Updatecutline(opgl->path, opgl->pathTrace, opgl->PmblockTemp->m_vecPVert);

			opgl->Pmeshblock.PkiTool.ClearTheList(opgl->Pmblock);
			opgl->Pmeshblock.PkiTool.StoretheMeshTri(opgl->PmblockTemp, opgl->Pmblock, 0);

			opgl->Pmeshblock.Updatecutline(opgl->path, opgl->pathTrace, opgl->Pmblock->m_vecPVert);

			opgl->Pmeshblock.PkiTool.ClearTheList(opgl->PmblockTemp);
			STLModell::CreateEdgeTopByFctVert(opgl->Pmblock);
			opgl->Pmeshblock.pcboundary->Skeletonizing_partlize(opgl->Rbox, opgl->path, opgl->Pmblock->m_vecPFacetTri, opgl->Pmblock->m_vecPVert);
		}
		else
		{
			opgl->Pmeshblock.pcboundary->Skeletonizing_partlize(opgl->Rbox, opgl->path, opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPVert);
		}
		opgl->m_Block = true; opgl->m_bOptiDir = false;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("恭喜，分割完成"));
		for (int i = 0; i < opgl->Pmblock->m_vecPVert.size(); i++)//计算所有顶点的法矢
		{
			opgl->Pmblock->m_vecPVert[i]->VertexNormal = KitTool::CalcuVerNormal(opgl->Pmblock->m_vecPVert[i]);
		}
		opgl->Projectway = false; opgl->CF = false;
	}
	else
	{
		opgl->m_Block = !opgl->m_Block; 
	}
}
void MyQT::MeshBlock()
{
	if (!opgl->pSTL) return;
	if (!opgl->Rbox.size())
	{
		opgl->Rbox.clear();
		if (opgl->Projectway)
		{
			for (int i = 0; i < opgl->pathTrace.size(); i++)
			{
				if (i == 0)
				{
					//opgl->Pmblock = STLModell::GetInstance();
					opgl->Pmblock = new STLModell; 
					//opgl->PmblockTemp = STLModell::GetInstance();
					opgl->PmblockTemp = new STLModell;
					opgl->Pmeshblock.PkiTool.StoretheMeshTri(opgl->pSTL, opgl->PmblockTemp, 0);
				}
				else
				{
					opgl->Pmeshblock.PkiTool.ClearTheList(opgl->PmblockTemp);
					opgl->Pmeshblock.PkiTool.StoretheMeshTri(opgl->Pmblock, opgl->PmblockTemp, 0);
					opgl->Pmeshblock.PkiTool.ClearTheList(opgl->Pmblock);
				}
				STLModell::CreateEdgeTopByFctVert(opgl->PmblockTemp);
				opgl->Pmeshblock.CutTraceProjection(opgl->pathTrace[i], opgl->Pmblock->m_vecPFacetTri, opgl->Pmblock->m_vecPVert, opgl->PmblockTemp);
			}
			opgl->Pmeshblock.PkiTool.ClearTheList(opgl->PmblockTemp);
			STLModell::CreateEdgeTopByFctVert(opgl->Pmblock);
			opgl->Pmeshblock.pcboundary->Skeletonizing_partlize(opgl->Rbox, opgl->path, opgl->Pmblock->m_vecPFacetTri, opgl->Pmblock->m_vecPVert);

			for (int i = 0; i < opgl->Pmblock->m_vecPVert.size(); i++)//计算所有顶点的法矢
			{
				opgl->Pmblock->m_vecPVert[i]->VertexNormal = KitTool::CalcuVerNormal(opgl->Pmblock->m_vecPVert[i]);
			}
		}
		else
		{
			opgl->Pmeshblock.pcboundary->Skeletonizing_partlize(opgl->Rbox, opgl->path, opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPVert);
		}
		opgl->m_Block = true;
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("恭喜，分割完成"));
		
	}
	else
	{
		opgl->m_Block = !opgl->m_Block; opgl->m_bIfShowSTL = !opgl->m_bIfShowSTL;
	}
}
POINT3D MyQT::ScreenToPoint(QPoint P)
{
	int hits;
	UINT items[64];
	double x, y, z;
	x = y = z = 0;
	POINT3D PP;
	PP = opgl->BeginSelection(P.x(), P.y());
	hits = opgl->EndSelection(items);
	return PP;
}
void MyQT::Detectedge()
{
	if (opgl->pSTL!=0)
	{
		opgl->Pmeshblock.SharpEdgeDetection(opgl->pSTL, opgl->Pmeshblock.vec_edge);
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("特征边已识别显示!"));
	}
}
void MyQT::Detectvetex()
{
	if (opgl->pSTL != 0)
	{
		if (m_laplacian==0)
		{
			opgl->Pmeshblock.pLaplacian->change(opgl->pSTL->m_vecPVert, opgl->pSTL, opgl->Pmeshblock.m_vecLp, opgl->Pmeshblock.m_vecV);//拉普拉斯变形
			opgl->m_bIfShowSTL = false;
			opgl->initVBO(opgl->pSTL);
			opgl->Pmeshblock.m_Laplacian = true;
		}
		if (m_laplacian == 1)
		{
			opgl->Pmeshblock.pLaplacian->back(opgl->pSTL->m_vecPVert, opgl->pSTL, opgl->Pmeshblock.m_vecLp, opgl->Pmeshblock.m_vecV);//恢复变形
			opgl->m_bIfShowSTL = true;
			opgl->Pmeshblock.m_Laplacian = false;
		}
		if (m_laplacian ==2)
		{
			RoughdetectDlg Rdetect;
			Rdetect.exec();
			
			if (opgl->PSimple)
			{
				opgl->Pmeshblock.featurPOINT(opgl->PSimple->m_vecPFacetTri, opgl->PSimple->m_vecPHEdge, opgl->PSimple->m_vecPVert, Rdetect.Limitvalue);//我的方法搜索特征点
				//opgl->Pmeshblock.pcboundary->featurPOINT(opgl->PSimple->m_vecPFacetTri, opgl->PSimple->m_vecPHEdge, opgl->PSimple->m_vecPVert, Rdetect.Limitvalue);//师兄方法搜索特征点
			}
			else
			{
				//opgl->pSimply->featurPOINT(opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPHEdge, opgl->pSTL->m_vecPVert, Rdetect.Limitvalue);//我的方法搜索特征点
				//opgl->Pmeshblock.pcboundary->featurPOINT(opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPHEdge, opgl->pSTL->m_vecPVert, Rdetect.Limitvalue);//师兄方法搜索特征点

				opgl->Pmeshblock.pLaplacian->change(opgl->pSTL->m_vecPVert, opgl->pSTL, opgl->Pmeshblock.m_vecLp, opgl->Pmeshblock.m_vecV);//拉普拉斯变形
				opgl->Pmeshblock.pcboundary->featurPOINT(opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPHEdge, opgl->pSTL->m_vecPVert, Rdetect.Limitvalue);
				opgl->Pmeshblock.pLaplacian->back(opgl->pSTL->m_vecPVert, opgl->pSTL, opgl->Pmeshblock.m_vecLp, opgl->Pmeshblock.m_vecV);//恢复变形
			}
		}
	}
}
void MyQT::Skeletonizing()
{
	if (!opgl->pSTL)return;
	TanshexianDlg T;
	T.exec();
	opgl->Pmeshblock.pcboundary->Skeletonizing(opgl->RBLOOP, opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPHEdge, opgl->pSTL->m_vecPVert, opgl->pSTL->m_vecPEdge);
	//opgl->Pmeshblock.pcboundary->Skeletonizing_close(opgl->RBLOOP, opgl->pSTL->m_vecPFacetTri, opgl->pSTL->m_vecPVert, opgl->pSTL->m_vecPHEdge, T.angel);
}
void MyQT::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		MouseDown = 0;
		m_bifCtrl = false;
	}
}
void MyQT::mouseMoveEvent(QMouseEvent *event)
{
	if (MouseDown == 1)
	{
		int nMoveX, nMoveY;
		QPoint pCurrent;
		pCurrent = event->pos();
		nMoveX = pCurrent.x() - pOld.x();
		nMoveY = pOld.y() - pCurrent.y();
		pOld = pCurrent;
		
		if (m_bifCtrl == true)
			opgl->MoveView(0.001*nMoveX, 0.001*nMoveY);
		else
		{
			if (nMoveX > 3)
			{
				opgl->RotateView(ROTATE_ANGLE, 1);
			}
			else
			{
				if (nMoveX < -3)
				{
					opgl->RotateView(ROTATE_ANGLE_NEG, 1);
				}
			}
			if (nMoveY > 3)
			{
				opgl->RotateView(ROTATE_ANGLE_NEG, 0);
			}
			else
			{
				if (nMoveY < -3)
				{
					opgl->RotateView(ROTATE_ANGLE, 0);
				}
			}
		}
		update();
	}
}
void MyQT::DirTackBack()
{
	if (opgl->All_VecDir.size()!=0)
	{
		m_VecDir = VECTOR3D(0, 0, 0) - opgl->All_VecDir[opgl->nBlock];
		opgl->All_VecDir[opgl->nBlock] = m_VecDir;
		opgl->m_Dirback = true;
	}
	else
	{
		m_VecDir = VECTOR3D(0, 0, 0) - m_VecDir;
	}
	QString  Str = QString::number(m_VecDir.dx, 'f', 3) + "," + QString::number(m_VecDir.dy, 'f', 3) + "," + QString::number(m_VecDir.dz, 'f', 3);
	ui->sliceEdit->setText(Str);
	opgl->m_VecDir = m_VecDir;
	if (opgl->m_bOptiDir == true)
	{
		COptDir OptDir;
		OptDir.VisualResult(opgl->pSTL->m_vecFacetNorm, m_VecDir);
		opgl->m_vecFacetNum = OptDir.m_vecPFacetTri;
		opgl->pSTL->GetBox();
		opgl->updateGL();
	}
}
void MyQT::ChangeDir()
{
	if (opgl->pSTL != 0)
	{
		QString  tempText = QString::number(m_VecDir.dx, 'f', 3) + "," + QString::number(m_VecDir.dy, 'f', 3) + "," + QString::number(m_VecDir.dz, 'f', 3);;
		VECTOR3D  v3D;
		QByteArray by = ui->sliceEdit->text().toLocal8Bit();
		const char * MyDir = by.data();

		if (sscanf(MyDir, "%lf%*c%lf%*c%lf", &(v3D.dx), &(v3D.dy), &(v3D.dz)) != 3)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您输入的数据有误, 请检查是否输入了中文符号等问题?"));
			ui->sliceEdit->setText(tempText);
			return;
		}
		m_VecDir = v3D;
		m_VecDir.Normalize();
		opgl->m_VecDir = m_VecDir;
		
	}
	if (opgl->m_bOptiDir == true)
	{
		ShowOpti();
	}
}
void MyQT::ShowOpti()
{
	if (opgl->All_vecFacetNum.size() != 0)
	{
		opgl->m_Beoptir = false;
		opgl->m_bOptiDir = true;
		opgl->m_IfshowDir = !opgl->m_IfshowDir;
		

		if (opgl->nShowBdir<opgl->All_vecFacetNum.size()&& opgl->m_IfshowDir)
		{
			opgl->nShowBdir++;
		}
		if (opgl->nShowBdir == opgl->All_vecFacetNum.size()&& opgl->m_IfshowDir)
		{
			opgl->nShowBdir = 0;
		}
		opgl->updateGL();
		QString  Str = QString::number(opgl->All_VecDir[opgl->nShowBdir].dx, 'f', 3) + "," + QString::number(opgl->All_VecDir[opgl->nShowBdir].dy, 'f', 3) + "," + QString::number(opgl->All_VecDir[opgl->nShowBdir].dz, 'f', 3);
		ui->sliceEdit->setText(Str);
	}
	else
	{
		opgl->m_bOptiDir = !opgl->m_bOptiDir;
		if (opgl->m_bOptiDir)
		{
			ui->ButtonShowOpti->setChecked(true);
			if (opgl->pSTL != 0)
			{
				COptDir OptDir;
				OptDir.VisualResult(opgl->pSTL->m_vecFacetNorm, m_VecDir);
				opgl->m_vecFacetNum = OptDir.m_vecPFacetTri;
				opgl->pSTL->GetBox();
				opgl->updateGL();
			}
		}
		else
		{
			opgl->m_bIfShowSTL = true;
			ui->ButtonShowOpti->setChecked(false);
			opgl->updateGL();
		}
	}
}
void MyQT::OptiDir()
{
	if (opgl->pSTL == 0)
		return;
	if (opgl->All_VecDir.size() != 0)  return;
	if (opgl->m_VecDir.dx != 0 && opgl->m_VecDir.dy != 0 && opgl->m_VecDir.dz != 0) return;

	if (opgl->Rbox.size()!=0)//分割模型方向优化
	{
		if (opgl->All_vecFacetNum.size() != 0)
		{
			return;
		}
		vector<UNIT>  vec_Unit;
		opgl->m_Block = false;
		opgl->m_Beoptir = true;
		vec_PVECTOR3D m_vecPFacetNorm;
		for (int i = 0; i < opgl->Rbox.size(); i++)
		{
			COptDir OptDir;
			ui->SliceProgressBar->setRange(0, 199);
			ui->SliceProgressBar->show();
			ui->label_AutoDir->setText(QString::fromLocal8Bit("正在处理……"));
			vec_Unit.clear();
			vector<UNIT>  vec_UnitTemp;
			vec_UnitTemp = OptDir.MOPSO(opgl->Rbox[i].OneBlock, ui->SliceProgressBar);

			if (i != 0)
			{
				opgl->m_bOptiDir = !opgl->m_bOptiDir;
			}
			/////用于查找重复////////////////
			for (int j = 0; j < vec_UnitTemp.size(); j++)
			{
				if (j == 0)
				{
					vec_Unit.push_back(vec_UnitTemp[j]);
				}
				else
				{
					if (vec_UnitTemp[j].f1Sum != vec_Unit.back().f1Sum || vec_UnitTemp[j].f2Sum != vec_Unit.back().f2Sum)
					{
						vec_Unit.push_back(vec_UnitTemp[j]);
					}
				}
			}
			DlgOpti   dlg;
			vector<QString> vec_str;
			for (int j = 0; j < vec_Unit.size(); j++)
			{
				QString str1;
				QString str2;
				QString str;
				str1 = QString::number(vec_Unit[j].f1Sum, 'f', 8);
				str2 = QString::number(vec_Unit[j].f2Sum, 'f', 8);
				str = QString::fromLocal8Bit("无效体积") + str1 + QString::fromLocal8Bit("支撑面积") + str2;
				vec_str.push_back(str);
			}
			dlg.m_vec_str = vec_str;
			dlg.getStr();
			ui->label_AutoDir->setText(QString::fromLocal8Bit("处理完毕！"));
			if (dlg.exec() == QDialog::Accepted)
			{
				VECTOR3D v3D;
				VECTOR2D v2D;
				int index = dlg.index;
				v2D = vec_Unit[index].Dir;
				v3D.dx = sin(v2D.dy)*cos(v2D.dx);
				v3D.dy = sin(v2D.dy)*sin(v2D.dx);
				v3D.dz = cos(v2D.dy);
				m_VecDir = v3D;
				m_VecDir.Normalize();
				opgl->All_VecDir.push_back(m_VecDir);
				opgl->m_VecDir = m_VecDir;
				QString  Str = QString::number(v3D.dx, 'f', 3) + "," + QString::number(v3D.dy, 'f', 3) + "," + QString::number(v3D.dz, 'f', 3);
				ui->sliceEdit->setText(Str);
				opgl->m_bOptiDir = !opgl->m_bOptiDir;
				opgl->nBlock = i;

				m_vecPFacetNorm.clear();
				for (int j = 0; j < opgl->Rbox[i].OneBlock.size(); j++)
				{
					m_vecPFacetNorm.push_back(opgl->Rbox[i].OneBlock[j]->m_PFacetNorm);
				}
				OptDir.VisualResult(m_vecPFacetNorm, m_VecDir);
				opgl->m_vecFacetNum = OptDir.m_vecPFacetTri;
				opgl->All_vecFacetNum.push_back(opgl->m_vecFacetNum);
				opgl->updateGL();
			}
			ui->label_AutoDir->setText(QString::fromLocal8Bit(""));
			ui->SliceProgressBar->hide();
		}
	}
	else//整体进行方向优化
	{
		COptDir OptDir;
		ui->SliceProgressBar->setRange(0, 199);
		ui->SliceProgressBar->show();
		ui->label_AutoDir->setText(QString::fromLocal8Bit("正在处理……"));
		vector<UNIT>  vec_Unit = OptDir.MOPSO(opgl->pSTL, ui->SliceProgressBar);
		DlgOpti   dlg;
		vector<QString> vec_str;
		vec_PVECTOR3D m_vecPFacetNorm;
		for (int i = 0; i < vec_Unit.size(); i++)
		{
			QString str1;
			QString str2;
			QString str;
			str1 = QString::number(vec_Unit[i].f1Sum, 'f', 8);
			str2 = QString::number(vec_Unit[i].f2Sum, 'f', 8);
			str = QString::fromLocal8Bit("无效体积") + str1 + QString::fromLocal8Bit("支撑面积") + str2;
			vec_str.push_back(str);
		}
		dlg.m_vec_str = vec_str;
		dlg.getStr();
		ui->label_AutoDir->setText(QString::fromLocal8Bit("处理完毕！"));
		if (dlg.exec() == QDialog::Accepted)
		{
			VECTOR3D v3D;
			VECTOR2D v2D;
			int index = dlg.index;
			v2D = vec_Unit[index].Dir;
			v3D.dx = sin(v2D.dy)*cos(v2D.dx);
			v3D.dy = sin(v2D.dy)*sin(v2D.dx);
			v3D.dz = cos(v2D.dy);
			m_VecDir = v3D;
			m_VecDir.Normalize();
			opgl->m_VecDir = m_VecDir;
			m_vecPFacetNorm.clear();
			for (int i = 0; i < opgl->pSTL->m_vecPFacetTri.size(); i++)
			{
				m_vecPFacetNorm.push_back(opgl->pSTL->m_vecPFacetTri[i]->m_PFacetNorm);
			}
			OptDir.VisualResult(m_vecPFacetNorm, m_VecDir);
			opgl->m_vecFacetNum = OptDir.m_vecPFacetTri;
			QString  Str = QString::number(v3D.dx, 'f', 3) + "," + QString::number(v3D.dy, 'f', 3) + "," + QString::number(v3D.dz, 'f', 3);
			ui->sliceEdit->setText(Str);
		}
		opgl->m_bOptiDir = true;
		ui->label_AutoDir->setText(QString::fromLocal8Bit(""));
		ui->SliceProgressBar->hide();
	}
}
void MyQT::BlockSQ()
{
	if (opgl->Rbox.size() == 0) return;
	COptisequence PseQ;
	ui->OptiSeQProgressBar->setRange(0, 6);
	ui->OptiSeQProgressBar->show();
	opgl->m_optiSeQ = true;
	DlgInterveResult PInterve;

	vector<SUNIT>  vec_Unit = PseQ.MOPSO(opgl->Rbox, opgl->All_VecDir,PInterve, ui->OptiSeQProgressBar);
	CDlgOptisequence dlg;

	vector<QString> vec_str;
	for (int i = 0; i < vec_Unit.size(); i++)
	{
		QString str1;
		QString str2;
		QString str;
		str1 = QString::number(vec_Unit[i].f1Sum, 'f', 8);
		str2 = QString::number(vec_Unit[i].f2Sum, 'f', 8);
		str = QString::fromLocal8Bit("The total Swing-Angle:") + str1 + QString::fromLocal8Bit("，") + QString::fromLocal8Bit("The total Moving-Distance:") + str2;
		vec_str.push_back(str);
	}
	dlg.m_vec_str = vec_str;
	dlg.getStr();
	if (dlg.exec() == QDialog::Accepted)
	{
		vector<int> Sequence;
		int index = dlg.index;
		Sequence = vec_Unit[index].Sequence;
		PseQ.SaveOptiResult(opgl->Rbox,opgl->All_VecDir, Sequence);
		opgl->m_optiSeQ = true;//显示开关
		ui->OptiSeQProgressBar->hide();
	}
}
void MyQT::ShowSeQ()
{
	opgl->m_optiSeQ = !opgl->m_optiSeQ;
}
void MyQT::ShowHeadbox()
{
	opgl->m_Headbox = !opgl->m_Headbox;
	if (opgl->Rbox.size() != 0 )
	{
		COptisequence Pseq;

		VECTOR3D DirZ = VECTOR3D(0, 0, 1);
		double Angle = DirZ | opgl->All_VecDir[0];
		if (fabs(Angle) >AuxZero)
		{
			opgl->All_VecDir[0] = DirZ;
			/*for (int i = 0; i < opgl->Rbox.size(); i++)
			{
			Pseq.RotateBlock(opgl->Rbox[i], opgl->All_VecDir[0], DirZ);
			}

			Pseq.RotateDir(opgl->All_VecDir, opgl->All_VecDir[0], DirZ);*/
		}
		opgl->vec_HeadBox.clear();
		POINT3D pceter,pcentTemp;
		pceter = opgl->CalCenterPoint(opgl->Rbox[opgl->nBlock], opgl->All_VecDir[opgl->nBlock]);
		opgl->CreatBoxcir(pceter, opgl->All_VecDir[opgl->nBlock]);
		
		if (opgl->vec_PlanBoard.size()== 0)
		{
			pcentTemp = opgl->CalCenterPoint(opgl->Rbox[0], opgl->All_VecDir[0]);
			Pseq.Creatboarddate(opgl->vec_PlanBoard, pcentTemp);
		}
	}
}
void MyQT::Ceshi()
{
	opgl->m_Dirback = true;
	vector<MyBlock> RboxTemp;

	MyBlock block0,block1;
	block0.OneBlock = opgl->Rbox[1].OneBlock;
	block0.OneBlockPair = opgl->Rbox[1].OneBlockPair;

	block1.OneBlock = opgl->Rbox[0].OneBlock;
	block1.OneBlockPair = opgl->Rbox[0].OneBlockPair;

	
	for (int i = 0; i < opgl->Rbox[0].vec_PairBlock.size(); i++)
	{
		if (opgl->Rbox[0].vec_PairBlock[i] == 1)
		{
			opgl->Rbox[0].vec_PairBlock[i] = 0;
		}
	}
	block1.vec_PairBlock = opgl->Rbox[0].vec_PairBlock;
	opgl->Rbox[1].vec_PairBlock[0] = 1;
	block0.vec_PairBlock = opgl->Rbox[1].vec_PairBlock;
	RboxTemp.push_back(block0); RboxTemp.push_back(block1);

	for (int i = 2; i < opgl->Rbox.size(); i++)
	{
		RboxTemp.push_back(opgl->Rbox[i]);
	}
	opgl->Rbox.clear();
	for (int i = 0; i < RboxTemp.size(); i++)
	{
		opgl->Rbox.push_back(RboxTemp[i]);
	}
	RboxTemp.clear();
	//最优方向调换一下
	vector<VECTOR3D> Vec_DirTemp;
	Vec_DirTemp.push_back(opgl->All_VecDir[1]); Vec_DirTemp.push_back(opgl->All_VecDir[0]);
	for (int i = 2; i < opgl->All_VecDir.size(); i++)
	{
		Vec_DirTemp.push_back(opgl->All_VecDir[i]);
	}
	opgl->All_VecDir.clear();
	for (int i = 0; i < Vec_DirTemp.size(); i++)
	{
		opgl->All_VecDir.push_back(Vec_DirTemp[i]);
	}
	Vec_DirTemp.clear();

	//opgl->ceshi = !opgl->ceshi;
	//COptisequence Pseq;
	//POINT3D pv = POINT3D(0, 0, 0);
	//VECTOR3D DirZ = VECTOR3D(0, 0, 1); VECTOR3D DirY = VECTOR3D(0, 1, 0);
	//if (opgl->vec_Ceshi0.size() == 0 || opgl->vec_Ceshi1.size() == 0)
	//{
	//	Pseq.CreatCeshidate(opgl->vec_Ceshi0, pv, DirZ);
	//	Pseq.CreatCeshidate(opgl->vec_Ceshi1, pv, DirY);
	//}
	//SInterB CurrentModel;
	//CurrentModel.ModelDate = opgl->vec_Ceshi0; CurrentModel.BoxDate = opgl->vec_Ceshi0;
	//vector<SInterB> AllModeDate; AllModeDate.push_back(CurrentModel);
	//bool rete;//rate为true代表有相交
	//if (opgl->Rbox.size()!=0)
	//{
	//	int der=0;
	//	rete = Pseq.GetIntereResult(AllModeDate, opgl->Rbox[0], opgl->vec_Ceshi1, pv, DirZ,der);
	//}
	//int dwr = 0;
}
void MyQT::DetectBlock()
{
	if (opgl->All_VecDir.size() == 0) return;
	bool ifMb = true;
	for (int i = 0; i < opgl->Rbox.size(); i++)
	{
		Blockpath pMeshTri;
		//(i == 0) ? ifMb = true : ifMb = false;
		opgl->pslice->CreateEdgeTopByFctVert(pMeshTri, opgl->Rbox[i].OneBlock);
		opgl->pslice->FindBlockHole(pMeshTri, opgl->Rbox[i].OneBlock, opgl->All_VecDir[i], ifMb);
		opgl->Rbox[i].OneBlock = pMeshTri.m_vecPFacetTri;
		vec_PHEDGE BordRing;//存储所有的边界边，查找边界边的一阶邻域面片
		for (int j = 0; j < pMeshTri.m_vecPEdge.size(); j++)
		{
			PHEDGE Ph = pMeshTri.m_vecPEdge[j]->pHEdgeAdj;
			if (!Ph->pHEdgePair)
			{
				BordRing.push_back(Ph);
			}
		}
		VECTOR3D Diz=VECTOR3D(0,0,0);
		if (BordRing.size() != 0)
		{
			PPOLYPOLYGON vec_poly; vec_poly = new POLYPOLYGON;
			KitTool::closedBoundary(BordRing, vec_poly);//给所有的边界顶点排序
			opgl->BlockEdge.push_back(vec_poly);
			BordRing.clear();

			//调试占用
			vec_POINT3D ploy=vec_poly->m_vecPPolygons[0]->m_vecPnts;
			VECTOR3D v0, v1;
			v0 = ploy[(int)ploy.size()*0.5] - ploy[0];
			v1 = ploy[ploy.size()-1] - ploy[0];
			v1 = v0*v1; v1.Normalize();
			double angel = opgl->All_VecDir[i] | Diz;
			angel = abs(abs(angel) - 1);
			int OneZero = 0.000004;
			if (angel > OneZero)
			{
				int ceshi = 0;
			}

		}

	}
}
void MyQT::BlockSwitch()
{
	if (opgl->All_VecDir.size() != 0)
	{
		opgl->m_SwichBlock = !opgl->m_SwichBlock;
		opgl->Sblock = !opgl->m_SwichBlock;
		if (opgl->nBlock < opgl->All_VecDir.size() && opgl->m_SwichBlock)
		{
			opgl->m_Block = false;
			opgl->nBlock++;
		}
		if (opgl->nBlock == opgl->All_VecDir.size() && opgl->m_SwichBlock)
		{
			opgl->nBlock = 0;
			opgl->m_Block = false;
		}
		opgl->m_Beoptir = false;
		opgl->m_IfshowDir = true;
		opgl->nShowBdir = opgl->nBlock;
		opgl->updateGL();
	}
}
void MyQT::Slicing()
{
	opgl->m_Alltractory = true;
	opgl->m_slice = !opgl->m_slice;
	if (opgl->m_slice)
	{
		ui->SliceRoadbar->show();
		opgl->m_optiSeQ = false;
		if (opgl->All_VecDir.size() != 0)//分块后分层
		{
			for (int i = 0; i < opgl->All_VecDir.size();i++)
			{
				opgl->All_VecDir[i].Normalize();
			}
			opgl->pslice->GetBlockEvenslice(opgl->Rbox, opgl->Blocklunkuo, opgl->All_VecDir, m_equdept, ui->SliceRoadbar);
			
		}
		else
		{
			if (opgl->lunkuo.size() == 0)
			{
				opgl->pslice->GetEvenslice(opgl->pSTL, opgl->lunkuo, m_VecDir, m_equdept, ui->SliceRoadbar);
				opgl->m_bIfShowSTL = false;
				ui->ButtonSlice->setChecked(true);
			}
		}
		ui->SliceRoadbar->hide();
	}
	else
	{
		opgl->m_bIfShowSTL = true;
		ui->ButtonSlice->setChecked(false);
	}
}
void MyQT::Shadeslice()
{
	opgl->m_slice = !opgl->m_slice;

}
void MyQT::OnFill()
{
	opgl->m_Alltractory = true;
	ui->SliceRoadbar->show();
	if (opgl->BlockFill.size()!=0||opgl->Fill.size()!=0)
	{
		opgl->m_Fill = !opgl->m_Fill;
		opgl->m_Block = !opgl->m_Block;
	}
	else
	{
		int i = ui->FillcomboBox->currentIndex();
		if (opgl->m_Block)
		{
			opgl->pslice->GetBlockFillResult(opgl->Blocklunkuo, opgl->All_VecDir,opgl->BlockFill, ui->SliceRoadbar, Diamer, i);

			opgl->dlg.FindTheLow(opgl->BlockFill[0], opgl->PMainlow);
			opgl->pSEQ.TranRbox(opgl->BlockFill, opgl->PMainlow);
		}
		else
		{
			
			opgl->pslice->GetFillResult(opgl->lunkuo, m_VecDir, opgl->Fill, ui->SliceRoadbar, Diamer, i);
		}
		opgl->m_Fill = true; opgl->m_slice = false; 
		opgl->m_Block = false; opgl->Sblock = false;
	}
	ui->SliceRoadbar->hide();
}
void MyQT::SeqLayerShow()
{
	//QTimer *timer;
	//timer = new QTimer(this);
	//timer->start(1000);
	//connect(timer, SIGNAL(timeout()), this, SLOT(SeqLayerShow()));
	opgl->m_Alltractory = false;
	opgl->m_SliceSeqShow = !opgl->m_SliceSeqShow;
	if (opgl->Blocklunkuo.size() == 0) return;
	if (opgl->BlockFill.size()!=0)
	{
		if (opgl->SeqFill.size() == 0)
		{
			for (int i = 0; i < opgl->BlockFill.size();i++)
			{
				for (int j = 0; j < opgl->BlockFill[i].size(); j++)
				{
					opgl->SeqFill.push_back(opgl->BlockFill[i][j]);
				}
			}
		}
		if (opgl->SeqlunkuoLayer.size() == 0)
		{
			for (int i = 0; i < opgl->Blocklunkuo.size(); i++)
			{
				for (int j = 0; j < opgl->Blocklunkuo[i].size(); j++)
				{
					opgl->SeqlunkuoLayer.push_back(opgl->Blocklunkuo[i][j]);
				}
			}
		}
	}
	else
	{
		if (opgl->SeqlunkuoLayer.size() == 0)
		{
			for (int i = 0; i < opgl->Blocklunkuo.size(); i++)
			{
				for (int j = 0; j < opgl->Blocklunkuo[i].size(); j++)
				{
					opgl->SeqlunkuoLayer.push_back(opgl->Blocklunkuo[i][j]);
				}
			}
		}
	}
 }
void MyQT::saveFillResult()
{
	MeshSaveDlg STLSave;
	if (opgl->BlockFill.size() != 0)
	{
		int res = STLSave.exec();
		QString filepath = "..\\Result\\BlockModelFillresult.stl";
		if (filepath == NULL)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("不能保存文件!"));
		}
		opgl->PSimple->Write_BlockFillResult(opgl->BlockFill, opgl->Blocklunkuo, opgl->All_VecDir, filepath);
		if (res == QDialog::Accepted)
		{
			QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
		}
	}
	else
	{
		if (opgl->Fill.size()!=0)
		{
			int res = STLSave.exec();
			QString filepath = "..\\Result\\ModelFillresult.stl";
			if (filepath == NULL)
			{
				QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("不能保存文件!"));
			}
			VECTOR3D DirZ = VECTOR3D(0, 0, 1);
			opgl->PSimple->Write_FillResult(opgl->Fill, opgl->lunkuo, DirZ, filepath);
			if (res == QDialog::Accepted)
			{
				QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
			}
		}
	}
}
void MyQT::SimlastBlock()
{
	if (opgl->Rbox.size() == 0) return;
	DlgBlockSeq dlg;
	if (opgl->BlockFill.size() != 0)//仿真成型轨迹
	{
		
		dlg.LastBlockPath(opgl->Nublcok, opgl->ToolpathFill);
		opgl->Nublcok--;
	}
	else//子块位姿仿真
	{
		dlg.LastBlock(opgl->Nublcok, opgl->SimALLBlock);
		opgl->Nublcok--;
	}
}
void MyQT::SimNextBlock()
{
	if (opgl->Rbox.size() == 0) return;
	opgl->m_board = true;
	DlgBlockSeq dlg;
	if (opgl->Nublcok == opgl->BlockFill.size())    opgl->Nublcok = 0;
	if (opgl->BlockFill.size() != 0)//仿真成型轨迹
	{
		dlg.FindTheLow(opgl->BlockFill[opgl->Nublcok], opgl->Plow);
		if (opgl->Nublcok == 0)  opgl->PMainlow = opgl->Plow;

		if (opgl->vec_PlatRotate.size() != 0)   opgl->vec_PlatRotate.clear();
		if (opgl->p_RowRotate.size() != 0)   opgl->p_RowRotate.clear();
		dlg.UpdateBoarddate(opgl->All_VecDir, opgl->vec_Plat, opgl->vec_PlatRotate, opgl->p_Row, opgl->p_RowRotate, opgl->Plow, opgl->Nublcok);
		
		dlg.NextBlockPath(opgl->BlockFill, opgl->All_VecDir, opgl->Plow, opgl->Nublcok, opgl->ToolpathFill);
	}
	else//子块位姿仿真
	{
		if (opgl->Nublcok == 0)
		{
			opgl->ALLBlock.clear();
			for (int i = 0; i < opgl->Rbox.size();i++)
			{
				opgl->ALLBlock.push_back(opgl->Rbox[i].OneBlock);
			}
			dlg.FindTheLow(opgl->ALLBlock[0], opgl->Plow);
		}
		dlg.NextBlock(opgl->ALLBlock, opgl->All_VecDir, opgl->Plow, opgl->Nublcok, opgl->SimALLBlock);
	}
	(opgl->Nublcok != opgl->BlockFill.size()) ? opgl->Nublcok++ : opgl->Nublcok=0;
	opgl->m_board = false;
}
void MyQT::SimlateResult()
{
	if (opgl->BlockFill.size() == 0) return;
	DlgBlockSeq dlg;
	dlg.Getallblcokpath(opgl->BlockFill,opgl->All_VecDir, opgl->ToolpathFill);

	/*COptisequence PseQ;
	DlgBlockSeq dlg;
	dlg.inial(opgl);
	dlg.exec();*/
}
void MyQT::Linput(QString)
{
	QString text = ui->m_equlineEdit->text();
	m_equdept = text.toFloat();
	QString text1 = ui->m_minlineEdit->text();
	m_mindept = text1.toFloat();
	QString text2 = ui->m_Wdiamt->text();//丝材直径
	Diamer = text2.toFloat();
	if (m_mindept > m_equdept || m_mindept < 0 || m_equdept<0 || Diamer<0)
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您输入的数据有误,重新输入!"));
		return;
	}
	opgl->m_Fill = false;
	opgl->m_slice=false;
	opgl->NCurrentlayer = ui->BoxLayer->value();
}
void MyQT::Laplacian()
{
	m_laplacian = 0;
}
void MyQT::Laplacianback()
{
	m_laplacian = 1;
}
void MyQT::LaplacianFeature()
{
	m_laplacian = 2;
}

void MyQT::MeshZoom()
{
	if (opgl->pSTL != 0)
	{
		ZoomDlg mydialog;
		mydialog.exec();
		//opgl->pMeshZoom = STLModell::GetInstance();
		opgl->pMeshZoom = new STLModell;
		opgl->pSTL->Zooommesh(opgl->pSTL, opgl->pMeshZoom, mydialog.rate_x, mydialog.rate_y, mydialog.rate_z);
		QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型已缩放，请保存模型!"));
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("请加载模型后重试!"));
	}
}
void MyQT::MeshSave()
{
	if (!opgl->pSTL) return;
	MeshSaveDlg STLSave;
	if (opgl->m_bIfShowSTL || opgl->m_IFSimple )
	{
		int res = STLSave.exec();
		if (STLSave.saveways)//保存为二进制格式
		{
			if (opgl->m_bIfShowSTL&&opgl->pMeshZoom != 0)
			{
				FILE* fileTemp = fopen("..\\Result\\meshzoomresult.stl", "wb");
				if (fileTemp == NULL)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("不能保存文件!"));
				}
				opgl->pMeshZoom->write_stl(opgl->pMeshZoom, fileTemp);
				fclose(fileTemp);
				fileTemp = NULL;
				if (res == QDialog::Accepted)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
				}
			}
			if (opgl->m_IFSimple&&opgl->PSimple != 0)//保存为文本格式
			{
				FILE* fileTemp = fopen("..\\Result\\meshsimplyresult.stl", "wb");
				if (fileTemp == NULL)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("不能保存文件!"));
				}
				opgl->PSimple->write_stl(opgl->PSimple, fileTemp);
				fclose(fileTemp);
				fileTemp = NULL;
				if (res == QDialog::Accepted)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
				}
			}
		}
		else
		{
			if (opgl->m_bIfShowSTL&&opgl->pMeshZoom != 0)
			{
				QString filepath = "..\\Result\\meshzoomresult.stl";
				opgl->pMeshZoom->Write_STL(opgl->pMeshZoom, filepath);
				if (res == QDialog::Accepted)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
				}
			}
			if (opgl->m_IFSimple&&opgl->PSimple != 0)
			{
				QString filepath = "..\\Result\\meshsimplyresult.stl";
				opgl->PSimple->Write_STL(opgl->PSimple, filepath);
				if (res == QDialog::Accepted)
				{
					QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
				}
			}
		}
	}
	else
	{
		QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请重试!"));
	}
}
void MyQT::BlockSave()
{
	MeshSaveDlg STLSave;
	if (opgl->Rbox.size() != 0)
	{
		int res = STLSave.exec();
		if ((opgl->m_Block&&opgl->Rbox.size() != 0) || opgl->m_Dirback)
		{
			QString filepath = "..\\Result\\meshSEGresult.stl";
			if (filepath == NULL)
			{
				QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("不能保存文件!"));
			}
			for (int i = 0; i < opgl->All_VecDir.size(); i++)//单位化求得的法矢
			{
				opgl->All_VecDir[i].Normalize();
			}
			opgl->PSimple->Write_STLSEG(opgl->Rbox,opgl->All_VecDir, filepath);
			if (res == QDialog::Accepted)
			{
				QMessageBox::information(this, QString::fromLocal8Bit("提示"), QStringLiteral("模型数据已保存!"));
			}
		}
	}
}
void MyQT::SimplifyGrid()
{
	if (!opgl->PSimple)
	{
		if (opgl->pSTL != 0)
		{
			opgl->PSimple = STLModell::GetInstance();
			//opgl->PSimple = new STLModell();
			ui->simplyprogressBar->show();
			QString  str = ui->m_rateEdit->text();
			rate = str.toFloat();
			QString  str1 = ui->m_ratepencet->text();
			ratepercent = str1.toFloat();
			ui->label_Auto->setText(QString::fromLocal8Bit("   正在处理……"));
			//double time_Start = (double)clock();
			opgl->Pmeshblock.pSimply = new CSimplify;

			int m_Selectsimply = ui->SimpcomboBox->currentIndex();
			opgl->Pmeshblock.pSimply->GetsimpleResult(opgl->pSTL, opgl->PSimple, m_Selectsimply, rate, ratepercent, ui->simplyprogressBar);
			//double time_End = (double)clock();
			//simpletime = (time_End - time_Start) / 1000.0;
			simpletime = opgl->Pmeshblock.pSimply->TriangleTime;
			opgl->initVBO(opgl->PSimple);
			Displayparameter(0);
			ui->label_Auto->setText(QString::fromLocal8Bit("   处理完成，恭喜！"));
			opgl->m_IFSimple = true;
			opgl->m_bIfShowSTL = false;//关闭原模型显示
			ui->simplyprogressBar->hide();
		}
		else
		{
			QMessageBox::information(this, QString::fromLocal8Bit("警告"), QStringLiteral("您的操作有误,请检查后重试!"));
		}
	}
	else
	{
		opgl->m_IFSimple = !opgl->m_IFSimple; opgl->m_bIfShowSTL = !opgl->m_bIfShowSTL;
		opgl->updateGL();
	}
}
void MyQT::Calmeshdegree()
{
	if (!opgl->pSTL) return;
	double Mdegeree1 = 0, Mdegeree2 = 0;
	if (!opgl->m_IFSimple)
	{
		opgl->Pmeshblock.pSimply->EvaluatemeshQuality(opgl->pSTL, Mdegeree1);
		QString Meshdegree = QString::number(Mdegeree1);
		ui->meshdegree->setText(Meshdegree);
	}
	else
	{
		opgl->Pmeshblock.pSimply->EvaluatemeshQuality(opgl->PSimple, Mdegeree2);
		QString Meshdegree = QString::number(Mdegeree2);
		ui->meshdegree1->setText(Meshdegree);
	}

}