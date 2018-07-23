#pragma once

using namespace std;
#include <vector>
#include <map>
#include <qstring.h>
#include "GeomBase.h"



typedef enum SolidType{SOLID_MESH_TRI,SOLID_PIONT, SOLID_ZBF,SOLID_OTHER}SOLIDTYPE;

class CSolid
{ 
	//三维实体类
protected:
	INT_PTR		m_ID;
	SOLIDTYPE	m_SolidType;

	BOX3D		m_Box;
	QString		m_strName;	

	COLORREF	m_clrFront;			//正面材质
	COLORREF	m_clrBk;			//反面材质
	COLORREF	m_clrSelected;		//被选择时的正面材质
	COLORREF	m_clrSelectedBk;	//被选择是的反面材质

	BOOL		m_bVisible;
	BOOL		m_bSelected;
	BOOL		m_bIsActive;

	//根据法向量情况显示面片颜色
	BOOL		m_bIsColorDisp;	
	//COLORREF	m_clrUp;
	//COLORREF	m_clrDown;
	//COLORREF	m_clrVertical;

	BOOL		m_bModified;
	BOOL		m_bTransform;
	MATRIX3D	m_matTransform;

public:
	CSolid()
	{
		m_bModified = FALSE;
		m_ID = -1;
	//	m_pBox = NULL;
		m_clrFront = RGB(0,150,100);//RGB(128,128,128);
		m_clrBk = RGB(200,100,0);
		m_clrSelected = RGB(0,200,50);//RGB(150,150,0);
		m_clrSelectedBk = RGB(200,0,100);//RGB(155,155,155);

		//m_clrUp = RGB(200,50,50);
		//m_clrDown = RGB(50,200,50);
		//m_clrVertical = RGB(50,50,200);
		m_bIsColorDisp = FALSE;

		m_strName = "Unknow";
		m_bVisible = TRUE;
		m_bSelected = FALSE;

		m_SolidType = SOLID_OTHER;

		m_bIsActive = FALSE;

		m_bTransform = FALSE;
	};

	virtual ~CSolid()
	{

	};

	//attrib accessing 
	BOX3D	GetBox()
	{
		if(m_bModified)
			UpdateBox();

		return m_Box*m_matTransform;
	};

	BOX3D	GetTransformBox()
	{
		if(m_bModified)
			UpdateBox();

		return m_Box*m_matTransform;
	}

	BOOL	IsTransform()
	{
		return m_bTransform;
	}

	void	SetTransform(BOOL bTrans)
	{
		m_bTransform = bTrans;
	}

	MATRIX3D	GetTransformMat()
	{
		return m_matTransform;
	}

	void	SetTransformMat(const MATRIX3D mat)
	{
		 m_matTransform = mat;
	}

	void		SetID(INT_PTR nID)
	{
		m_ID = nID;
	};
	INT_PTR		GetID()
	{
		return m_ID;
	};

	void		SetName(QString name)
	{
		m_strName = name;
	};
	QString		GetName()
	{
		return m_strName;
	};

	void		SetFrontColor(COLORREF color)
	{
		m_clrFront = color;
	};
	COLORREF	GetFrontColor()
	{
		return m_clrFront;
	};

	void		SetBackColor(COLORREF color)
	{
		m_clrBk = color;
	};
	COLORREF	GetBackColor()
	{
		return m_clrBk;
	};

	void		SetSelectedFrontColor(COLORREF color)
	{
		m_clrSelected = color;
	};
	COLORREF	GetSelectedFrontColor()
	{
		return m_clrSelected;
	};

	void		SetSelectedBackColor(COLORREF color)
	{
		m_clrSelectedBk = color;
	};
	COLORREF	GetSelectedBackColor()
	{
		return m_clrSelectedBk;
	};

	void		SetVisible(BOOL bVisible)
	{
		m_bVisible = bVisible;
	};
	BOOL		IsVisible()
	{
		return m_bVisible;
	};

	void		SetSelected(BOOL bSelected)
	{
		m_bSelected = bSelected;
	};
	BOOL		IsSelected()
	{
		return m_bSelected;
	};

	void		SetColorDispTag(BOOL bClrDisp)
	{
		m_bIsColorDisp = bClrDisp;
	};
	BOOL		IsColorDisp()
	{
		return m_bIsColorDisp;
	};

	void		SetSolidType(SOLIDTYPE sType)
	{
		m_SolidType = sType;
	};
	SOLIDTYPE	GetSolidType()
	{
		return m_SolidType;
	};	

	void		SetActiveStatus(BOOL bIsActive = FALSE)
	{
		m_bIsActive = bIsActive;
	};
	BOOL		IsActive()
	{
		return m_bIsActive;
	};

	void		SetModifyStatus(BOOL bIsModified = FALSE)
	{
		m_bModified = bIsModified;
	};
	BOOL		IsModify()
	{
		return m_bModified;
	};


public:
	virtual bool	IsEmpty()		= 0;
	virtual	void	UpdateBox()		= 0;
};

//typedef	vector<CEntity> vec_ENTITY;
//typedef vec_ENTITY::iterator it_vec_ENTITY;
typedef	vector<CSolid*> vec_PSOLID;
typedef vec_PSOLID::iterator it_vec_PSOLID;