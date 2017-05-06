#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"
#include "Material.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

using namespace std;

class Object {
	protected:
		vec3 position, scaling, velocity;
		float orientation, angularVelocity;
		unsigned int shader;
		Material* material;

	public:
		Object(const string& name) : scaling(1.0, 1.0, 1.0), orientation(0.0), angularVelocity(0.0){
			shader = ShaderManager::GetShader(name);
			material = new Material(vec3(0.4, 0.4, 0.4), vec3(0.6, 0.6, 0.6), vec3(0.3, 0.3, 0.3),200);
		}

		Object* Scale(float sx, float sy, float sz) {
			scaling.x = sx;
			scaling.y = sy;
			scaling.z = sz;
			return this;
		}
		virtual void SetTransform(Camera* C) {
			mat4 scale(
				scaling.x, 0, 0, 0,
				0, scaling.y, 0, 0,
				0, 0, scaling.z, 0,
				0, 0, 0, 1);

			mat4 scaleInv(
				1.0 / scaling.x, 0, 0, 0,
				0, 1.0 / scaling.y, 0, 0,
				0, 0, 1.0 / scaling.z, 0,
				0, 0, 0, 1);

			float alpha = orientation / 180 * M_PI;

			mat4 rotate(
				cos(alpha), 0, sin(alpha), 0,
				0, 1, 0, 0,
				-sin(alpha), 0, cos(alpha), 0,
				0, 0, 0, 1);

			mat4 rotateInv(
				cos(alpha), 0, -sin(alpha), 0,
				0, 1, 0, 0,
				sin(alpha), 0, cos(alpha), 0,
				0, 0, 0, 1);

			mat4 translate(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				position.x, position.y, position.z, 1);

			mat4 translateInv(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				-position.x, -position.y, -position.z, 1);

			mat4 M = scale * rotate * translate;

			mat4 MInv = translateInv * rotateInv * scaleInv;

			int location = glGetUniformLocation(shader, "MInv");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MInv);
			else printf("uniform MInv cannot be set\n");

			mat4 MVP = M * C->GetViewMatrix() * C->GetProjectionMatrix();

			location = glGetUniformLocation(shader, "MVP");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVP);
			else printf("uniform MVP cannot be set\n");

			location = glGetUniformLocation(shader, "M");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, M);
			else printf("uniform M cannot be set\n");
		}

		
		void SetShadowTransform(Camera* C,unsigned int shaderID) {
			mat4 scale(
				scaling.x, 0, 0, 0,
				0, scaling.y, 0, 0,
				0, 0, scaling.z, 0,
				0, 0, 0, 1);

			float alpha = orientation / 180 * M_PI;

			mat4 rotate(
				cos(alpha), 0, sin(alpha), 0,
				0, 1, 0, 0,
				-sin(alpha), 0, cos(alpha), 0,
				0, 0, 0, 1);


			mat4 translate(
				1, 0, 0, 0,
				0, 1, 0, 0,
				0, 0, 1, 0,
				position.x, position.y, position.z, 1);


			mat4 M = scale * rotate * translate;

			mat4 VP = C->GetViewMatrix() * C->GetProjectionMatrix();
			
			int location = glGetUniformLocation(shaderID, "VP");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, VP);
			else printf("uniform VP cannot be set\n");

			location = glGetUniformLocation(shaderID, "M");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, M);
			else printf("uniform M cannot be set\n");
		}

		virtual void Draw(Light* light, Camera* camera, TextureCube* Env) {
			glUseProgram(shader);
			Env->Bind(shader);
			SetTransform(camera);
			light->setLight(shader);
			camera->SetEyePosition(shader);
			material->setMaterial(shader);
			DrawModel();
		}

		void Draw(Light* light, Camera* camera) {
			glUseProgram(shader);
			SetTransform(camera);
			light->setLight(shader);
			camera->SetEyePosition(shader);
			material->setMaterial(shader);
			DrawModel();
		}
		
		
		void DrawShadow(Light* light, Camera* camera, const string& shadowShader) {
			unsigned int shaderID = ShaderManager::GetShader(shadowShader);
			glUseProgram(shaderID);
			SetShadowTransform(camera, shaderID);

			vec4 l = light->getPointLightPos();
			mat4 ShadowMat( l.v[1], 0, 0, 0,
							l.v[0]*-1.0, 0, -1.0*l.v[2], -1,
							0, 0, l.v[1], 0,
							0, 0.01, 0, l.v[1]);

			int location = glGetUniformLocation(shaderID, "ShadowMat");
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, ShadowMat);
			else printf("uniform ShadowMat cannot be set\n");
			DrawModel();
		}


		void setObjMaterial(Material * m) {
			material = m;
		}

		void setOrientation(float angle) {
			orientation = angle;
		}
		void setAngularSpeed(float v) {
			angularVelocity = v;
		}
		virtual void DrawModel() = 0;

		virtual void Move(float dt) {
			position = position + velocity * dt;
			orientation = orientation + angularVelocity * dt;
		}

		virtual void Control() {

		}

};