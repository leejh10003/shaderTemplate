#pragma once

#include <iostream>

#include "textfile.h"
#include <..\ShaderTF\gl\glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

class CShader
{
public:
	CShader(void);
	~CShader(void);

public:
	GLuint			verts;
	GLuint			frag;
	GLuint			program;
	
public:
	void	CreateShader(char *vertsFile, char *fragFile);
	void	UseProgram(){ glUseProgram(program); }
	GLuint	getLocation(char *name){ return glGetUniformLocation(program, name);}

	void	PrintShaderInfoLog(GLuint obj);
	void	PrintProgramInfoLog(GLuint p);
};

