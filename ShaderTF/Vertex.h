#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <vector>

using namespace std;

class Vertex
{
public:
	Vertex(void);
	~Vertex(void);

public:
	unsigned long	m_nVertexIdx;
	glm::vec3		m_fVertexPosition;
	glm::vec3		m_fVertexNormal;

	vector<int>	m_neighborVertexIdxList;
	vector<int> m_neighborFaceIdxList;

public:
	bool	SearchOverlapVertex(int vID);
	bool	SearchOverlapFaceIndex( int fID);

	void	AddNeighborFace( int fID) { m_neighborFaceIdxList.push_back( fID); }
	void	AddNeighborVertex( int vID) { m_neighborVertexIdxList.push_back( vID); }
};

