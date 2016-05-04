#pragma once

#include "glm\glm.hpp"
#include "glm\ext.hpp"
#include <vector>
#include "Vertex.h"

using namespace std;

class Face
{
public:
	Face(void);
	~Face(void);

public:
	
	glm::int3	m_nVertexIdx;
	glm::int3	m_nTexCoordIdx;
	glm::int3	m_nEdgeIdx;

	glm::vec3	m_fFaceNormal;
	

	vector<int> m_neighborFaceIdx;
	

};

