#ifndef  CBackGround_H_
#define  CBackGround_H_
#include <glut.h>
#include <glaux.h>
#pragma once

namespace ViewSpace
{
	enum BackGroundColor
	{
		//CHANGE_White;//白色背景
		CHANGE_GRAY,//灰色渐变
		CHANGE_DEONGAREE,//深蓝渐变
		CHANGE_LIGHT_BLUE//浅蓝渐变

	};  
class CBackGround
{
public:
	static void SetBlackGround(int Modeway);
	static void RenderBackGround(BackGroundColor backgroundcolor=CHANGE_GRAY);
	
};

}
#endif
