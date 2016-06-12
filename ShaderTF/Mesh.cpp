
#include "stb_image.h"
#include "Mesh.h"


Mesh::Mesh(void)
{
	m_nNumberOfVertex = 0;
	m_nNumberOfFace = 0;
	m_nNumberOfEdge = 0;
	
	m_fCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fDistancetoCOM = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fRotateAngle = 0.0f;

	m_bIsTextureCoord = false;
	m_bIsVertexNormal = false;
	m_nMinFaceIdx = 1000000;

	mVLists.clear();
	mFaces.clear();
	mEdges.clear();
	mTexCoordinates.clear();
}


Mesh::~Mesh(void)
{
	m_nNumberOfVertex = 0;
	m_nNumberOfFace = 0;
	m_nNumberOfEdge = 0;
	m_fCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);

	m_bIsTextureCoord = false;
	m_bIsVertexNormal = false;
	m_nMinFaceIdx = 1000000;

	mVLists.clear();
	mFaces.clear();
	mEdges.clear();
	mTexCoordinates.clear();
}

bool Mesh::LoadMeshFromFile(char* filename, char* texFile)
{
	FILE *fp = fopen(texFile, "rb");
	if (!fp){ printf("ERROR : No %s.\n fail to bind %d\n", texFile, mTexture);  return false; }

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);
	fclose(fp);
	
	//bind
	glBindTexture(GL_TEXTURE_2D, mTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return LoadMeshFromFile(filename);
}

bool Mesh::LoadMeshFromFile(char* filename)
{
	ifstream fs;
	fs.open(filename);

	if(fs.fail())
	{
		printf("--- Error :: Can not found file ! ---");
		return false;
	}
	else 
	{
		while(!fs.eof())
		{
			string n;
			fs >> n;

			string line;
			if( n == "#" )
			{
				getline(fs, line);
			}
			else if( n == "v" ) 
			{
				float x, y, z;
				fs >> x;
				fs >> y;
				fs >> z;

				m_fCenterOfMass.x = x;
				m_fCenterOfMass.y = y;
				m_fCenterOfMass.z = z;

				Vertex v;
				v.m_fVertexPosition = glm::vec3(x, y, z);
				mVLists.push_back(v);
			}
			else if ( n == "f" )
			{
				int x, y, z;
				int u, v, w;
				int findNum = 0;

				string faceUv, faceUv2, faceUv3;
				fs >> faceUv;
				fs >> faceUv2;
				fs >> faceUv3;

				findNum = faceUv.find("/");

				string strX = faceUv.substr(0, findNum);
				string strU = faceUv.substr(findNum+1);

				findNum = faceUv2.find("/");
				string strY = faceUv2.substr(0, findNum);
				string strV = faceUv2.substr(findNum+1);

				findNum = faceUv3.find("/");
				string strZ = faceUv3.substr(0, findNum);
				string strW = faceUv3.substr(findNum+1);


				x = atoi(strX.c_str());
				u = atoi(strU.c_str());

				y = atoi(strY.c_str());
				v = atoi(strV.c_str());

				z = atoi(strZ.c_str());
				w = atoi(strW.c_str());

				Face f;
				f.m_nVertexIdx = glm::vec3(x, y, z);
				f.m_nTexCoordIdx = glm::int3(u, v, w);
				mFaces.push_back(f);

				//Serch min face index
				m_nMinFaceIdx = minValue(m_nMinFaceIdx, x);
				m_nMinFaceIdx = minValue(m_nMinFaceIdx, y);
				m_nMinFaceIdx = minValue(m_nMinFaceIdx, z);

				//Add neighbor vertex and edge
			}
			else if ( n == "vt" )
			{
				float u,v;
				fs >> u;
				fs >> v;
				
			//	u = 1.0f - u;
				v = 1.0f - v;
				mTexCoordinates.push_back(glm::float2(u, v));
			}
		}
		//Close file stream
		fs.close();

		//Correct face min idx
		if( m_nMinFaceIdx > 0 )
		{
			for(int i = 0; i < (int)mFaces.size(); i ++)
			{
				mFaces[i].m_nVertexIdx.x -= m_nMinFaceIdx;
				mFaces[i].m_nVertexIdx.y -= m_nMinFaceIdx;
				mFaces[i].m_nVertexIdx.z -= m_nMinFaceIdx;

				mFaces[i].m_nTexCoordIdx.x -= 1;
				mFaces[i].m_nTexCoordIdx.y -= 1;
				mFaces[i].m_nTexCoordIdx.z -= 1;
			}
		}
	
		//Generate Edge List and Neighbor Vertex List and Neighbor face
		AddNeighbor();

		//Compute Face Normal
		ComputeFaceNormal();
		
		//Compute Vertex Normal
		ComputeVertexNormal();

		//Compute Edge Normal
		ComputeEdgeNormal();

		//Update Mesh properties 
		m_nNumberOfVertex = (int)mVLists.size();
		m_nNumberOfFace = (int)mFaces.size();
		m_nNumberOfEdge = (int)mEdges.size();

		//Compute Center of mass
		m_fCenterOfMass.x /= (float)m_nNumberOfVertex;
		m_fCenterOfMass.y /= (float)m_nNumberOfVertex;
		m_fCenterOfMass.z /= (float)m_nNumberOfVertex;

		//Print Log
		PrintMeshInformation(filename);
	}
	
	return true;
}

