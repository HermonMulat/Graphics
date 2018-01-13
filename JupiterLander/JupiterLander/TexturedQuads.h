#pragma once
#define _USE_MATH_DEFINES
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


class TexturedQuad : public Object
{
	protected:
		unsigned int vao;	// vertex array object id
		Texture* objTexture;

	public:
		TexturedQuad(Texture* t, const string& shaderName) : Object(shaderName), objTexture(t){
			name = TEX_QUAD;
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

		TexturedQuad(Texture* t, const string& shaderName, float* vertexCoords, float* textureCoord, int size) :
			Object(shaderName), objTexture(t) {
			name = TEX_QUAD;
			glGenVertexArrays(1, &vao);	// create a vertex array object
			glBindVertexArray(vao);		// make it active

			unsigned int vbo[2];
			glGenBuffers(2, &vbo[0]);

			//  vertex cord setup
			glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
			glBufferData(GL_ARRAY_BUFFER, size, vertexCoords, GL_STATIC_DRAW);
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);

			// Texture mapping
			glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
			glBufferData(GL_ARRAY_BUFFER, size, textureCoord, GL_STATIC_DRAW);
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

		virtual void check() {
			/*
			float dx = (scale.x / 2.0)*0.5;
			float dy = (scale.y / 2.0)*0.5;

			if (position.x > 1.0 - dx || position.x < -1.0 + dx) {
				velocity.x *= -1;
			}
			if (position.y > 1.0 - dy || position.y < -1.0 + dy) {
				velocity.y *= -1;
			}*/
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


/*class EphemeralNoFad : public TexturedQuad {
	private:
		int LifeTime;
		int currentAge;
	public:
		EphemeralNoFad(Texture *t, const string& shader) : TexturedQuad(t, shader) {
			LifeTime = 60;
			currentAge = 0;
			angularSpeed = (double)rand() / RAND_MAX;
			orientation = ((double)rand() / RAND_MAX) * 2 * M_PI;
		}
		EphemeralNoFad(Texture *t, const string& shader, int age) : TexturedQuad(t, shader) {
			LifeTime = age;
			currentAge = 0;
			angularSpeed = ((double)rand() / RAND_MAX);
			orientation = ((double)rand() / RAND_MAX) * 2 * M_PI;
		}
		void check() {
			currentAge++;
			alive = (currentAge < LifeTime);
		}
		void setLifeTime(int age) {
			LifeTime = age;
		}
};*/

class Jovian :public TexturedQuad {
	private:
		int subTexture;
	public:
		Jovian(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			name = JOVIAN;
			scale = vec2(0.7, 0.7);
			subTexture = 0;
		}

		void DrawModel() {
			static int motion = 0;

			objTexture->Bind(shaderName);

			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "subTexture");
			if (location >= 0) glUniform1i(location, subTexture);
			else cout << "uniform SubTexture cannot be set" << endl;
			if (subTexture < 140) {
				if (motion % 6 == 0) {
					subTexture++;
				}
				
				motion++;
			}

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisable(GL_BLEND);

		}

		void check() {
			alive = (subTexture < 140);
		}
};

class Explosion :public TexturedQuad{
	private:
		int subTexture;
	public:
		Explosion(Texture* t, const string& shaderName) : TexturedQuad(t, shaderName) {
			name = EXPLOSION;
			scale = vec2(0.7, 0.7);
			subTexture = 0;
		}

		void DrawModel() {
			//check();
			/*static double lastTime = 0.0;
			double currTime = glutGet(GLUT_ELAPSED_TIME) * 0.001;
			double dt = currTime - lastTime;
			lastTime = currTime;
			cout << dt << " total " << currTime << endl;*/
			objTexture->Bind(shaderName);

			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "subTexture");
			if (location >= 0) glUniform1i(location, subTexture);
			else cout << "uniform SubTexture cannot be set" << endl;
			if (subTexture < 35) subTexture++;
		
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisable(GL_BLEND);

		}

		void check() {
			alive = (subTexture < 34);
		}
};

class Ephemeral : public TexturedQuad {
	private:
		int LifeTime;
		int currentAge;
		int Youth; // time before it starts fadding
	public:
		Ephemeral(Texture *t, const string& shader): TexturedQuad(t,shader){
			LifeTime = 45;
			currentAge = 0;
			Youth = 0;
			angularSpeed = (double)rand() / RAND_MAX;
			orientation = ((double)rand() / RAND_MAX)*2*M_PI;
		}
		Ephemeral(Texture *t, const string& shader, int age,int youth) : TexturedQuad(t, shader) {
			LifeTime = age;
			Youth = youth;
			currentAge = 0;
			angularSpeed = ((double)rand() / RAND_MAX);
			orientation = ((double)rand() / RAND_MAX) * 2 * M_PI;
		}
		void check() {
			currentAge++;
			alive = (currentAge < LifeTime);
		}
		void setLifeTime(int age) {
			LifeTime = age;
		}

		void setVelocity(const vec2& velo) {
			velocity = velo;
		}
		
		void DrawModel() {
			objTexture->Bind(shaderName);

			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "Transparency");
			float transparency = 1;
			if (currentAge > Youth) {
				transparency = (LifeTime - currentAge)*1.0 / (LifeTime - Youth);
			}

			if (location >= 0) glUniform1f(location, transparency);
			else cout << "uniform SubTexture cannot be set" << endl;

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glBindVertexArray(vao);
			glDrawArrays(GL_QUADS, 0, 4);
			glDisable(GL_BLEND);

		}
};

