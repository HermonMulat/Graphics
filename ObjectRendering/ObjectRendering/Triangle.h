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

class Triangle :public Object
{
	unsigned int vao;	// vertex array object id

public:
	Triangle(const string& shaderName): Object(shaderName) {
	

		glGenVertexArrays(1, &vao);								// create a vertex array object
		glBindVertexArray(vao);									// make it active

		unsigned int vbo[2];									// vertex buffer object
		glGenBuffers(2, &vbo[0]);								// generate a vertex buffer object

																// vertex coordinates: vbo -> Attrib Array 0 -> vertexPosition of the vertex shader

		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);					// make it active, it is an array

		static float vertexCoords[] = { 0.25,-0.25,				// vertex data on the CPU
										-0.25, -0.25,
										0.25, 0.25 };		

		glBufferData(GL_ARRAY_BUFFER,							// copy to the GPU
			sizeof(vertexCoords),								// size of the vbo in bytes
			vertexCoords,										// address of the data array on the CPU
			GL_STATIC_DRAW);									// copy to that part of the memory which is not modified

		// map Attribute Array 0 to the currently bound vertex buffer (vbo)
		glEnableVertexAttribArray(0);

		// data organization of Attribute Array 0
		glVertexAttribPointer(0,								// Attribute Array 0
			2, GL_FLOAT,										// components/attribute, component type
			GL_FALSE,											// not in fixed point format, do not normalized
			0, NULL);											// stride and offset: it is tightly packed

		//  same thing for color
		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		static float vertexColors[] = { 1,0,0, 0,1,0, 0,0,1 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	}


	void DrawModel(){
		glUseProgram(ShaderManager::GetShader(shaderName));
		glBindVertexArray(vao);									// make the vao and its vbos active playing the role of the data source
		glDrawArrays(GL_TRIANGLES, 0, 3);						// draw a single triangle with vertices defined in vao
	}
};
