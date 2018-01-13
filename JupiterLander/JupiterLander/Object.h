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



using namespace std;
enum object_t {OBJ,QUAD,TRI,EXPLOSION,PLASMA,PLATFORM,LANDER,TEX_QUAD,POKEBALL,POKEMON,JOVIAN,DIAMOND,METEOR};
class Object {
	protected:
		
		vec2 scale;
		object_t name;
		float orientation;
		vec2 position;
		string shaderName;
		vec2 velocity;
		float angularSpeed;
		bool alive;
		mat4 preMatrix;
		mat4 postMatrix;

	public:
	
		Object(const string& name) :scale(1.0, 1.0),
									orientation(0), 
									shaderName(name),
									velocity(0.0,0.0),
									angularSpeed(0.0),
									alive(1),
									preMatrix(mat4()),
									postMatrix(mat4()),
									name(OBJ)
									{}
		
		void virtual setVelocity(const vec2& v) {
			velocity = v;
		}

		float getAngular() {
			return angularSpeed;
		}
		vec2 getVelocity() {
			return velocity;
		}

		void setName(object_t n) {
			name = n;
		}
		void virtual setVelocity(float dx, float dy) {
			velocity.x = dx;
			velocity.y = dy;
		}

		void virtual scaleVelocity(float dx, float dy) {
			velocity.x *= dx;
			velocity.y *= dy;
		}

		void virtual scaleAngularSpeed(float s) {
			angularSpeed *= s;
		}

		void virtual addVelocity(const vec2& v) {
			velocity.x += v.x;
			velocity.y += v.y;
		}
		void virtual addVelocity(float dx, float dy) {
			velocity.x += dx;
			velocity.y += dy;
		}

		float getOrientation() {
			return orientation;
		}

		void virtual setOrientation(float o) {
			orientation = o;
		}

		void virtual setPosition(float x, float y) {
			position.x = x;
			position.y = y;
		}

		void virtual setAngular(float ang) {
			angularSpeed = ang;
		}

		void virtual addAngular(float ang) {
			angularSpeed += ang;
		}


		void Draw(int windowWidth, int windowHeight) {
			check();
			// define the MVPTransform here as mat4 according to scale, orientation, and position
			mat4 MVPTransform(1, 0, 0, 0,
							  0, 1, 0, 0,
							  0, 0, 1, 0,
							  0, 0, 0, 1);

			// define scale, rotation, and translation matrices separately and multiply them
			mat4 scale(scale.x, 0, 0, 0,
					   0, scale.y, 0, 0,
					   0, 0, 1, 0,
					   0, 0, 0, 1);

			mat4 translate(1, 0, 0, 0,
						   0, 1, 0, 0,
						   0, 0, 1, 0,
						   position.x, position.y, 0, 1);

			float alpha = cos(orientation);
			float beta = sin(orientation);

			mat4 rotate(alpha, beta, 0, 0,
				-beta, alpha, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			mat4 fit(1, 0, 0, 0,
				0, float(windowWidth) / windowHeight, 0, 0,
				0, 0, 1, 0,
				0, 0, 0, 1);

			// rotation about the center of object
			MVPTransform = preMatrix*scale*rotate*translate*ShaderManager::GetCam()*postMatrix*fit;


			glUseProgram(ShaderManager::GetShader(shaderName));
			
			// set GPU uniform matrix variable MVP with the content of CPU variable MVPTransform
			int location = glGetUniformLocation(ShaderManager::GetShader(shaderName), "MVP");
			// set uniform variable MVP to the MVPTransform
			if (location >= 0) glUniformMatrix4fv(location, 1, GL_TRUE, MVPTransform);
			else cout <<"uniform MVPTransform cannot be set"<<endl;
			DrawModel();
		}

		virtual Object* Scale(const vec2& s) {
			scale *= s;
			return this;
		}
		virtual Object* Scale(float a, float b) {
			scale.x *= a;
			scale.y *= b;
			return this;
		}
		virtual Object* Rotate(float angle) {
			orientation += angle;
			return this;
		}
		virtual Object* Translate(const vec2& t) {
			position += t;
			return this;
		}

		void setPreMatrix(const mat4& pre) {
			preMatrix = pre;
		}
		void setPostMatrix(const mat4& post) {
			postMatrix = post;
		}

		void move(float dt) {
			position = position + velocity*dt;
			orientation = orientation + angularSpeed*dt;
		}

		virtual void DrawModel() = 0;

		virtual void check() {
		}

		bool isAlive() {
			return alive;
		}
		void kill() {
			alive = false;
		}

		object_t what() {
			return name;
		}

		vec2 GetPosition() {
			return position;
		}

		vec2 GetScale() {
			return scale;
		}


};