void Mesh::AddNeighbor()
{
	for(int i = 0; i < (int)mFaces.size(); i ++)
	{
		int	v1 = mFaces[i].m_nVertexIdx.x;
		int	v2 = mFaces[i].m_nVertexIdx.y;
		int	v3 = mFaces[i].m_nVertexIdx.z;
		 
		mVLists[v1].AddNeighborFace(i);
		mVLists[v2].AddNeighborFace(i);
		mVLists[v3].AddNeighborFace(i);
		
		//Add Neighbor Vertex and edge
		if( mVLists[v1].SearchOverlapVertex(v2) == false)
		{
			mVLists[v1].AddNeighborVertex(v2);
			Edge e;
			e.m_nVertexIdx = glm::int2(v1, v2);
			glm::vec3 diff;
			diff.x = mVLists[v1].m_fVertexPosition.x - mVLists[v3].m_fVertexPosition.x;
			diff.y = mVLists[v1].m_fVertexPosition.y - mVLists[v3].m_fVertexPosition.y;
			diff.z = mVLists[v1].m_fVertexPosition.z - mVLists[v3].m_fVertexPosition.z;
			e.m_fDistance = glm::length(diff);
			mEdges.push_back(e);
		}
		if( mVLists[v1].SearchOverlapVertex(v3) == false)
		{
			mVLists[v1].AddNeighborVertex(v3);
			Edge e;
			e.m_nVertexIdx = glm::int2(v1, v3);
			glm::vec3 diff;
			diff.x = mVLists[v1].m_fVertexPosition.x - mVLists[v3].m_fVertexPosition.x;
			diff.y = mVLists[v1].m_fVertexPosition.y - mVLists[v3].m_fVertexPosition.y;
			diff.z = mVLists[v1].m_fVertexPosition.z - mVLists[v3].m_fVertexPosition.z;
			e.m_fDistance = glm::length(diff);
			mEdges.push_back(e);
		}

		if( mVLists[v2].SearchOverlapVertex(v1) == false)
		{
			mVLists[v2].AddNeighborVertex(v1);
		}
		if( mVLists[v2].SearchOverlapVertex(v3) == false)
		{
			mVLists[v2].AddNeighborVertex(v3);
			Edge e;
			e.m_nVertexIdx = glm::int2(v2, v3);
			glm::vec3 diff;
			diff.x = mVLists[v1].m_fVertexPosition.x - mVLists[v3].m_fVertexPosition.x;
			diff.y = mVLists[v1].m_fVertexPosition.y - mVLists[v3].m_fVertexPosition.y;
			diff.z = mVLists[v1].m_fVertexPosition.z - mVLists[v3].m_fVertexPosition.z;
			e.m_fDistance = glm::length(diff);
			mEdges.push_back(e);
		}

		if(mVLists[v3].SearchOverlapVertex(v2) == false)
		{
			mVLists[v3].AddNeighborVertex(v2);
		}
		if( mVLists[v3].SearchOverlapVertex(v1) == false)
		{
			mVLists[v3].AddNeighborVertex(v1);
		}
	}

	//Add to Neighbor face index in edge structure
	for(int i = 0; i < (int)mEdges.size(); i ++)
	{
		int vertA = mEdges[i].m_nVertexIdx.x;
		int vertB = mEdges[i].m_nVertexIdx.y;

		for(int j = 0; j < (int)mVLists[vertA].m_neighborFaceIdxList.size(); j ++)
		{
			for(int k = 0; k < (int)mVLists[vertB].m_neighborFaceIdxList.size(); k ++)
			{
				if( mVLists[vertA].m_neighborFaceIdxList[j] == mVLists[vertB].m_neighborFaceIdxList[k] )
					mEdges[i].m_neighborFaceIdxList.push_back(mVLists[vertA].m_neighborFaceIdxList[j]);
			}
		}
	}
}

