#include "Engine.h"


Engine::Engine(void)
{
}


Engine::~Engine(void)
{
	DeleteShader();
	mCapture.ReleaseMemory();
}

void Engine::DeleteShader()
{
	
}


void Engine::SupportVersionCheck()
{
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	   /* Problem: glewInit failed, something is seriously wrong. */
		printf(" Load Fail : GLEW Library \n");
		cerr<< glewGetErrorString(err)<<endl;
	}
	else
		printf(" Ready for GLEW Library \n");

	if( glewIsSupported("GL_VERSION_2_0"))
		printf(" Ready for OpenGL 2.0 \n");
	else 
	{
		printf(" OpenGL 2.0 is not supported \n");
		exit(1);
	}
}

void Engine::Initialize()
{
	SupportVersionCheck();
	
	m_toonShader.CreateShader("toon.vert", "toon.frag");
	m_impToonShader.CreateShader("toon.vert", "toon2.frag");
	
	m_bCapture = false;

	CreateScene();

}

void Engine::CreateScene()
{
	//Table
	mTableMesh.LoadMeshFromFile(".\\Meshes\\TexTable2.obj", ".\\Meshes\\Texture\\table_diffuse.tga");
	mTableMesh.ScaleMesh(glm::vec3(2.0f, 1.5f, 3.0f));
	mTableMesh.TranslateMesh(glm::vec3(0.0f, -10.1f, 0.0f));

	//Teapot Mesh
	mTeapotMesh.LoadMeshFromFile(".\\Meshes\\Teapot.obj");
	mTeapotMesh.TranslateMesh(glm::vec3(0.0f, -7.6f, 0.0f));

}


void Engine::Render(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM)
{

	m_toonShader.UseProgram();
	mTableMesh.RenderTexMesh();
	mTeapotMesh.RenderSolidMesh(VERTEX_NORMAL);
	if( m_bCapture == true)
	{
		mCapture.BeginCapture();
	}
}

void Engine::Update()
{
	
}

void Engine::Key(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'c':
			if( m_bCapture == false )
				mCapture.CreateFolder();
			m_bCapture = !m_bCapture;
			break;

	}
}