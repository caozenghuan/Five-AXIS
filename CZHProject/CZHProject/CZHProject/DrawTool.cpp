#include "DrawTool.h"
drawTool::drawTool()
{
	m_clrBkMaterial = RGB(0, 0, 0);
}
drawTool::~drawTool()
{
}
///////////////////////////////////绘图函数/////////////////////////////////////////
void drawTool::SetMaterialColor(COLORREF clr)                   //设置材质
{
	m_clrMaterial = clr;    //将材料颜色保存在成员变量m_clrMaterial中
	BYTE r, g, b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);

	//将以上颜色设置为当前的材质属性
	GLfloat mat_amb_diff[] = { (GLfloat)r / 255, (GLfloat)g / 255, (GLfloat)b / 255, 1.0 };
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, mat_amb_diff);
}
void drawTool::SetBkMaterialColor(COLORREF clr)
{
	float mat[4];

	mat[0] = 0.5;
	mat[1] = 0.5;
	mat[2] = 0.5;
	mat[3] = 1.0;
	glMaterialfv(GL_BACK, GL_AMBIENT, mat);

	m_clrBkMaterial = clr;
	BYTE r, g, b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	//GLfloat mat_amb_diff[] = {(GLfloat)r/255,(GLfloat)g/255,(GLfloat)b/255,1.0};
	//glMaterialfv(GL_BACK,GL_AMBIENT_AND_DIFFUSE,mat_amb_diff);
	glMaterialfv(GL_BACK, GL_DIFFUSE, mat);

	glMaterialfv(GL_BACK, GL_SPECULAR, mat);
	glMaterialf(GL_BACK, GL_SHININESS, 0.25 * 128.0);
}
void drawTool::GetMaterialColor(COLORREF& clr)
{
	clr = m_clrMaterial;
}

void drawTool::SetColor(COLORREF clr)                  //设置非光照模式下的绘图颜色
{
	m_clr = clr;
	BYTE r, g, b;
	r = GetRValue(clr);
	g = GetGValue(clr);
	b = GetBValue(clr);
	glColor3ub(r, g, b);  //设置绘图颜色
}
void drawTool::GetColor(COLORREF& clr)
{
	clr = m_clr;
}
void drawTool::DrawPoint(const POINT3D& pt)   //绘制一个空间点，其大小在GLsetupRC定义
{
	glBegin(GL_POINTS);
	glVertex3f(pt.x, pt.y, pt.z);
	glEnd();
}

void drawTool::DrawPickPoint(const vector<POINT3D> &modelPOINT)
{
	if (modelPOINT.size() < 1) return;
	COLORREF old;
	GetColor(old);
	GetMaterialColor(old);
	SetMaterialColor(RGB(255, 0, 0));
	for (int i = 0; i < modelPOINT.size(); i++)
	{
		DrawSphere(modelPOINT[i], 0.8);
	}
	SetMaterialColor(old);
	
}
void drawTool::DrawLine(const POINT3D& sp, const POINT3D& ep)
{
	//glLineWidth(5.0f);
	glBegin(GL_LINES);
	glVertex3f(sp.x, sp.y, sp.z);
	glVertex3f(ep.x, ep.y, ep.z);
	glEnd();
}