void Mesh::LaplacianSmoothing()
{
	std::vector<glm::vec3> tempVertexPos;

	for(int i = 0 ; i < (int)mVLists.size(); i ++)
	{
		glm::vec3 vecPos = glm::vec3(0.0f, 0.0f, 0.0f);
		for(int j = 0; j < (int)mVLists[i].m_neighborVertexIdxList.size(); j ++)
		{
			glm::vec3 vecA = mVLists[mVLists[i].m_neighborVertexIdxList[j]].m_fVertexPosition;
			glm::vec3 vecB = mVLists[i].m_fVertexPosition; 
			glm::vec3 diff = vecB - vecA;
			vecPos.x += diff.x;
			vecPos.y += diff.y;
			vecPos.z += diff.z;
		}

		vecPos.x = vecPos.x / ((float)mVLists[i].m_neighborVertexIdxList.size());
		vecPos.y = vecPos.y / ((float)mVLists[i].m_neighborVertexIdxList.size());
		vecPos.z = vecPos.z / ((float)mVLists[i].m_neighborVertexIdxList.size());

		tempVertexPos.push_back(vecPos);
	}

	for(int i = 0; i < (int)mVLists.size(); i++)
	{
		mVLists[i].m_fVertexPosition.x = mVLists[i].m_fVertexPosition.x + tempVertexPos[i].x * 0.01f;
		mVLists[i].m_fVertexPosition.y = mVLists[i].m_fVertexPosition.y + tempVertexPos[i].y * 0.01f;
		mVLists[i].m_fVertexPosition.z = mVLists[i].m_fVertexPosition.z + tempVertexPos[i].z * 0.01f;
	}
}

void Mesh::ComputeVertexNormal()
{
	for(int i = 0; i < (int)mVLists.size(); i++)
	{
		Vertex *pVertex = &mVLists[i];
		glm::vec3 avrFaceNormal = glm::vec3(0.0f, 0.0f, 0.0f); 
		
		for(int j = 0; j < (int)pVertex->m_neighborFaceIdxList.size(); j ++)
		{
			avrFaceNormal.x += mFaces[ pVertex->m_neighborFaceIdxList[j] ].m_fFaceNormal.x;
			avrFaceNormal.y += mFaces[ pVertex->m_neighborFaceIdxList[j] ].m_fFaceNormal.y;
			avrFaceNormal.z += mFaces[ pVertex->m_neighborFaceIdxList[j] ].m_fFaceNormal.z;
		}
		/*
		avrFaceNormal.x = avrFaceNormal.x / (float)pVertex->m_neighborFaceIdxList.size();
		avrFaceNormal.y = avrFaceNormal.y / (float)pVertex->m_neighborFaceIdxList.size();
		avrFaceNormal.z = avrFaceNormal.z / (float)pVertex->m_neighborFaceIdxList.size();
		*/
		avrFaceNormal = glm::normalize(avrFaceNormal);
		pVertex->m_fVertexNormal = avrFaceNormal;

	}
}

