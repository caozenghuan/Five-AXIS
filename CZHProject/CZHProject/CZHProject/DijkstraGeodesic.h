//#ifndef  CDijkstraGeodesic_H_
//#define  CDijkstraGeodesic_H_
#pragma once
#include "STLModell.h"
#include "K-d tree/KDtree.h"
#include "K-d tree/Vec.h"
#include "datastructure/GeomBase.h"
#include "KitTool.h"
//using namespace std;
	class CDijkstraGeodesic
	{

	public:
		CDijkstraGeodesic();
		virtual~CDijkstraGeodesic();

	public:
		void Compute_Distance();
		vector<int > pre;
		vector<int> m_Point_BeginEnd;

		KitTool Pkittool;
		//float m_Distance;
	public:
		class CNode
		{    
			typedef CNode* CNode_pointer;
		public:
			CNode(void){m_VisitFlag=Inactive;m_GeodesicDistance =std::numeric_limits<float>::max();m_PreID=-1;m_Is_Source=false;};
			~CNode(void){};
			enum VisitFlag{Frozen,Active,Inactive};
			VisitFlag m_VisitFlag;
			float m_GeodesicDistance;
			int  m_VertexID;
			int m_PreID;
			VECTOR2D  m_UV;
			VECTOR3D  m_Normal,m_e1,m_e2;
			bool m_Is_Source;


			float& distance_from_source(){return m_GeodesicDistance;};
			virtual bool operator()(CNode_pointer const s1, CNode_pointer const s2) const
			{
				return s1->distance_from_source()!=s2->distance_from_source() ?
					s1->distance_from_source() < s2->distance_from_source() ://不等的时候根据距离排序
				s1->m_VertexID< s2->m_VertexID;//相等的时候需要根据ID号来
			};



		};
		typedef CNode* CNode_pointer;
		
		float  Propagate(STLModell*Tmesh,int source,int target,vector<int>&path);
		MATRIX3D AlignNormal(STLModell*m_Tmesh, int p, int q);
		void  Average_e1(vector<CNode>&Anode, int p, VECTOR3D &Averagee1);
		void Average_GVector(vector<CNode>&Anode, int q, VECTOR2D &AverageGvector);
		void SeedTangentPlane(VECTOR3D seedNormal0, VECTOR3D &e1, VECTOR3D &e2);

		float Update_Dist(STLModell*Tmesh, vector<CNode>&Anode, int v, int w, int k_v);
		VECTOR2D Get_2D_Edge(STLModell*m_Tmesh, vector<CNode>&Anode, int pre, int v);
		VECTOR2D PathFlatten(STLModell*m_Tmesh, vector<int>path);
	};

//#endif
