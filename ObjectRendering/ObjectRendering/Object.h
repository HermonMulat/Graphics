#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"

#include "GL/glew.h"
#include "GL/freeglut.h"



using namespace std;

class Object {
protected:
	vec2 scaling; 
	float orientation;
	vec2 position;
	GLuint shaderProgram;
	string shaderName;

public:
	
	Object(const string& name) : 
		scaling(1.0, 1.0), 
		orientation(double(rand())), 
		shaderName(name) {}

	void Draw(int windowWidth, int windowHeight) {
		// define the MVPTransform here as mat4 according to scaling, orientation, and position
		mat4 MVPTransform(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		// define scaling, rotation, and translation matrices separately and multiply them
		mat4 scale(scaling.x, 0, 0, 0,
			0, scaling.y, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		mat4 translate(1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			position.x, position.y, 0, 1);

		float alpha = cos(orientation);
		float beta = sin(orientation);
		mat4 rotate(alpha, beta, 0, 0,
			-beta, alpha, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		mat4 fit(1, 0, 0, 0,
			0, float(windowWidth) / windowHeight, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);

		MVPTransform = rotate*scale*translate*fit;

		// set GPU uniform matrix variable MVP with the content of CPU variable MVPTransform
		int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "MVP");
		// set uniform variable MVP to the MVPTransform
		if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVPTransform);
		else cout <<"uniform MVPTransform cannot be set"<<endl;
		DrawModel();
	}

	Object* Scale(const vec2& s) {
		scaling = scaling*s;
		return this;
	}
	Object* Rotate(float angle) {
		orientation += angle;
		return this;
	}
	Object* Translate(const vec2& t) {
		position += t;
		return this;
	}

	virtual void DrawModel() = 0;

};