// Always include glew before freeglut
#include <iostream>
#include <vector>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "ShaderManager.h"
#include "Maths.h"
#include "Object.h"
#include "Triangle.h"
#include "Quads.h"



using namespace std;



ShaderManager* programs;
vector<Object*> myObjects;


bool showTriangle = true;

int windowWidth = 800;
int windowHeight = 800;

void renderScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 1); // RGBAlpha

	for (int i = 0; i < myObjects.size(); i++) {
		myObjects[i]->Draw(windowWidth, windowHeight);
	}
	
	glutSwapBuffers();
}

void closeCallback(){
	delete programs;
	glutLeaveMainLoop();
}

void Init() {

	glEnable(GL_DEPTH_TEST);

	// load and compile shaders
	programs->CreateProgram("SimpleObject", "vertexShader.glsl", "fragmentShader.glsl");
	programs->CreateProgram("BlueObject", "vertexShader.glsl", "fs_blue.glsl");

	// test create 4 random objects

	myObjects.push_back(new Triangle("SimpleObject"));
	myObjects.push_back(new Triangle("BlueObject"));

	myObjects.push_back(new Quad("SimpleObject"));
	myObjects.push_back(new Quad("BlueObject"));


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void onIdle() {
	
	myObjects[0]->Scale(vec2(0.995, 0.995));
	myObjects[1]->Translate(vec2(0.001, 0));
	myObjects[2]->Translate(vec2(0, -0.001));
	
	glutPostRedisplay();
}

int main(int argc, char **argv) {
	
	srand(time(NULL));

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);

	// set up position and size of window
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(windowWidth, windowHeight);
	
	glutCreateWindow("Triangle Rendering");
	glewInit();
	Init();
	

	// register display funtion
	glutDisplayFunc(renderScene);
	glutIdleFunc(onIdle);
	glutCloseFunc(closeCallback);
	
	glutMainLoop();
		
	return 0;
}