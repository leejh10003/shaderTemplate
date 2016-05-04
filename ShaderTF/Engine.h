#pragma once

#include <Windows.h>
#include <iostream>
#include <..\ShaderTF\gl\glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include"ScreenCapture.h"
#include "Shader.h"
#include "Mesh.h"
#include "textfile.h"

#include "glm\glm.hpp"
#include "glm\ext.hpp"

using namespace std;

class Engine
{
public:
	Engine(void);
	~Engine(void);

public:
	void SupportVersionCheck();
	void Initialize();
	void Render(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM);
	void Update();
	void Key(unsigned char key, int x, int y);

public:
	//Simulation 
	float			m_lightPosDt;
	float			m_lightPos[4];
	Mesh			mLampMesh;
	Mesh			mPlaneMesh;
	Mesh			mBulbMesh;
	Mesh			mTableMesh;
	Mesh			mTeapotMesh;
	Mesh			mSphereMesh[8];

	//Screen Capture
	bool			m_bCapture;
	ScreenCapture	mCapture;

	//Shader
	CShader			m_toonShader;
	CShader			m_impToonShader;
	CShader			m_waveShader;
	CShader			m_lightShader;
	CShader			m_normalMapShader;
	CShader			m_lambertShader;

	float			m_fAngle;
	float			m_fTime;

	void	DeleteShader();
	
public:
	void	SphereScene();
	void	CreateScene();

};

