// Always include glew before freeglut

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



using namespace std;

Scene* s;

int windowWidth = 800;
int windowHeight = 800;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0); // RGBAlpha
	
	s->Draw(windowWidth, windowHeight);

	glutSwapBuffers();
}



void Init() {

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
	if (c == ' ') s->Fire();
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// set up position and size of window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	
	glutCreateWindow("Textured Astroids");
	glewInit();
	Init();
	
	// register display funtion
	glutDisplayFunc(renderScene);
	glutReshapeFunc(onResize);
	glutKeyboardFunc(keyPress);
	glutIdleFunc(onIdle);
	glutCloseFunc(closeCallback);
	
	glutMainLoop();
		
	return 0;
}