
#include "ShowShader.h"
#include <glut.h>
//#include <glaux.h>
//#include <glew.h>
using namespace ViewSpace;
CShowShader::CShowShader()
{
}
CShowShader::~CShowShader()
{
}
void CShowShader::SetMaterialColor(COLORREF clr)                   //设置材质
{
	//m_clrMaterial = clr;    //将材料颜色保存在成员变量m_clrMaterial中
	BYTE r, g, b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);

	//将以上颜色设置为当前的材质属性
	GLfloat mat_amb_diff[] = { (GLfloat)r / 255, (GLfloat)g / 255, (GLfloat)b / 255, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
}
void CShowShader::ShaderWireFrame(STLModell*Tmesh)
{      
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0f,0.0f,1.0f);

	glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	int fn=Tmesh->m_vecPFacetTri.size();
	vector<unsigned int>FaceIndexArray_All(fn*3);
	for (int nVer = 0; nVer < Tmesh->m_vecPVert.size(); nVer++)
	{
		Tmesh->m_vecPVert[nVer]->VertexNormal = KitTool::CalcuVerNormal(Tmesh->m_vecPVert[nVer]);
	}
	for (int i = 0; i < fn; i++)
	{
		FaceIndexArray_All[3 * i] = Tmesh->m_vecPFacetTri[i]->m_PVerts[0]->VertexID;
		FaceIndexArray_All[3 * i + 1] = Tmesh->m_vecPFacetTri[i]->m_PVerts[1]->VertexID;
		FaceIndexArray_All[3 * i + 2] = Tmesh->m_vecPFacetTri[i]->m_PVerts[2]->VertexID;
	}
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glVertexPointer(3, GL_FLOAT, 0, &Tmesh->m_vecPVert[0]->x);
	glNormalPointer(GL_FLOAT, 0, &Tmesh->m_vecPVert[0]->VertexNormal.dx);
	glDrawElements(GL_TRIANGLES, fn * 3, GL_UNSIGNED_INT, &FaceIndexArray_All[0]);
	glDisable(GL_COLOR_MATERIAL);
}
void CShowShader::HideWireFrame(STLModell*Tmesh/*, vector<vec> vec_nomal, vector<point> vec_point*/)
{
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	glColor3f(0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	vector<vec> vec_nomal; vector<point> vec_point;
	for (int nVer = 0; nVer < Tmesh->m_vecPVert.size(); nVer++)
	{
		Tmesh->m_vecPVert[nVer]->VertexNormal = KitTool::CalcuVerNormal(Tmesh->m_vecPVert[nVer]);
		vec vre = vec(Tmesh->m_vecPVert[nVer]->VertexNormal.dx, Tmesh->m_vecPVert[nVer]->VertexNormal.dy, Tmesh->m_vecPVert[nVer]->VertexNormal.dz);
		vec_nomal.push_back(vre);
		point poit = point(Tmesh->m_vecPVert[nVer]->x, Tmesh->m_vecPVert[nVer]->y, Tmesh->m_vecPVert[nVer]->z);
		vec_point.push_back(poit);
	}
	int fn = Tmesh->m_vecPFacetTri.size();
	vector<unsigned int>FaceIndexArray_All(fn * 3);
	for (int i = 0; i < fn; i++)
	{
		FaceIndexArray_All[3 * i] = Tmesh->m_vecPFacetTri[i]->m_PVerts[0]->VertexID;
		FaceIndexArray_All[3 * i + 1] = Tmesh->m_vecPFacetTri[i]->m_PVerts[1]->VertexID;
		FaceIndexArray_All[3 * i + 2] = Tmesh->m_vecPFacetTri[i]->m_PVerts[2]->VertexID;
	}
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glVertexPointer(3, GL_FLOAT, 0, &vec_point[0][0]);
	glNormalPointer(GL_FLOAT, 0, &vec_nomal[0][0]);
	glDrawElements(GL_TRIANGLES, fn * 3, GL_UNSIGNED_INT, &FaceIndexArray_All[0]);
	glDisable(GL_COLOR_MATERIAL);
}

//下面函数之所以这样复杂，是为了速度考虑，尽量避免调用opengl绘制函数可以大大减小绘制时间
//把选中与未被选中的模型分开画，这样可以减少调用材质设置函数
void CShowShader::SmoothShaderModel(STLModell*Tmesh)
{
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	vector<vec> vec_nomal; vector<point> vec_point;
	for (int nVer = 0; nVer < Tmesh->m_vecPVert.size(); nVer++)
	{
		Tmesh->m_vecPVert[nVer]->VertexNormal = KitTool::CalcuVerNormal(Tmesh->m_vecPVert[nVer]);
		vec vre = vec(Tmesh->m_vecPVert[nVer]->VertexNormal.dx, Tmesh->m_vecPVert[nVer]->VertexNormal.dy, Tmesh->m_vecPVert[nVer]->VertexNormal.dz);
		vec_nomal.push_back(vre);
		point poit = point(Tmesh->m_vecPVert[nVer]->x, Tmesh->m_vecPVert[nVer]->y, Tmesh->m_vecPVert[nVer]->z);
		vec_point.push_back(poit);
	}
	int fn=Tmesh->m_vecPFacetTri.size();
	glDisable(GL_COLOR_MATERIAL);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);//设置双面光照使物体的背面接受的光照准确
	glVertexPointer(3, GL_FLOAT, 0, &vec_point[0][0]);
	glNormalPointer(GL_FLOAT, 0, &vec_nomal[0][0]);
	vector<int>noselectF;
	vector<int>beselectF;
	noselectF.reserve(fn);
	beselectF.reserve(fn);
	for (int i=0;i<fn;i++)
	{
		if(Tmesh->m_vecPFacetTri[i]->beSelect) beselectF.push_back(i);
		else    noselectF.push_back(i);
	}

	int fn_be=beselectF.size();
	int fn_no=noselectF.size();
	vector<unsigned int> beselectF_Index(3*beselectF.size());
	vector<unsigned int> noselectF_Index(3*noselectF.size());
#pragma omp parallel for if(fn_be>1e3)
	for (int i=0;i<fn_be;i++)
	{   
		for (int j=0;j<3;j++)
		{
			beselectF_Index[3*i+j]=Tmesh->m_vecPFacetTri[beselectF[i]]->m_PVerts[j]->VertexID;
		}
	}
#pragma omp parallel for  
	for (int i=0;i<fn_no;i++)
	{   
		for (int j=0;j<3;j++)
		{
			noselectF_Index[3 * i + j] = Tmesh->m_vecPFacetTri[noselectF[i]]->m_PVerts[j]->VertexID;
		}
	}
	if (!noselectF_Index.empty())glDrawElements(GL_TRIANGLES,fn_no*3,GL_UNSIGNED_INT,&noselectF_Index[0]);


	if (!beselectF_Index.empty())
	{
		SetMaterialColor(RGB(255,20,147));//粉红
		glDrawElements(GL_TRIANGLES,fn_be*3,GL_UNSIGNED_INT,&beselectF_Index[0]);
	}
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

}
//下面函数之所以这样复杂，是为了速度考虑，尽量避免调用opengl绘制函数可以大大减小绘制时间
//把选中与未被选中的模型分开画，这样可以减少调用材质设置函数
void CShowShader::PlaneShaderModel(STLModell*Tmesh)
{
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	int fn=Tmesh->m_vecPFacetTri.size();
	for (int i=0;i<fn;i++)
	{
		if (Tmesh->m_vecPFacetTri[i]->beSelect)continue;
		glBegin(GL_TRIANGLES);
		glNormal3f(Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dx, Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dy, Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dz);
		FACETTRI pf0; pf0 = *Tmesh->m_vecPFacetTri[i];
		glVertex3f(pf0.m_PVerts[0]->x, pf0.m_PVerts[0]->y, pf0.m_PVerts[0]->z);
		glVertex3f(pf0.m_PVerts[1]->x, pf0.m_PVerts[1]->y, pf0.m_PVerts[1]->z);
		glVertex3f(pf0.m_PVerts[2]->x, pf0.m_PVerts[2]->y, pf0.m_PVerts[2]->z);
		glEnd();
	}

	bool flag=false;
	for (int i=0;i<fn;i++)
	{
		if (Tmesh->m_vecPFacetTri[i]->beSelect)
		{
			SetMaterialColor(RGB(255,20,147));
			flag=true;
			break;
		}		
	}
	if(flag)
	for (int i=0;i<fn;i++)
	{
		if (Tmesh->m_vecPFacetTri[i]->beSelect)
		{
			glBegin(GL_TRIANGLES);
			glNormal3f(Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dx, Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dy, Tmesh->m_vecPFacetTri[i]->m_PFacetNorm->dz);
			FACETTRI pf0; pf0 = *Tmesh->m_vecPFacetTri[i];
			glVertex3f(pf0.m_PVerts[0]->x, pf0.m_PVerts[0]->y, pf0.m_PVerts[0]->z);
			glVertex3f(pf0.m_PVerts[1]->x, pf0.m_PVerts[1]->y, pf0.m_PVerts[1]->z);
			glVertex3f(pf0.m_PVerts[2]->x, pf0.m_PVerts[2]->y, pf0.m_PVerts[2]->z);
			glEnd();
		}
	}
	
}
void CShowShader::ShaderMesh(STLModell*Tmesh, ShaderModel shadermodle0, vector<vec> vec_nomal, vector<point> vec_point, vector<vec> vec_facenomal)
{
	switch(shadermodle0)
	{
	case SMOOTH_SHADER:
		{
			SmoothShaderModel(Tmesh);
			break;
		}
	case WIRE_FRAME:
		{
			ShaderWireFrame(Tmesh);
			break;
		}
	case PLANE_SHADER:
		{
			PlaneShaderModel(Tmesh);
			break;
		}
	case HIDE_WIRE_FRAME:
		{
			HideWireFrame(Tmesh/*,vec_nomal,vec_point*/);
			break;
		}
	default:PlaneShaderModel(Tmesh);
	}

}




