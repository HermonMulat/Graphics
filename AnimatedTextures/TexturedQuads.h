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

#include "Texture.h"
#include "Object.h"


class TexturedQuad :public Object
{
	protected:
		unsigned int vao;	// vertex array object id
		Texture* objTexture;

	public:
		TexturedQuad(Texture* t, const string& shaderName) : Object(shaderName), objTexture(t){
			glGenVertexArrays(1, &vao);	// create a vertex array object
			glBindVertexArray(vao);		// make it active

			unsigned int vbo[2];
			glGenBuffers(2, &vbo[0]);

			//  vertex cord setup
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			float vertexCoords[] = { -0.25,0.25,
									  0.25,0.25,
									  0.25,-0.25,
									 -0.25,-0.25 };
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertexCoords), vertexCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			// Texture mapping
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			float textureCoord[] = { 0,0, 1,0, 1,1, 0,1 };
			glBufferData(GL_ARRAY_BUFFER, sizeof(textureCoord), textureCoord, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

		}

		void DrawModel() {

			objTexture->Bind(shaderName);
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisable(GL_BLEND );

		}
};

class Asteroid : public TexturedQuad{
	public:
		Asteroid(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			scale = vec2(0.3, 0.3);
			orientation = (float)rand() / RAND_MAX * 360.0;
			position = vec2::random() * 0.3;
			velocity = vec2::random() * 0.3;
			angularSpeed = ((float)rand() / RAND_MAX) * 5.0;
		}
};

class Bullet : public TexturedQuad {
	public:
		Bullet(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			scale = vec2(0.2, 0.2);
		}



		void setPosition(float ori) {
			position.x = cos(ori)*0.125;
			position.y = sin(ori)*0.125;
		}

		void setVelocity(const vec2& velo) {
			velocity = velo;
		}

};

class Cannon : public TexturedQuad{
	public:
		Cannon(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			scale = vec2(0.52, 1.0);
			angularSpeed = 1.0;
		}

		vec2 GetBulletVelocity() {
			return (vec2(cos(orientation),sin(orientation)))*3.0;
		}

		float GetOrientation() {
			return orientation;
		}
};

class Explosion :public TexturedQuad{
	public:
		Explosion(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			scale = vec2(0.8, 0.8);
		}

		void DrawModel() {

			objTexture->Bind(shaderName);

			static int subtex = 0;
			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "subTexture");
			if (location >= 0) glUniform1i(location, subtex);
			else cout << "uniform SubTexture cannot be set" << endl;
			if (subtex < 35) subtex++;
		
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisable(GL_BLEND);

		}
};