void Mesh::ComputeFaceNormal()
{
	for(int i = 0; i < (int)mFaces.size(); i ++)
	{
		int idx1, idx2, idx3;
		idx1 = mFaces[i].m_nVertexIdx.x;
		idx2 = mFaces[i].m_nVertexIdx.y;
		idx3 = mFaces[i].m_nVertexIdx.z;

		glm::vec3 vecA;
		vecA.x = mVLists[ idx2].m_fVertexPosition.x - mVLists[idx1].m_fVertexPosition.x;
		vecA.y = mVLists[ idx2].m_fVertexPosition.y - mVLists[idx1].m_fVertexPosition.y;
		vecA.z = mVLists[ idx2].m_fVertexPosition.z - mVLists[idx1].m_fVertexPosition.z;

		glm::vec3 vecB;
		vecB.x = mVLists[idx3].m_fVertexPosition.x - mVLists[ idx1].m_fVertexPosition.x;
		vecB.y = mVLists[idx3].m_fVertexPosition.y - mVLists[ idx1].m_fVertexPosition.y;
		vecB.z = mVLists[idx3].m_fVertexPosition.z - mVLists[ idx1].m_fVertexPosition.z;

		glm::vec3 crossValue = glm::cross(vecA, vecB);
		glm::vec3 normal = glm::normalize(crossValue);

		mFaces[i].m_fFaceNormal = normal;
	}
}

void Mesh::ComputeEdgeNormal()
{
	for(int i = 0; i < (int)mEdges.size(); i ++)
	{
		glm::vec3 sumOfNormal = glm::vec3(0.0f);
		for(int j = 0; j < (int)mEdges[i].m_neighborFaceIdxList.size(); j ++)
		{
			sumOfNormal.x += mFaces[mEdges[i].m_neighborFaceIdxList[j]].m_fFaceNormal.x;
			sumOfNormal.y += mFaces[mEdges[i].m_neighborFaceIdxList[j]].m_fFaceNormal.y;
			sumOfNormal.z += mFaces[mEdges[i].m_neighborFaceIdxList[j]].m_fFaceNormal.z;
		}

		sumOfNormal.x /= (float)mEdges[i].m_neighborFaceIdxList.size();
		sumOfNormal.y /= (float)mEdges[i].m_neighborFaceIdxList.size();
		sumOfNormal.z /= (float)mEdges[i].m_neighborFaceIdxList.size();

		glm::vec3 normal = glm::normalize(sumOfNormal);
		mEdges[i].m_fEdgeNormal = normal;
		
	}
}

//Trnasformation
void Mesh::TranslateMesh(glm::vec3 translate)
{
	m_fCenterOfMass = glm::vec3(0.0f, 0.0f, 0.0f);

	for(int i = 0; i < (int)m_nNumberOfVertex; i ++)
	{
		mVLists[i].m_fVertexPosition.x += translate.x;
		mVLists[i].m_fVertexPosition.y += translate.y;
		mVLists[i].m_fVertexPosition.z += translate.z;

		m_fCenterOfMass.x += mVLists[i].m_fVertexPosition.x;
		m_fCenterOfMass.y += mVLists[i].m_fVertexPosition.y;
		m_fCenterOfMass.z += mVLists[i].m_fVertexPosition.z;
	}
	m_fCenterOfMass.x /= (float)m_nNumberOfVertex;
	m_fCenterOfMass.y /= (float)m_nNumberOfVertex;
	m_fCenterOfMass.z /= (float)m_nNumberOfVertex;

	//Compute Normal
	ComputeVertexNormal();
	ComputeFaceNormal();
	ComputeEdgeNormal();
}

