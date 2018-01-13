#pragma once
#include "vec3.h"

class Camera {
	vec3 eye;		//< world space camera position
	vec3 lookAt;	//< center of window in world space
	vec3 right;		//< vector from window center to window right-mid (in world space)
	vec3 up;		//< vector from window center to window top-mid (in world space)

public:
	Camera() {
		eye = vec3(0, 0, 5);
		lookAt = vec3(0, 0, 0);
		right = vec3(1, 0, 0);
		up = vec3(0, 1, 0);
	}
	vec3 getEye() {
		return eye;
	}
	// compute ray through pixel at normalized device coordinates
	vec3 rayDirFromNdc(float x, float y) {
		return ((lookAt - eye)  + right * x + up * y).normalize();
	}
};