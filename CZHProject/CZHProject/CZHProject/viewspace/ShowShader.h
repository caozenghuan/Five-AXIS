#ifndef  CShowShader_H_
#define  CShowShader_H_
#pragma once
#include "STLModell.h"
#include "K-d tree/Vec.h"
#include "KitTool.h"
namespace ViewSpace
{
	enum ShaderModel
	{
		SMOOTH_SHADER,//平滑着色
		PLANE_SHADER,//平面着色
		WIRE_FRAME,//线框模式
		HIDE_WIRE_FRAME//隐藏线框

	}; 
	class CShowShader
	{
	public:
		CShowShader();
		~CShowShader();
	public:
		/*vector<vec> vec_nomal; vector<point> vec_point;*/
	public:
		//COLORREF m_clrMaterial; //材料的颜色

		static void ShaderWireFrame(STLModell*Tmesh);
		static void PlaneShaderModel(STLModell*Tmesh);
		static void SmoothShaderModel(STLModell*Tmesh);
		static void ShaderMesh(STLModell*Tmesh, ShaderModel shadermodle0, vector<vec> vec_nomal, vector<point> vec_point, vector<vec> vec_facenomal);
		static void HideWireFrame(STLModell*Tmesh/*, vector<vec> vec_nomal, vector<point> vec_point*/);
		static void SetMaterialColor(COLORREF clr);
	};
}
#endif
