#pragma once
#include "Maths.h"
#include "GL\glew.h"
#include "GL\freeglut.h"
class Material {
	protected:
		vec3 ka, kd, ks;
		float shininess;


	public:
		Material() {
			ka = vec3(0.1, 0.1, 0.1);
			kd = vec3(0.9, 0.9, 0.9);
			ks = vec3(0.0, 0.0, 0.0);
			shininess = 0;
		}

		Material(vec3 a, vec3 d, vec3 s, float shine) {
			ka = a;
			kd = d;
			ks = s;
			shininess = shine;
		}

		void setMaterial(int shaderID) {
			int location = glGetUniformLocation(shaderID, "ka");
			if (location >= 0) glUniform3fv(location, 1, &(ka.x));
			else printf("uniform ka cannot be set\n");


			location = glGetUniformLocation(shaderID, "kd");
			if (location >= 0) glUniform3fv(location, 1, &(kd.x));
			else printf("uniform kd cannot be set\n");


			location = glGetUniformLocation(shaderID, "ks");
			if (location >= 0) glUniform3fv(location, 1, &(ks.x));
			else printf("uniform ks cannot be set\n");

			location = glGetUniformLocation(shaderID, "shininess");
			if (location >= 0) glUniform1f(location, shininess);
			else printf("uniform shininess cannot be set\n");
		}
};