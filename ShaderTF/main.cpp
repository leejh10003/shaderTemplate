#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

#include <Windows.h>
#include <iostream>
#include <..\ShaderTF\gl\glew.h>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "glm\glm.hpp"
#include "glm\ext.hpp"

#include "Engine.h"

using namespace std;

//Window Size
int						WINDOW_WIDTH;
int						WINDOW_HEIGHT;
//Camera
float					g_Zoom;				// view zoom
float					g_Rotate[2];		// x,y
float					g_Translate[2];		// x,y
//Mouse
int						g_Mouse_Coord[2];	// previous mouse coordinates
unsigned char			g_Mouse_Event[3];	// mouse event handler

Engine		g_Engine;

glm::mat4 viewMat;
glm::mat4 projMat;
glm::mat4 modelMat;

int width;
int height;

void Print_CameraInfo()
{
	cout << "Zoom : "		<< g_Zoom			<< endl;
	cout << "Rotate : "		<< g_Rotate[0]		<< " "		<< g_Rotate[1]		<< endl;
	cout << "Translate : "	<< g_Translate[0]	<< " "		<< g_Translate[1]	<< endl;
	cout << endl;
}

void Initialize(void)
{
	//To do : Initialize Code
	g_Engine.Initialize();

	//Console Window 위치 변경
	HWND hWnd = ::FindWindow("ConsoleWindowClass" ,NULL );
	::MoveWindow(hWnd , 0 , 0 , 500 , 500, TRUE );

	g_Zoom = 33.8f;

	for(int i=0; i<2; i++)
	{
		g_Rotate[i] = 0.0f;
		g_Translate[i] = 0.0f;
		g_Mouse_Coord[i] = 0.0f;
	}

	g_Translate[0] = -0.3f;
	g_Translate[1] = 1.8f;
	g_Rotate[0]	   = 2.0f;
	g_Rotate[1]	   = -0.4f;

	g_Mouse_Event[0] = g_Mouse_Event[1] = g_Mouse_Event[2] = 0;

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.2f, 1.0f );
}

void Render(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glm::vec3 eyeVec = glm::vec3(0.0, 0.0, 1.0);
	glm::vec3 lookAtVec = glm::vec3(0.0, 0.0, 0.0);
	glm::vec3 upVec = glm::vec3(0.0, 1.0, 0.0);

	viewMat = glm::lookAt(eyeVec, lookAtVec, upVec);
	projMat = glm::perspective(45.0f, (float)width/height, 0.1f, 500.0f);
	modelMat = glm::mat4(1.0f);

	glPushMatrix();
		glTranslatef(0.0, 0.0, -g_Zoom);
		glTranslatef(g_Translate[0], g_Translate[1], 0.0);
		glRotatef(g_Rotate[0], 1.0, 0.0, 0.0);
		glRotatef(g_Rotate[1], 0.0, 1.0, 0.0);
	
		modelMat = glm::translate(modelMat, 0.0f, 0.0f, -g_Zoom);
		modelMat = glm::translate(modelMat, g_Translate[0], g_Translate[1], 0.0f);
		modelMat = glm::rotate(modelMat,  g_Rotate[0], glm::vec3(1.0, 0.0, 0.0));
		modelMat = glm::rotate(modelMat,  g_Rotate[1], glm::vec3(0.0, 1.0, 0.0));

		//To do :: Rendering Scene
		g_Engine.Render(viewMat, modelMat, projMat);
	glPopMatrix();

	glutSwapBuffers();
}

void Reshape(int w, int h)
{
	width = w;
	height = h;

	if(w==0) 
	{
		h = 1;
	}
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (float)w/h, 0.1, 500);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void Mouse(int mouse_event, int state, int x, int y)
{
	g_Mouse_Coord[0] = x;
	g_Mouse_Coord[1] = y;

	switch(mouse_event)
	{
	case GLUT_LEFT_BUTTON:
		g_Mouse_Event[0] = ((GLUT_DOWN==state)?1:0);
		break;
	case GLUT_MIDDLE_BUTTON:
		g_Mouse_Event[1] = ((GLUT_DOWN==state)?1:0);
		break;
	case GLUT_RIGHT_BUTTON:
		g_Mouse_Event[2] = ((GLUT_DOWN==state)?1:0);
		break;
	default:
		break;		
	}

	glutPostRedisplay();
}


void Motion(int x, int y)
{
	int diffx=x-g_Mouse_Coord[0];
	int diffy=y-g_Mouse_Coord[1];

	g_Mouse_Coord[0] = x;
	g_Mouse_Coord[1] = y;

	if(g_Mouse_Event[0])
	{
		g_Rotate[0] += (float) 0.1 * diffy;
		g_Rotate[1] += (float) 0.1 * diffx;
	}
	else if(g_Mouse_Event[1])
	{
		g_Translate[0] += (float) 0.1 * diffx;
		g_Translate[1] -= (float) 0.1 * diffy;
	}
	else if(g_Mouse_Event[2])
	{
		g_Zoom += (float) 0.1 * diffy;
	}

	glutPostRedisplay();
}


void Keyboard(unsigned char key, int x, int y)
{
	//To do : Keyboard Input
	g_Engine.Key(key, x, y);

	switch(key) 
	{
	
		case VK_ESCAPE:
			exit(0);
			break;
		case VK_RETURN:

			break;
	}

	glutPostRedisplay();
}

void SpecialKey(int key, int x, int y)
{
	//To do : Keyboard Input
	switch(key) 
	{
		case GLUT_KEY_F1:
			Print_CameraInfo();
			break;
	}

	glutPostRedisplay();
}

void Idle(void)
{
	g_Engine.Update();
	glutPostRedisplay();
}


int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	WINDOW_WIDTH = 800;
	WINDOW_HEIGHT = 800;

	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	glutInitWindowPosition(501, 0);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutCreateWindow("Physics GPU : Position based Fluids");

	Initialize();

	glutDisplayFunc(Render);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutIdleFunc(Idle);

	

	glutMainLoop();

	return 0;
}

