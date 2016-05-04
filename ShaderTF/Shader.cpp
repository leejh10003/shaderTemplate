#include "Shader.h"

CShader::CShader(void)
{
}


CShader::~CShader(void)
{
}

void CShader::CreateShader(char *vertsFile, char *fragFile)
{
	char *vSource;
	char *fSource;

	//Vertex Shader
	verts = glCreateShader(GL_VERTEX_SHADER);
	vSource = textFileRead(vertsFile);

	const char *vs = vSource;
	glShaderSource(verts, 1, &vs, NULL);
	free(vSource);

	//Fragment Shader
	frag = glCreateShader(GL_FRAGMENT_SHADER);
	fSource = textFileRead(fragFile);

	const char *fs = fSource;
	glShaderSource(frag, 1, &fs, NULL);
	free(fSource);

	//Create Program
	program = glCreateProgram();
	
	//Compile Shaders
	glCompileShader(verts);
	glAttachShader(program, verts);

	glCompileShader(frag);
	glAttachShader(program, frag);


	glLinkProgram(program);

	PrintShaderInfoLog(verts);
	PrintShaderInfoLog(frag);
	PrintProgramInfoLog(program);

}


void CShader::PrintShaderInfoLog(GLuint obj)
{
	int infologLength = 0;
	int charsWritten  = 0;

	char *infoLog;

	glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &infologLength);

	if (infologLength > 0)
	{
		printf("----------------------------------- \n");
		printf("Shader Error ! \n");
		printf("----------------------------------- \n");

		infoLog = (char *)malloc(infologLength);
		glGetShaderInfoLog(obj, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);

		free(infoLog);
	}
}

void CShader::PrintProgramInfoLog(GLuint p)
{
	int infologLength = 0;
	int charsWritten  = 0;
	char *infoLog;

	glGetProgramiv(p, GL_INFO_LOG_LENGTH,&infologLength);

	if (infologLength > 0)
	{
		printf("----------------------------------- \n");
		printf("Program Error ! \n");
		printf("----------------------------------- \n");

		infoLog = (char *)malloc(infologLength);
		glGetProgramInfoLog(p, infologLength, &charsWritten, infoLog);
		printf("%s\n",infoLog);

		free(infoLog);
	}
}
