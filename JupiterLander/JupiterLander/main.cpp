#include <iostream>
#include <time.h>
#include <string>
#include <vector>

#include "GL\glew.h"
#include "GL\freeglut.h"

#include "ShaderManager.h"
#include "Maths.h"
#include "Object.h"
#include "Triangle.h"
#include "Quads.h"
#include "Texture.h"
#include "TexturedQuads.h"
#include "Scene.h"
#include <Windows.h>


using namespace std;

Scene* s;

int windowWidth = 1600;
int windowHeight = 900;

void renderScene() {
	Sleep(4);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0); // RGBAlpha
	
	
	s->Draw(windowWidth, windowHeight);
	glutSwapBuffers();
}



void Init() {

	// load shaders
	ShaderManager::CreateProgram("TexturedObj", "vertex_texture.glsl", "fragment_texture.glsl");
	ShaderManager::CreateProgram("Explosion", "vertex_texture.glsl", "fragment_explosion.glsl");
	ShaderManager::CreateProgram("Transparent", "vertex_texture.glsl", "fragment_transparent.glsl");
	ShaderManager::CreateProgram("Jovian", "vertex_texture.glsl", "fragment_jovian.glsl");
	ShaderManager::CreateProgram("GameOver", "vertex_texture.glsl", "fragment_gameOver.glsl");

	glEnable(GL_DEPTH_TEST | GL_BLEND);
	s = new Scene();
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void closeCallback() {
	delete s;
	glutLeaveMainLoop();
}

void onIdle() {
	static double lastTime = 0.0;

	double currTime = glutGet(GLUT_ELAPSED_TIME) * 0.001;
	double dt = currTime - lastTime;
	lastTime = currTime;

	s->Move(dt);
	glutPostRedisplay();
}

void onResize(int w, int h) {
	windowHeight = h;
	windowWidth = w;
	glViewport(0, 0, w, h);
}

void keyPress(unsigned char c, int x, int y) {
	if (!s->gameOver()) {
		s->press(tolower(c));
	}
	else if (tolower(c) == ' ') {
		s = new Scene();
	}
	
	//glutPostRedisplay();
}

void keyRelease(unsigned char c, int x, int y) {
	if (!s->gameOver()) {
		s->release(tolower(c));
	}
}

void mouseClicks(int button, int state, int x, int y) {
	if (!s->gameOver() && button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		s->Fire(x, y,windowHeight,windowWidth);
		s->MouseHeldDown();
	}
	else{
		s->MouseReleased();
	}
}

void mouseDir(int x, int y) {
	s->mouseDir(x, y, windowWidth, windowHeight);
}

int main(int argc, char **argv) {
	
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// set up position and size of window
	glutInitWindowPosition(150, 25);
	glutInitWindowSize(windowWidth, windowHeight);
	
	glutCreateWindow("Jupiter Lander");
	glewInit();
	Init();
	
	// register display funtion
	glutDisplayFunc(renderScene);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(keyPress);
	glutKeyboardUpFunc(keyRelease);
	glutMouseFunc(mouseClicks);
	//glutPassiveMotionFunc(mouseDir);
	glutMotionFunc(mouseDir);
	glutIdleFunc(onIdle);
	glutCloseFunc(closeCallback);
	
	glutMainLoop();
		
	return 0;
}