void Mesh::RotateMesh(int nAxis, float angle)
{
	//Translate origin position
	glm::vec3 origin = glm::vec3(0.0f, 0.0f, 0.0f);
	m_fDistancetoCOM.x = origin.x - m_fCenterOfMass.x;
	m_fDistancetoCOM.y = origin.y - m_fCenterOfMass.y;
	m_fDistancetoCOM.z = origin.z - m_fCenterOfMass.z;
	TranslateMesh(m_fDistancetoCOM);

	float radian = angle * 0.017453292519943295769236907684886;
	//Compute rotaion matrix
	float matRot[3][3];
	float cos_angle = (float)cos(radian);
	float sin_angle = (float)sin(radian);
	
	if( nAxis == AXIS_X)
	{
		matRot[0][0] = 1.0f;		matRot[0][1] = 0.0f;		matRot[0][2] = 0.0f;
		matRot[1][0] = 0.0f;		matRot[1][1] = cos_angle;	matRot[1][2] = -sin_angle;
		matRot[2][0] = 0.0f;		matRot[2][1] = sin_angle;	matRot[2][2] = cos_angle;
	}
	else if (nAxis == AXIS_Y)
	{
		matRot[0][0] = cos_angle;	matRot[0][1] = 0.0f;		matRot[0][2] = sin_angle;
		matRot[1][0] = 0.0f;		matRot[1][1] = 1.0f;		matRot[1][2] = 0.0f;
		matRot[2][0] = -sin_angle;	matRot[2][1] = 0.0f;		matRot[2][2] = cos_angle;
	}
	else
	{
		matRot[0][0] = cos_angle;	matRot[0][1] = -sin_angle;	matRot[0][2] = 0.0f;
		matRot[1][0] = sin_angle;	matRot[1][1] = cos_angle;	matRot[1][2] = 0.0f;
		matRot[2][0] = 0.0f;		matRot[2][1] = 0.0f;		matRot[2][2] = 1.0f;
	}

	//Compute Rotation vector
	for(int i = 0; i < m_nNumberOfVertex; i ++)
	{
		float translateVector[3];
		translateVector[0] = mVLists[i].m_fVertexPosition.x;
		translateVector[1] = mVLists[i].m_fVertexPosition.y;
		translateVector[2] = mVLists[i].m_fVertexPosition.z;

		float rotVector[3];
		for(int j = 0; j < 3; j ++)
		{
			float sum = 0.0f;
			for(int k = 0; k < 3; k ++)
			{
				sum += translateVector[k] * matRot[j][k];
			}
			rotVector[j] = sum;
		}
		//Applying Rotation
		mVLists[i].m_fVertexPosition.x = rotVector[0];
		mVLists[i].m_fVertexPosition.y = rotVector[1];
		mVLists[i].m_fVertexPosition.z = rotVector[2];
	}

	//원래위치로 복원
	glm::vec3 reposition;
	reposition.x = -m_fDistancetoCOM.x;
	reposition.y = -m_fDistancetoCOM.y;
	reposition.z = -m_fDistancetoCOM.z;
	TranslateMesh(reposition);


}

void Mesh::ScaleMesh(glm::vec3 scale)
{
	for(int i = 0; i < m_nNumberOfVertex; i ++)
	{
		mVLists[i].m_fVertexPosition.x *= scale.x;
		mVLists[i].m_fVertexPosition.y *= scale.y;
		mVLists[i].m_fVertexPosition.z *= scale.z;
	}
}

