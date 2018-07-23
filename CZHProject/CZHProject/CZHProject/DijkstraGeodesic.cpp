#include "DijkstraGeodesic.h"
CDijkstraGeodesic::CDijkstraGeodesic()
{
}
CDijkstraGeodesic::~CDijkstraGeodesic()
{
}

float  CDijkstraGeodesic::Propagate(STLModell*Tmesh, int source, int target, vector<int>&path)
{
	int vn = Tmesh->m_vecPVert.size();
	//方法一  使用最短路径搜寻法
	vector<CNode>m_nodes(vn);
#pragma omp parallel for
	for (int i=0;i<vn;i++)
	{   
		m_nodes[i].m_VertexID=i;
		VECTOR3D nor = Tmesh->m_vecPVert[i]->VertexNormal;
		nor.Normalize();
		m_nodes[i].m_Normal =nor;
	}
	VECTOR3D SeedNor(Tmesh->m_vecPVert[source]->VertexNormal.dx, Tmesh->m_vecPVert[source]->VertexNormal.dy, Tmesh->m_vecPVert[source]->VertexNormal.dz);
	SeedTangentPlane(SeedNor,m_nodes[source].m_e1,m_nodes[source].m_e2);
	m_nodes[source].m_UV=VECTOR2D (0,0);
	m_nodes[source].m_VisitFlag=CNode::Active;
	m_nodes[source].distance_from_source()=0;
	m_nodes[source].m_Is_Source=true;
	std::set<CNode_pointer, CNode>  Queue0; 
	Queue0.clear();
	Queue0.insert(&m_nodes[source]);


	while(!Queue0.empty())
	{
		CNode_pointer min_node = *Queue0.begin();
		Queue0.erase(Queue0.begin());
		min_node->m_VisitFlag=CNode::Frozen;
		vec_PVERT vec_oneRing;
		KitTool::FindOneRing(Tmesh->m_vecPVert[min_node->m_VertexID], vec_oneRing);//着重检查
		vector<int> NeighborV;
		for (int i = 0; i < vec_oneRing.size(); i++)
		{
			NeighborV.push_back(vec_oneRing[i]->VertexID);
		}
		for(unsigned i=0; i<NeighborV.size(); ++i)		
		{
			CNode_pointer next_node =&m_nodes[NeighborV[i]];
			if(next_node->m_VisitFlag==CNode::Frozen)continue;
			float a=Update_Dist(Tmesh,m_nodes,min_node->m_VertexID,next_node->m_VertexID,4);
			if(next_node->distance_from_source()>a)
			{
				if(next_node->m_VertexID==CNode::Active)
				{
					Queue0.erase(next_node);
					next_node->m_VertexID=CNode::Inactive;
				}	
				next_node->m_PreID=min_node->m_VertexID;
				next_node->distance_from_source() = a;
			}
		}

		if(min_node->m_VertexID==target)break;
		for (int j = 0; j< NeighborV.size(); j++)
		{
			int neighbors_of_u = NeighborV[j];
			if (m_nodes[neighbors_of_u].m_VisitFlag==CNode::Inactive)
			{
				Queue0.insert(&m_nodes[neighbors_of_u]);
				m_nodes[neighbors_of_u].m_VisitFlag=CNode::Active;			
			}

		}

	}//while

	path.push_back(target);
	while(1)
	{
		int pre_v=m_nodes[path[path.size()-1]].m_PreID;
		path.push_back(pre_v);
		if(pre_v==source)break;
	}
	//m_Distance=m_nodes[target].distance_from_source();

	VECTOR2D uv=PathFlatten(Tmesh,path);
	float distance0=uv.GetLength();
	return distance0;
}
float CDijkstraGeodesic::Update_Dist(STLModell*Tmesh, vector<CNode>&Anode, int v, int w, int k_v)
{
	int ori_pre=Anode[w].m_PreID;
	Anode[w].m_PreID=v;
	int temp_v=w;

	
	vector<int>pre_vector;
	pre_vector.push_back(w);
	int count0=0;
	while(count0<k_v)//回溯前面的K个顶点
	{
		int pre=Anode[temp_v].m_PreID;
		pre_vector.push_back(pre);
		count0++;
		if (Anode[pre].m_Is_Source)break;
		else temp_v=pre;
	}


	VECTOR3D w_e1, w_e2;
	int endv = pre_vector[pre_vector.size() - 1];
	SeedTangentPlane(VECTOR3D(Tmesh->m_vecPVert[endv]->VertexNormal.dx, Tmesh->m_vecPVert[endv]->VertexNormal.dy, Tmesh->m_vecPVert[endv]->VertexNormal.dz), w_e1, w_e1);
	vector<VECTOR3D>pre_e1(pre_vector.size());
	pre_e1[pre_vector.size()-1]=w_e1;
	for (int i=pre_vector.size()-2;i>=0;i--)
	{
		pre_e1[i]=pre_e1[i+1]*AlignNormal(Tmesh,pre_vector[i+1],pre_vector[i]);
	}
	
	vector<VECTOR3D>pre_e2(pre_e1.size());
	for (int i = 0; i < pre_e2.size(); i++)
	{
		VECTOR3D n = Tmesh->m_vecPVert[pre_vector[i]]->VertexNormal;
		pre_e2[i] = pre_e1[i] * VECTOR3D(n.dx, n.dy, n.dz);
		pre_e2[i].Normalize();
		pre_e1[i].Normalize();
	}


	vector<float>all_dist(1000000);
	for (int i = 1; i < pre_vector.size(); i++)
	{
		VECTOR2D  vector_edge;
		for (int j = 1; j <= i; j++)
		{
			VECTOR3D AB0 = *Tmesh->m_vecPVert[pre_vector[j - 1]] - *Tmesh->m_vecPVert[pre_vector[j]];
			AB0 = AB0*AlignNormal(Tmesh, pre_vector[j - 1], pre_vector[j]);
			vector_edge = vector_edge + VECTOR2D(pre_e1[j] | AB0, pre_e2[j] | AB0);
		}
		all_dist[i - 1] = Anode[pre_vector[i]].distance_from_source() + vector_edge.GetLength();
	}

	float min_dist = 1000000;
	for (int i = 0; i < k_v; i++)
	{
		if (all_dist[i]!=0)//自己加的，不知靠谱不
		{
			if (all_dist[i] < min_dist)  min_dist = all_dist[i];
		}
	}
	Anode[w].m_PreID = ori_pre;
	return min_dist;
}
//path最后一个点存路径的源点，通过把边向量旋转展平到源点的切平面上
VECTOR2D CDijkstraGeodesic::PathFlatten(STLModell*m_Tmesh, vector<int>path)
{
	VECTOR3D w_e1, w_e2;
	int endv = path[path.size() - 1];
	SeedTangentPlane(VECTOR3D(m_Tmesh->m_vecPVert[endv]->VertexNormal.dx, m_Tmesh->m_vecPVert[endv]->VertexNormal.dy, m_Tmesh->m_vecPVert[endv]->VertexNormal.dz), w_e1, w_e1);
	vector<VECTOR3D>pre_e1(path.size());
	pre_e1[path.size() - 1] = w_e1;
	for (int i = path.size() - 2; i >= 0; i--)
	{
		pre_e1[i] = pre_e1[i + 1] * AlignNormal(m_Tmesh, path[i + 1], path[i]);
	}

	vector<VECTOR3D>pre_e2(pre_e1.size());
	for (int i = 0; i < pre_e2.size(); i++)
	{
		VECTOR3D n = m_Tmesh->m_vecPVert[path[i]]->VertexNormal;
		pre_e2[i] = pre_e1[i] * VECTOR3D(n.dx, n.dy, n.dz);
		pre_e2[i].Normalize();
		pre_e1[i].Normalize();
	}


	VECTOR2D  vector_edge;
	for (int i = 1; i < path.size(); i++)
	{
		VECTOR3D AB = *m_Tmesh->m_vecPVert[path[i - 1]] - *m_Tmesh->m_vecPVert[path[i]];
		VECTOR3D AB0(AB.dx, AB.dy, AB.dz);
		AB0 = AB0*AlignNormal(m_Tmesh, path[i - 1], path[i]);
		vector_edge = vector_edge + VECTOR2D(pre_e1[i] | AB0, pre_e2[i] | AB0);
	}
	return vector_edge;
}
VECTOR2D CDijkstraGeodesic::Get_2D_Edge(STLModell*m_Tmesh, vector<CNode>&Anode, int pre, int v)
{
	VECTOR3D AB0 = *m_Tmesh->m_vecPVert[v] - *m_Tmesh->m_vecPVert[pre];
	AB0 = AB0*AlignNormal(m_Tmesh, v, pre);
	return VECTOR2D(Anode[pre].m_e1 | AB0, Anode[pre].m_e2 | AB0);
}

