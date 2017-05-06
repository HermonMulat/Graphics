#define _CRT_SECURE_NO_WARNINGS
#define _USE_MATH_DEFINES
#pragma once
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <iostream>

#include "Maths.h" 
#include "Mesh.h" 
#include "Camera.h"
#include "Texture.h"
#include "Object.h"
#include "MeshInstance.h"
#include "Scene.h"
#include "ShaderManager.h"

#if defined(__APPLE__)
#include <GLUT/GLUT.h>
#include <OpenGL/gl3.h>
#include <OpenGL/glu.h>
#else
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
#include <windows.h>
#endif
#include <GL/glew.h>		 
#include <GL/freeglut.h>	
#endif

 

using namespace std;

unsigned int windowWidth = 800, windowHeight = 800;

// OpenGL major and minor versions
int majorVersion = 4, minorVersion = 5;

Scene scene;

void onInitialization() {
	scene = Scene();
	glViewport(0, 0, windowWidth, windowHeight);
	scene.Initialize();
}

void onKeyboard(unsigned char key, int x, int y){
	scene.KeyPress(tolower(key));
	glutPostRedisplay();
}

void onKeyboardUp(unsigned char key, int x, int y){
	scene.KeyRelease(tolower(key));
	glutPostRedisplay();
}

void onExit() {
	ShaderManager::ClearShaders();
	printf("exit");
}

void onDisplay() {

	glClearColor(0, 0.3, 0.5, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	scene.Draw();

	glutSwapBuffers();
}

void onReshape(int winWidth0, int winHeight0)
{
	glViewport(0, 0, winWidth0, winHeight0);

	windowWidth = winWidth0, windowHeight = winHeight0;

	scene.SetAspectRatio((float)windowWidth / windowHeight);
}


void onIdle() {
	double t = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	static double lastTime = 0.0;
	double dt = t - lastTime;
	lastTime = t;

	scene.Control();
	scene.Move(dt);
	scene.Draw();

	glutPostRedisplay();
}


int main(int argc, char * argv[]) {
	srand(time(0));
	glutInit(&argc, argv);
#if !defined(__APPLE__)
	glutInitContextVersion(majorVersion, minorVersion);
#endif
	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition(100, 100);
#if defined(__APPLE__)
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH | GLUT_3_2_CORE_PROFILE);
#else
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
#endif
	glutCreateWindow("3D Mesh");

#if !defined(__APPLE__)
	glewExperimental = true;
	glewInit();
#endif

	printf("GL Vendor    : %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer  : %s\n", glGetString(GL_RENDERER));
	printf("GL Version (string)  : %s\n", glGetString(GL_VERSION));
	glGetIntegerv(GL_MAJOR_VERSION, &majorVersion);
	glGetIntegerv(GL_MINOR_VERSION, &minorVersion);
	printf("GL Version (integer) : %d.%d\n", majorVersion, minorVersion);
	printf("GLSL Version : %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	onInitialization();

	glutDisplayFunc(onDisplay);
	glutReshapeFunc(onReshape);
	glutKeyboardFunc(onKeyboard);
	glutKeyboardUpFunc(onKeyboardUp);
	glutIdleFunc(onIdle);

	glutMainLoop();
	onExit();
	return 1;
}