void Mesh::RenderTexMesh()
{
	//glEnable(GL_TEXTURE_2D);

	glBegin(GL_TRIANGLES);
	for(int index = 0; index < (int)mFaces.size(); index++)
	{
		int idx1, idx2, idx3;
		idx1 = mFaces[index].m_nVertexIdx.x;
		idx2 = mFaces[index].m_nVertexIdx.y;
		idx3 = mFaces[index].m_nVertexIdx.z;

		int tex1, tex2, tex3;
		tex1 = mFaces[index].m_nTexCoordIdx.x;
		tex2 = mFaces[index].m_nTexCoordIdx.y;
		tex3 = mFaces[index].m_nTexCoordIdx.z;

		glm::vec3 vNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		
		vNormal = mVLists[idx1].m_fVertexNormal;
		glNormal3f( vNormal.x, vNormal.y, vNormal.z);
		glTexCoord2f(mTexCoordinates[tex1].x, mTexCoordinates[tex1].y);
		glVertex3f(	mVLists[idx1].m_fVertexPosition.x, 
					mVLists[idx1].m_fVertexPosition.y,
					mVLists[idx1].m_fVertexPosition.z);	


		vNormal = mVLists[idx2].m_fVertexNormal;
		glNormal3f( vNormal.x, vNormal.y, vNormal.z); 
		glTexCoord2f(mTexCoordinates[tex2].x, mTexCoordinates[tex2].y);
		glVertex3f(	mVLists[idx2].m_fVertexPosition.x, 
			mVLists[idx2].m_fVertexPosition.y,
			mVLists[idx2].m_fVertexPosition.z);	


		vNormal = mVLists[idx3].m_fVertexNormal;
		glNormal3f( vNormal.x, vNormal.y, vNormal.z); 
		glTexCoord2f(mTexCoordinates[tex3].x, mTexCoordinates[tex3].y);
		glVertex3f(	mVLists[idx3].m_fVertexPosition.x, 
					mVLists[idx3].m_fVertexPosition.y,
					mVLists[idx3].m_fVertexPosition.z);	

	}
	glEnd(); 

}

void Mesh::RenderSolidMesh(int nNormalType)
{
	glBegin(GL_TRIANGLES);
	
	for(int index = 0; index < (int)mFaces.size(); index++)
	{
		int idx1, idx2, idx3;
		idx1 = mFaces[index].m_nVertexIdx.x;
		idx2 = mFaces[index].m_nVertexIdx.y;
		idx3 = mFaces[index].m_nVertexIdx.z;

		int tex1, tex2, tex3;
		tex1 = mFaces[index].m_nTexCoordIdx.x;
		tex2 = mFaces[index].m_nTexCoordIdx.y;
		tex3 = mFaces[index].m_nTexCoordIdx.z;

		glm::vec3 vNormal = glm::vec3(0.0f, 0.0f, 0.0f);
		if( nNormalType == VERTEX_NORMAL)
		{
			vNormal = mVLists[idx1].m_fVertexNormal;
			glNormal3f( vNormal.x, vNormal.y, vNormal.z);
			glVertex3f(	mVLists[idx1].m_fVertexPosition.x, 
						mVLists[idx1].m_fVertexPosition.y,
						mVLists[idx1].m_fVertexPosition.z);	


			vNormal = mVLists[idx2].m_fVertexNormal;
			glNormal3f( vNormal.x, vNormal.y, vNormal.z); 
			glVertex3f(	mVLists[idx2].m_fVertexPosition.x, 
						mVLists[idx2].m_fVertexPosition.y,
						mVLists[idx2].m_fVertexPosition.z);	


			vNormal = mVLists[idx3].m_fVertexNormal;
			glNormal3f( vNormal.x, vNormal.y, vNormal.z); 
			glVertex3f(	mVLists[idx3].m_fVertexPosition.x, 
						mVLists[idx3].m_fVertexPosition.y,
						mVLists[idx3].m_fVertexPosition.z);	

		}
		else 
		{
			glNormal3f( mFaces[index].m_fFaceNormal.x, 
						mFaces[index].m_fFaceNormal.y, 
						mFaces[index].m_fFaceNormal.z);
			glVertex3f(	mVLists[idx1].m_fVertexPosition.x, 
						mVLists[idx1].m_fVertexPosition.y,
						mVLists[idx1].m_fVertexPosition.z);	
			glVertex3f(	mVLists[idx2].m_fVertexPosition.x, 
						mVLists[idx2].m_fVertexPosition.y,
						mVLists[idx2].m_fVertexPosition.z);	
			glVertex3f(	mVLists[idx3].m_fVertexPosition.x, 
						mVLists[idx3].m_fVertexPosition.y,
						mVLists[idx3].m_fVertexPosition.z);	
		}
		
	}
	glEnd(); 

}

