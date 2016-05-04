#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <vector>
#include "Vertex.h"

class Edge
{
public:
	Edge(void);
	~Edge(void);

	float	m_fDistance;
	glm::int2	m_nVertexIdx;
	glm::vec3	m_fEdgeNormal;

	std::vector<int> m_neighborFaceIdxList;
};

