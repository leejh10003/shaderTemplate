#pragma once

#include <string>
#include <iostream>
#include <fstream>

#include <vector>
#include <gl/glut.h>

#include "Vertex.h"
#include "Face.h"
#include "Edge.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"


using namespace std;

#define VERTEX_NORMAL 1
#define FACE_NORMAL 2

//Transformation 
#define AXIS_X		1
#define AXIS_Y		2
#define AXIS_Z		3

class Mesh
{
public:
	Mesh(void);
	~Mesh(void);

//Mesh Properties
public:
	int				m_nNumberOfVertex;
	int				m_nNumberOfFace;
	int				m_nNumberOfEdge;
	
	glm::vec3			m_fCenterOfMass;
	glm::vec3			m_fDistancetoCOM;
	float			m_fRotateAngle;
//Exception syntex
public:
	bool			m_bIsTextureCoord;
	bool			m_bIsVertexNormal;
	int				m_nMinFaceIdx;

//Mesh Structure : vertex, face, edge, texcoordnate
public:
	GLuint			mTexture;
	vector<Vertex>  mVLists;
	vector<Face>	mFaces;
	vector<Edge>	mEdges;
	vector<glm::float2>	mTexCoordinates;
	
//Public Function
public:
	bool	LoadMeshFromFile(char* filename);
	bool	LoadMeshFromFile(char* filename, char* texFile);
	void	AddNeighbor();
	void	LaplacianSmoothing();
	void	ComputeVertexNormal();
	void	ComputeFaceNormal();
	void	ComputeEdgeNormal();
	void	RenderTexMesh();
	void	RenderSolidMesh(int nNormalType);
	void	RenderWireMesh();
	void	RenderVertexNormal();
	void	RenderFaceNormal();
	void	RenderEdgeNormal();
	
	//Transformation function
public:
	void	TranslateMesh(glm::vec3 translate);
	void	RotateMesh(int nAxis, float angle);
	void	ScaleMesh(glm::vec3 scale);

//Inline Min/Max function
public:
	inline int minValue(int a, int b) { return (a > b) ? b : a; }
	inline int maxValue(int a, int b) { return (a < b) ? b : a; }

//Print Function
public:
	void PrintMeshInformation(char* meshName);
};

