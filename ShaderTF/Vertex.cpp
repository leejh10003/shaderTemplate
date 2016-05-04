#include "Vertex.h"


Vertex::Vertex(void)
{
	m_fVertexPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	
	m_neighborVertexIdxList.clear();
	m_neighborFaceIdxList.clear();
}


Vertex::~Vertex(void)
{
	m_fVertexPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fVertexNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	
	m_neighborVertexIdxList.clear();
	m_neighborFaceIdxList.clear();
}


//Neighbor Vertex �߿� �ߺ��� ���� �ִ��� �˻��ϴ� ����
//TRUE �� ������ ���, �ߺ� ��� ����
//False �� ������ ���, �ߺ���� ����
bool	Vertex::SearchOverlapVertex(int vID)
{
	//Check the neighbor vertex list 
	if(m_neighborVertexIdxList.empty() == true)		//if Neighbor list is empty, return false
		return false;
	else		//Else check the overlap
	{
		//Search the neighbor vertex list, if the list has component equal pVertex, return FALSE
		for(int i = 0; i < (int)m_neighborVertexIdxList.size(); i++)	
		{
			if( vID == m_neighborVertexIdxList[i] )
				return true;		//if neighbor list has equal component, return true
		}
	}
	return false;
}

bool	Vertex::SearchOverlapFaceIndex( int fID )
{
	//Check the Face index list 
	if(m_neighborFaceIdxList.empty() == true)		//if Face index list is empty, return false
		return false;
	else		//Else check the overlap
	{
		//Search the neighbor vertex list, if the list has component equal pVertex, return FALSE
		for(int i = 0; i < (int)m_neighborFaceIdxList.size(); i++)	
		{
			if( fID == m_neighborFaceIdxList[i] )
				return true;		//if neighbor list has equal component, return true
		}
	}
	return false;
}
