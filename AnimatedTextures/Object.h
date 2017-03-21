#pragma once


#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"

#include "GL/glew.h"
#include "GL/freeglut.h"



using namespace std;

class Object {
	protected:
		vec2 scale; 
		float orientation;
		vec2 position;
		GLuint shaderProgram;
		string shaderName;
		vec2 velocity;
		float angularSpeed;

	public:
	
		Object(const string& name) :scale(1.0, 1.0),
									orientation(0), 
									shaderName(name),
									velocity(0.0,0.0),
									angularSpeed(0.0){}

		void Draw(int windowWidth, int windowHeight) {
			
			// define the MVPTransform here as mat4 according to scale, orientation, and position
			mat4 MVPTransform(1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			// define scale, rotation, and translation matrices separately and multiply them
			mat4 scale(scale.x, 0, 0, 0,
				0, scale.y, 0, 0,
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

			MVPTransform = scale*rotate*translate*fit;

			glUseProgram(ShaderManager::GetShader(shaderName));
			
			// set GPU uniform matrix variable MVP with the content of CPU variable MVPTransform
			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "MVP");
			// set uniform variable MVP to the MVPTransform
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVPTransform);
			else cout <<"uniform MVPTransform cannot be set"<<endl;
			DrawModel();
		}

		Object* Scale(const vec2& s) {
			scale *= s;
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

		void move(float dt) {
			position = position + velocity*dt;
			orientation = orientation + angularSpeed*dt;
		}

		virtual void DrawModel() = 0;

};