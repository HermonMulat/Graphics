#pragma once

#include <algorithm>
#include "vec3.h"

using namespace std;

class LightSource {
public:
	virtual vec3 getPowerDensityAt(vec3 x) = 0;
	virtual vec3 getLightDirAt(vec3 x) = 0;
	virtual float  getDistanceFrom(vec3 x) = 0;
};

class DirectionalLight : public LightSource {
	vec3 powerDensity, lightDir;
public:
	DirectionalLight(vec3 powerD, vec3 dir) : powerDensity(powerD), lightDir(dir) {}

	vec3 getPowerDensityAt(vec3 x) {
		return powerDensity;
	}

	vec3 getLightDirAt(vec3 x) {
		return lightDir*-1.0;
	}

	float getDistanceFrom(vec3) {
		return FLT_MAX;
	}
};

class PointLight : public LightSource {
	vec3 powerDensity, lightPos;
public:
	PointLight(vec3 powerD, vec3 pos) : powerDensity(powerD), lightPos(pos) {}
	vec3 getPowerDensityAt(vec3 x) {
		float distFromLight = (lightPos - x).norm();
		return powerDensity*(1 / distFromLight);
	}
	vec3 getLightDirAt(vec3 x) {
		return (lightPos - x).normalize();
	}

	float getDistanceFrom(vec3 x) {
		return (lightPos - x).norm();
	}
};