void drawTool::DrawTriChip(double n0, double n1, double n2,
	double v00, double v01, double v02,
	double v10, double v11, double v12,
	double v20, double v21, double v22)
{
	glBegin(GL_TRIANGLES);
	glNormal3d(n0, n1, n2);
	glVertex3d(v00, v01, v02);
	glVertex3d(v10, v11, v12);
	glVertex3d(v20, v21, v22);
	glEnd();
}
void drawTool::DrawBox(BOX3D bot)
{
	COLORREF old;
	GetColor(old);
	GetMaterialColor(old);
	SetMaterialColor(RGB(20, 150, 50));
	vector<POINT3D> pointbottom;
	vector<POINT3D> pointup;
	pointbottom.push_back(POINT3D(bot.x0, bot.y0, bot.z0));
	pointbottom.push_back(POINT3D(bot.x1, bot.y0, bot.z0));
	pointbottom.push_back(POINT3D(bot.x1, bot.y1, bot.z0));
	pointbottom.push_back(POINT3D(bot.x0, bot.y1, bot.z0));

	pointup.push_back(POINT3D(bot.x0, bot.y0, bot.z1));
	pointup.push_back(POINT3D(bot.x1, bot.y0, bot.z1));
	pointup.push_back(POINT3D(bot.x1, bot.y1, bot.z1));
	pointup.push_back(POINT3D(bot.x0, bot.y1, bot.z1));
	for (int i = 0; i < pointbottom.size();i++)
	{
		i == 3 ? DrawLine(pointbottom[3], pointbottom[0]) : DrawLine(pointbottom[i], pointbottom[i + 1]);
	}
	for (int i = 0; i < pointup.size(); i++)
	{
		i == 3 ? DrawLine(pointup[3], pointup[0]) : DrawLine(pointup[i], pointup[i + 1]);
	}
	for (int i = 0; i < pointup.size(); i++)
	{
		DrawLine(pointbottom[i], pointup[i]);
	}
	SetMaterialColor(old);
}
void drawTool::DrawHeadBox(vector<vec_POINT3D> vec_face, COLORREF clr)
{
	COLORREF old_clr;
	GetMaterialColor(old_clr);
	SetMaterialColor(clr);
	for (int i = 0; i < vec_face.size(); i++)
	{
		DrawTriChip(vec_face[i][0].x, vec_face[i][0].y, vec_face[i][0].z,
			vec_face[i][1].x, vec_face[i][1].y, vec_face[i][1].z,
			vec_face[i][2].x, vec_face[i][2].y, vec_face[i][2].z,
			vec_face[i][3].x, vec_face[i][3].y, vec_face[i][3].z);
	}
	SetMaterialColor(old_clr);
}
void drawTool::Drawcone(const POINT3D &cen,double r,VECTOR3D &h)
{
	glPushMatrix();

	//平移变换至点cen  
	glTranslatef(cen.x, cen.y, cen.z);

	VECTOR3D vecNY(0, 0, 1);
	VECTOR3D axis = vecNY*h;
	double ang = _AngleBetween3D(vecNY, h);
	ang = ang * 180 / PI;
	//旋转变换  
	glRotatef(ang, axis.dx, axis.dy, axis.dz);
	//绘制圆锥面  
	GLfloat angle, x, y;
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0, 0, h.GetLength());
	for (angle = 0.0f; angle < (2.125f*PI); angle += (PI / 8.0f))
	{
		x = r*sin(angle);
		y = r*cos(angle);
		glVertex2f(x, y);
	}
	glEnd();

	// 绘制圆锥底面  
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(0.0f, 0.0f);
	for (angle = 0.0f; angle < (2.125f*PI); angle += (PI / 8.0f))
	{
		x = r*sin(angle);
		y = r*cos(angle);
		glVertex2f(x, y);
	}
	glEnd();
	glPopMatrix();

}

void drawTool::DrawSphere(const POINT3D& cen, double r)
{
	double width, height;
	//m_Camera.get_view_rect(width, height);
	//double len = min(width, height);
	//len *= 0.01;
	glPushMatrix();
	glTranslatef(cen.x, cen.y, cen.z);
	auxSolidSphere(r);
	glPopMatrix();
}
void drawTool::DrawText(char* string, int flag)
{
	char* p = NULL;
	for (p = string; *p; p++)
	{
		if (flag == 24)
			glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *p);
		else if (flag == 18)
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, *p);
		else
			glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *p);
	}
}
string drawTool::itos(int i)
{
	// 将int 转换成string{
	stringstream s;
	s << i;
	char c[3];
	strcpy(c, s.str().c_str());
	//string 转为char*

	return s.str();

}