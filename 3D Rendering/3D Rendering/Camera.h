#pragma once

#include "Maths.h"
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "Light.h"

class Camera {
	vec3  wEye, wLookat, wVup, velocity;
	float fov, asp, fp, bp, angularSpeed;

	public:
		Camera() {
			velocity = vec3(0.0, 0.0, 0.0);
			angularSpeed = 0;
			wEye = vec3(0.0, 0.4, 1.0);
			wLookat = vec3(0.0, 0.4, 0.0);
			wVup = vec3(0.0, 1.0, 0.0);
			fov = M_PI / 2.0; asp = 1.0; fp = 0.01; bp = 10.0;
		}

		vec3 GetEyePos() {
			return wEye;
		}
		void SetAspectRatio(float a) { asp = a; }

		// view matrix
		mat4 GetViewMatrix() {
			vec3 w = (wEye - wLookat).normalize();
			vec3 u = wVup.cross(w).normalize();
			vec3 v = w.cross(u);

			return
				mat4(
					1.0f, 0.0f, 0.0f, 0.0f,
					0.0f, 1.0f, 0.0f, 0.0f,
					0.0f, 0.0f, 1.0f, 0.0f,
					-wEye.x, -wEye.y, -wEye.z, 1.0f) *
				mat4(
					u.x, v.x, w.x, 0.0f,
					u.y, v.y, w.y, 0.0f,
					u.z, v.z, w.z, 0.0f,
					0.0f, 0.0f, 0.0f, 1.0f);
		}

		// projection matrix
		mat4 GetProjectionMatrix() {
			float sy = 1 / tan(fov / 2);
			return mat4(
				sy / asp, 0.0f, 0.0f, 0.0f,
				0.0f, sy, 0.0f, 0.0f,
				0.0f, 0.0f, -(fp + bp) / (bp - fp), -1.0f,
				0.0f, 0.0f, -2 * fp*bp / (bp - fp), 0.0f);
		}

		mat4 GetInverseViewMatrix() {
			vec3 w = (wEye - wLookat).normalize();
			vec3 u = wVup.cross(w).normalize();
			vec3 v = w.cross(u);
			return mat4(
				u.x, u.y, u.z, 0.0f,
				v.x, v.y, v.z, 0.0f,
				w.x, w.y, w.z, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f);
		}
		mat4 GetInverseProjectionMatrix() {
			float sy = 1 / tan(fov / 2);
			return mat4(
				asp / sy, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0 / sy, 0.0f, 0.0f,
				0.0f, 0.0f, 0.0f, (fp - bp) / 2.0 / fp / bp,
				0.0f, 0.0f, -1.0f, (fp + bp) / 2.0 / fp / bp);
		}


		void Up() {
			vec3 ahead = (wLookat - wEye);
			ahead = ahead.normalize();
			ahead = ahead * 0.1;
			wEye = wEye + ahead;
			wLookat = wLookat + ahead;	//modify eye just like lookAt so the distance between them will always be the same
			//light.SetPointLightSource(wEye);
		}

		void Down() {
			vec3 ahead = (wLookat - wEye);
			ahead = ahead.normalize();
			ahead = ahead * -0.1;
			wEye = wEye + ahead;
			wLookat = wLookat + ahead;
			//light.SetPointLightSource(wEye);
		}

		void Left() {
			vec3 ahead = (wLookat - wEye);
			ahead = ahead.normalize(); 
			vec3 right = ahead.cross(wVup);
			float angle = (M_PI / 1800.0) * 5;
			vec3 ahead2 = ahead * cos(angle) - right * sin(angle);
			ahead2 = ahead2 * ahead.length();
			wLookat = wEye + ahead2;
		}

		void Right() {
			vec3 ahead = (wLookat - wEye);
			ahead = ahead.normalize();
			vec3 right = ahead.cross(wVup);
			float angle = (M_PI / 1800.0) * 5;
			vec3 ahead2 = ahead * cos(angle) + right * sin(angle);
			ahead2 = ahead2 * ahead.length();
			wLookat = wEye + ahead2;
		}

		void Move() {
			wEye = wEye + (velocity * 0.5);
			wLookat = wLookat + (velocity * 0.5);
		}
		
		void SetEyePosition(unsigned int shaderID) {
			//set worldEyePos in shader to camera position
			int location = glGetUniformLocation(shaderID, "worldEye");
			if (location >= 0) glUniform3fv(location, 1, &(wEye.x));
			else printf("uniform worldEye cannot be set\n");
		}

};