void Mesh::RenderWireMesh()
{
	glDisable(GL_LIGHTING);
	glColor3f(1, 0, 0);
	glLineWidth(1.0f);
	
	glBegin(GL_LINES);
	for(auto& edge : mEdges)
	{
		Vertex *pV1 = &mVLists[edge.m_nVertexIdx.x];
		Vertex *pV2 = &mVLists[edge.m_nVertexIdx.y];

		glVertex3f( pV1->m_fVertexPosition.x, pV1->m_fVertexPosition.y, pV1->m_fVertexPosition.z);
		glVertex3f( pV2->m_fVertexPosition.x, pV2->m_fVertexPosition.y, pV2->m_fVertexPosition.z);
	}
	glEnd();


}

void Mesh::RenderVertexNormal()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < (int)mVLists.size(); i++)
	{
		Vertex *pV = &mVLists[i];
		glm::vec3 vp = pV->m_fVertexPosition;
		glm::vec3 vn = pV->m_fVertexNormal;
		vn.x += vp.x;
		vn.y += vp.y;
		vn.z += vp.z;

		glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f( vp.x, vp.y, vp.z);
		glVertex3f( vn.x, vn.y, vn.z);
	}
	
	glEnd();
}

void Mesh::RenderFaceNormal()
{
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < (int)mFaces.size(); i++)
	{
		glm::vec3 faceCenter;
		glm::vec3 v1_p = mVLists[mFaces[i].m_nVertexIdx.x].m_fVertexPosition;
		glm::vec3 v2_p = mVLists[mFaces[i].m_nVertexIdx.y].m_fVertexPosition;
		glm::vec3 v3_p = mVLists[mFaces[i].m_nVertexIdx.z].m_fVertexPosition;
		faceCenter.x = (v1_p.x + v2_p.x + v3_p.x) / 3.0f;
		faceCenter.y = (v1_p.y + v2_p.y + v3_p.y) / 3.0f;
		faceCenter.z = (v1_p.z + v2_p.z + v3_p.z) / 3.0f;

		glm::vec3 fn = mFaces[i].m_fFaceNormal;
		fn.x += faceCenter.x;
		fn.y += faceCenter.y;
		fn.z += faceCenter.z;

		glVertex3f( faceCenter.x, faceCenter.y, faceCenter.z);
		glVertex3f( fn.x, fn.y, fn.z);
	}
	
	glEnd();
}


void Mesh::RenderEdgeNormal()
{
	glLineWidth(1.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	glBegin(GL_LINES);
	for(int i = 0; i < (int)mEdges.size(); i ++)
	{
		Vertex *pV1 = &mVLists[mEdges[i].m_nVertexIdx.x];
		Vertex *pV2 = &mVLists[mEdges[i].m_nVertexIdx.y];
		glm::vec3 centerPoint = pV1->m_fVertexPosition + pV2->m_fVertexPosition;
		centerPoint.x /= 2.0f;
		centerPoint.y /= 2.0f;
		centerPoint.z /= 2.0f;

		glm::vec3 eN = mEdges[i].m_fEdgeNormal;
		eN.x += centerPoint.x;
		eN.y += centerPoint.y;
		eN.z += centerPoint.z;

		glVertex3f( centerPoint.x, centerPoint.y, centerPoint.z);
		glVertex3f( eN.x, eN.y, eN.z);
	}

	glEnd();
}

void Mesh::PrintMeshInformation(char* meshName)
{
	printf(" # Complete Load Mesh \n");
	printf("     - Mesh : %s \n", meshName);
	printf("     - Number of Vertex : %d \n ", m_nNumberOfVertex);
	printf("     - Number of Face : %d \n ", m_nNumberOfFace);
	printf("     - Number of Edge : %d \n ", m_nNumberOfEdge);

}
