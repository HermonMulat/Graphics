#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Object.h"

class Quad :public Object
{
	unsigned int vao;	// vertex array object id

public:
	Quad(const string& shaderName) : Object(shaderName) {
		glGenVertexArrays(1, &vao);	// create a vertex array object
		glBindVertexArray(vao);		// make it active

		unsigned int vbo[2];
		glGenBuffers(1, &vbo[0]);

		//  vertex cord setup
		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		static float vertexCoords[] = { -0.25,0.25, 
										0.25,0.25, 
										0.25,-0.25, 
										-0.25,-0.25 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		// color of vertex setup
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		static float vertexColors[] = { 0,0,0, 1,0,0, 0,1,0, 0,0,1 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	}

	void DrawModel(){
		glUseProgram(ShaderManager::GetShader(shaderName));
		glBindVertexArray(vao);	// make the vao and its vbos active playing the role of the data source
		glDrawArrays(GL_QUADS, 0, 4); // draw a single rectangle with vertices defined in vao
	}
};