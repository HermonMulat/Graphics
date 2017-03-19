#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "Texture.h"
#include "Object.h"


class TexturedQuad :public Object
{
	unsigned int vao;	// vertex array object id
	Texture* objTexture;

public:
	TexturedQuad(Texture* t, const string& shaderName) : Object(shaderName), objTexture(t){
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
		static float textureCoord[] = { 0,0, 1,0, 1,1, 1,0 };
		glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoord), textureCoord, GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

	}

	void DrawModel() {

		glUseProgram(ShaderManager::GetShader(shaderName));
		objTexture->Bind(shaderName);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBindVertexArray(vao);
		glDrawArrays(GL_QUADS, 0, 4);
		glDisable(GL_BLEND);

	}
};

class Asteroid : public TexturedQuad{
	public:
		Asteroid(Texture* t, const string& shaderName) : TexturedQuad(t,shaderName){
			scale = vec2(0.3, 0.3);
			orientation = (float)rand() / RAND_MAX * 360.0;
			position = vec2::random();
		}
};