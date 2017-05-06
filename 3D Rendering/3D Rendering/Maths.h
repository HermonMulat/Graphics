#pragma once
#define _USE_MATH_DEFINES
#include <stdlib.h>
#include <string>
#include <iostream>
#include <math.h>

using namespace std;
// row-major matrix 4x4
struct mat4
{
	float m[4][4];
	public:
		mat4() {
			m[0][0] = 1; m[0][1] = 0; m[0][2] = 0; m[0][3] = 0;
			m[1][0] = 0; m[1][1] = 1; m[1][2] = 0; m[1][3] = 0;
			m[2][0] = 0; m[2][1] = 0; m[2][2] = 1; m[2][3] = 0;
			m[3][0] = 0; m[3][1] = 0; m[3][2] = 0; m[3][3] = 1;
		}
		mat4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33)
		{
			m[0][0] = m00; m[0][1] = m01; m[0][2] = m02; m[0][3] = m03;
			m[1][0] = m10; m[1][1] = m11; m[1][2] = m12; m[1][3] = m13;
			m[2][0] = m20; m[2][1] = m21; m[2][2] = m22; m[2][3] = m23;
			m[3][0] = m30; m[3][1] = m31; m[3][2] = m32; m[3][3] = m33;
		}

		mat4 operator*(const mat4& right)
		{
			mat4 result;
			for (int i = 0; i < 4; i++)
			{
				for (int j = 0; j < 4; j++)
				{
					result.m[i][j] = 0;
					for (int k = 0; k < 4; k++) result.m[i][j] += m[i][k] * right.m[k][j];
				}
			}
			return result;
		}
		operator float*() { 
			return &m[0][0]; 
		}
};


// 3D point in homogeneous coordinates
struct vec4
{
	float v[4];

	vec4(float x = 0, float y = 0, float z = 0, float w = 1){
		v[0] = x; v[1] = y; v[2] = z; v[3] = w;
	}

	vec4 operator*(const mat4& mat){
		vec4 result;
		for (int j = 0; j < 4; j++){
			result.v[j] = 0;
			for (int i = 0; i < 4; i++) result.v[j] += v[i] * mat.m[i][j];
		}
		return result;
	}

	vec4 operator+(const vec4& vec){
		vec4 result(v[0] + vec.v[0], v[1] + vec.v[1], v[2] + vec.v[2], v[3] + vec.v[3]);
		return result;
	}
};

// 2D point in homogeneous coordinates (or 3D point in cartisian)
struct vec3
{
	float x, y, z;

	vec3(float x = 0.0, float y = 0.0, float z = 0.0) : x(x), y(y), z(z) {}

	static vec3 random() { return vec3(((float)rand() / RAND_MAX) * 2 - 1, ((float)rand() / RAND_MAX) * 2 - 1, ((float)rand() / RAND_MAX) * 2 - 1); }

	vec3 operator+(const vec3& v) { return vec3(x + v.x, y + v.y, z + v.z); }

	vec3 operator-(const vec3& v) { return vec3(x - v.x, y - v.y, z - v.z); }

	vec3 operator*(float s) { return vec3(x * s, y * s, z * s); }

	vec3 operator/(float s) { return vec3(x / s, y / s, z / s); }

	float length() { return sqrt(x * x + y * y + z * z); }

	vec3 normalize() { return *this / length(); }

	void print() { printf("%f \t %f \t %f \n", x, y, z); }

	vec3 cross(const vec3& b){
		return vec3(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
	}
};



// 2D point in Cartesian coordinates
struct vec2
{
	float x, y;
	// default constructor initializes a random vector in NDC
	vec2() {
		x = 0;
		y = 0;
	}

	double length() {
		return pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
	}

	void normalize() {
		double length = pow(pow(x, 2.0) + pow(y, 2.0), 0.5);
		x /= length;
		y /= length;
	}
	static vec2 random() {
		double px = ((double)rand() / (RAND_MAX)) * 2 - 1;
		double py = ((double)rand() / (RAND_MAX)) * 2 - 1;
		return vec2(px, py);
	}

	vec2(float x, float y) : x(x), y(y) {}

	vec2 operator+(const vec2& v){
		return vec2(x + v.x, y + v.y);
	}

	vec2 operator-(const vec2& v) {
		return vec2(x - v.x, y - v.y);
	}


	vec2 operator+= (const vec2& v) {
		x += v.x;
		y += v.y;
		return *this;
	}

	vec2 operator*(const float s) {
		return vec2(x*s, y*s);
	}

	vec2 operator*(const vec2& v) {
		return vec2(x*v.x, y*v.y);
	}

	vec2 operator*=(const vec2& v) {
		x = v.x;
		y = v.y;
		return *this;
	}

	float Orientation() {
		return atan2(y , x);
	}

	int OrientationAngle() {
		return static_cast<int>(atan2(y, x)*180.0/M_PI);
	}
};