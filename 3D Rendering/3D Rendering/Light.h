#pragma once
#include "Maths.h"
#include "GL\glew.h"
#include "GL\freeglut.h"

class Light {
	protected:
		vec3 La, Le;
		vec4 worldLightPosition;

	public:
		Light(vec3 la, vec3 le) {
			La = la;
			Le = le;
		}

		void SetPointLightSource(vec3& pos) {
			worldLightPosition.v[0] = pos.x;
			worldLightPosition.v[1] = pos.y;
			worldLightPosition.v[2] = pos.z;
			worldLightPosition.v[3] = 1;
		}

		void SetDirectionalLightSource(vec3& dir) {
			worldLightPosition.v[0] = dir.x;
			worldLightPosition.v[1] = dir.y;
			worldLightPosition.v[2] = dir.z;
			worldLightPosition.v[3] = 0;
		}

		vec4 getPointLightPos() {
			return worldLightPosition;
		}

		void MoveLight(const vec3& pos) {
			worldLightPosition.v[0] += pos.x;
			worldLightPosition.v[1] += pos.y;
			worldLightPosition.v[2] += pos.z;
		}

		void MoveLight(float x, float y, float z) {
			worldLightPosition.v[0] += x;
			worldLightPosition.v[1] += y;
			worldLightPosition.v[2] += z;
		}

		void setLight(int shaderID) {
			int location = glGetUniformLocation(shaderID, "La");
			if (location >= 0) glUniform3fv(location, 1, &(La.x));
			else printf("uniform La cannot be set\n");

			location = glGetUniformLocation(shaderID, "Le");
			if (location >= 0) glUniform3fv(location, 1, &(Le.x));
			else printf("uniform Le cannot be set\n");

			location = glGetUniformLocation(shaderID, "worldLightPosition");
			if (location >= 0) glUniform4fv(location, 1, &(worldLightPosition.v[0]));
			else printf("uniform worldLightPosition cannot be set\n");

		}
};
