#include "Face.h"


Face::Face(void)
{
	m_nVertexIdx = glm::int3(0, 0, 0);
	m_nTexCoordIdx = glm::int3(0, 0, 0);
	m_fFaceNormal = glm::vec3(0.0f, 0.0f, 0.0f);

	m_neighborFaceIdx.clear();
}


Face::~Face(void)
{
	m_nVertexIdx = glm::int3(0, 0, 0);
	m_nTexCoordIdx = glm::int3(0, 0, 0);
	m_fFaceNormal = glm::vec3(0.0f, 0.0f, 0.0f);

	m_neighborFaceIdx.clear();
}
