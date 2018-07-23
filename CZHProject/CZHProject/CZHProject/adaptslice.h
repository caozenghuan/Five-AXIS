#pragma once
#include "STLModell.h"
#include "datastructure/mesh.h"
#include "datastructure/solidMesh.h"
#include "datastructure/solidPolygon.h"
class adaptslice
{
public:
	adaptslice();
	virtual ~adaptslice();

public:
	vector<float> Saliency(STLModell *pSTL);
	double Trianglearea(POINT3D p0, POINT3D p1, POINT3D p2);
	float  CalcuVerMeanCurvature(PVERT p00);

};

