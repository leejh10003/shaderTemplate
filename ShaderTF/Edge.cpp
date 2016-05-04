#include "Edge.h"


Edge::Edge(void)
{
	m_nVertexIdx = glm::int2(0, 0);
	m_fEdgeNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	m_neighborFaceIdxList.clear();
}


Edge::~Edge(void)
{
	m_nVertexIdx = glm::int2(0, 0);
	m_fEdgeNormal = glm::vec3(0.0f, 0.0f, 0.0f);
	m_neighborFaceIdxList.clear();
}
