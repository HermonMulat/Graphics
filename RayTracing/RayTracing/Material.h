#pragma once
#include "vec3.h"
#include "Noise.h"
#include "LightSource.h"
#include <algorithm>
#include <math.h>

using namespace std;
class Material {
	protected:
		vec3 color1, color2;

	public:
		float reflectance;
		bool Reflect;
		Material() : color1(1, 1, 0), color2(1, 0, 0), reflectance(0), Reflect(false) {}
		Material(vec3 c1) : color1(c1), color2(c1), reflectance(0), Reflect(false) {}
		Material(vec3 c1,float r) : color1(c1), color2(c1), reflectance(r), Reflect(true){}
		Material(vec3 f, vec3 b) : color1(f), color2(b), reflectance(0), Reflect(false) {}

		virtual vec3 getColor(vec3 position) {
			return color1;
		}
 
		virtual vec3 shade(vec3 position, vec3 normal, LightSource* light) {
			vec3 M = light->getPowerDensityAt(position);
			float dotVal = max(0.0f,normal.dot(light->getLightDirAt(position)));  
			return M*getColor(position)*dotVal;
		}
};


class Wood : public Material {
	float scale;
	float turbulence;
	float period;
	float sharpness;

public:
	Wood() :
		Material(vec3(1, 0.3, 0), vec3(0.35, 0.1, 0.05)) {
		scale = 48;
		turbulence = 600;
		period = 32;
		sharpness = 10;
	}
	
	virtual vec3 getColor(vec3 position) {
		float w = position.x * period + pow(snoise(position * scale), sharpness)*turbulence + 10000.0;
		w -= int(w);
		return (color1 * w + color2 * (1 - w));
	}
};


class Marble : public Material {
	float scale;
	float turbulence;
	float period;
	float sharpness;
public:
	Marble() :
		Material(vec3(0, 0, 1), vec3(1, 1, 1)) {
		scale = 96;
		turbulence = 100;
		period = 96;
		sharpness = 1;
	}
	virtual vec3 getColor(vec3 position) {
		float w = position.x * period + pow(snoise(position * scale), sharpness)*turbulence;
		w = pow(sin(w)*0.5 + 0.5, 4);
		return (color1 * w + color2 * (1 - w));
	}
};

class BeachBall : public Material {
	vec3 color3, color4;
public:
	BeachBall() :
		Material(vec3(1, 0, 0), vec3(1, 1, 0)) {
		color3 = vec3(1, 1, 1);
		color4 = vec3(1, 1, 0);
	}
	// partition 3d space into 8 partitions based on the standard x-y,x-z and y-z planes
	virtual vec3 getColor(vec3 position) {
		float z = position.z + 2.0;
		float x = position.x + 0.75; 
		int angle = abs(floor(atan(z/ x) * 180/M_PI));
		int freqency = 40;
		if ((angle%freqency)*2 > freqency) {
			return color1;
		}
		else {
			return color2;
		}
	}
};