//求解e1 e2
void CDijkstraGeodesic::SeedTangentPlane(VECTOR3D seedNormal0, VECTOR3D &e1, VECTOR3D &e2)
{

	int indexflag = 0;
	double maxxyz = 0;
	vector<double> seedNormal;
	seedNormal.push_back(seedNormal0.dx); seedNormal.push_back(seedNormal0.dy); seedNormal.push_back(seedNormal0.dz); 
	//seedNormal[1] = seedNormal0.dy; seedNormal[2] = seedNormal0.dz;
	for (int i = 0; i < 3; i++)
	{
		if (maxxyz < fabs(seedNormal[i]))
		{
			indexflag = i;
			maxxyz = fabs(seedNormal[i]);
		}
	}
	if (indexflag == 0 || indexflag == 1)
	{
		e1.dx = -seedNormal[1];
		e1.dy = seedNormal[0];
		e1.dz = 0.0;
	}
	else if (indexflag == 2)
	{
		e1.dx = -seedNormal[2];
		e1.dy = 0.0;
		e1.dz = seedNormal[0];
	}

	VECTOR3D n(seedNormal[0], seedNormal[1], seedNormal[2]);
	e2 = n*e1;
	e1.Normalize();
	e2.Normalize();

}

//求p的法向量往q法向量的旋转矩阵
MATRIX3D CDijkstraGeodesic::AlignNormal(STLModell*m_Tmesh, int p, int q)
{
	double angle = 0.0;
	VECTOR3D p_Nor, q_Nor, Cross_qN_pN;
	MATRIX3D rotMtrx1;
	p_Nor = VECTOR3D(m_Tmesh->m_vecPVert[p]->VertexNormal.dx, m_Tmesh->m_vecPVert[p]->VertexNormal.dy, m_Tmesh->m_vecPVert[p]->VertexNormal.dz);
	q_Nor = VECTOR3D(m_Tmesh->m_vecPVert[q]->VertexNormal.dx, m_Tmesh->m_vecPVert[q]->VertexNormal.dy, m_Tmesh->m_vecPVert[q]->VertexNormal.dz);;
	//两个法失平行时
	if ((p_Nor*q_Nor).GetLength() < 1e-4)
	{
		rotMtrx1.IdenticalMatrix();
		return rotMtrx1;
	}
	p_Nor.Normalize();
	q_Nor.Normalize();
	angle = p_Nor | q_Nor;
	angle = acos(angle);
	Cross_qN_pN = p_Nor*q_Nor;
	rotMtrx1 = rotMtrx1.CreateRotateMatrix(angle, Cross_qN_pN);

	return rotMtrx1;
}