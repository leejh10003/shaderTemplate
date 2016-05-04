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


//Neighbor Vertex 중에 중복된 것이 있는지 검사하는 구문
//TRUE 를 리턴할 경우, 중복 요소 있음
//False 를 리턴할 경우, 중복요소 없음
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
