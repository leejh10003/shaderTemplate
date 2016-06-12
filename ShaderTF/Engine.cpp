#include "Engine.h"
#include "stb_image.h"


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
	
	/*m_toonShader.CreateShader("toon.vert", "toon.frag");
	m_impToonShader.CreateShader("toon.vert", "toon2.frag");*/
	mIronmanShader.CreateShader("ironman.vert", "ironman.frag");
	m_bCapture = false;

	CreateScene();

}

void Engine::CreateScene()
{
	////Table
	//mTableMesh.LoadMeshFromFile(".\\Meshes\\TexTable2.obj", ".\\Meshes\\Texture\\table_diffuse.tga");
	//mTableMesh.ScaleMesh(glm::vec3(2.0f, 1.5f, 3.0f));
	//mTableMesh.TranslateMesh(glm::vec3(0.0f, -10.1f, 0.0f));

	////Teapot Mesh
	//mTeapotMesh.LoadMeshFromFile(".\\Meshes\\Teapot.obj");
	//mTeapotMesh.TranslateMesh(glm::vec3(0.0f, -7.6f, 0.0f));
	mIronMan.LoadMeshFromFile(".\\Meshes\\ironman.obj");
	mIronManDiffuse = LoadTexImage(".\\Meshes\\Texture\\ironman_d.tga");
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mIronManDiffuse);

	mIronmanSpecular = LoadTexImage(".\\Meshes\\Texture\\ironman_s.tga");
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, mIronmanSpecular);

	mIronMan.ScaleMesh(glm::vec3(2.0f, 2.0f, 2.0f));
	mIronMan.TranslateMesh(glm::vec3(0.0f, -10.1f, 0.0f));

	return;
}


void Engine::Render(glm::mat4 viewM, glm::mat4 modelM, glm::mat4 projM)
{

	mIronmanShader.UseProgram();
	/*GLuint diffuseTex = mIronmanShader.getLocation("myTexture");
	GLuint specTex = mIronmanShader.getLocation("specularTex");
	glUniform1i(diffuseTex, mIronManDiffuse);
	glUniform1f(specTex, mIronmanSpecular);*/
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
GLuint Engine::LoadTexImage(char* texFile)
{
	GLuint texture;
	FILE* fp = fopen(texFile, "rb");
	if (!fp) { printf("ERROR: No %s.\n fail to bind %d\n", texFile, texture); return false; }

	int width, height, channel;
	unsigned char *image = stbi_load_from_file(fp, &width, &height, &channel, 4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, GL_MODULATE);

	return texture;
}