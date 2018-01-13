#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>			// standard c++ library 
#include "Maths.h"			// file containg def for vec2,vec3,math4 ... 
#include "ShaderManager.h"
#include "Texture.h"
#include "Object.h"

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "TexturedQuads.h"

using namespace std;

class Lander {
	private:
		Object* body;
		vector<Object*> burners;
		vector<bool> burnerStatus;
		bool landing,takeoff;
	public:
		Lander(){
			landing = false;
			takeoff = false;
		}

		vec2 getVelocity() {
			return body->getVelocity();
		}
		vec2 getPosition() {
			return body->GetPosition();
		}

		float getOrientation() {
			return body->getOrientation();
		}

		float OrientationVariance() {
			float angle = body->getOrientation() * 180.0 / M_PI;

		}


		float getAngular() {
			return body->getAngular();
		}
		
		vec2 getScale() {
			return body->GetScale();
		}
		void addBody(Texture* t, const string& shader) {
			body = new TexturedQuad(t, shader);
		}

		void addBurner(Texture* t,const string& shader,float* burnerVert,float* burnerText, int size,float dx, float dy) {
			Object* burner = new TexturedQuad(t, shader, burnerVert, burnerText, size);
			mat4 preMat = mat4();
			preMat.m[3][0] = dx; preMat.m[3][1] = dy;
			burner->setPreMatrix(preMat);
			burners.push_back(burner);
			burnerStatus.push_back(false);
		}

		void addDrag(float dt) {
			float dragFactor = exp(-0.35 * dt);
			body->scaleVelocity(dragFactor, dragFactor);
			body->scaleAngularSpeed(dragFactor);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->scaleVelocity(dragFactor, dragFactor);
				burners[i]->scaleAngularSpeed(dragFactor);
			}
		}


		void setVelocity(float vx, float vy) {
			body->setVelocity(vx, vy);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->setVelocity(vx,vy);
			}
		}
		void setVelocity(const vec2& v) {
			body->setVelocity(v);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->setVelocity(v);
			}
		}

		void addVelocity(float vx, float vy) {
			body->addVelocity(vx, vy);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->addVelocity(vx, vy);
			}
		}

		void addVelocity(const vec2& v) {
			body->addVelocity(v);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->addVelocity(v);
			}
		}

		void setPosition(float x, float y) {
			body->setPosition(x, y);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->setPosition(x, y);
			}
		}

		void setAngular(float a) {
			body->setAngular(a);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->setAngular(a);
			}
		}
		void addAngular(float a) {
			body->addAngular(a);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->addAngular(a);
			}
		}


		Lander* Scale(float sx, float sy) {
			body->Scale(sx, sy);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->Scale(sx, sy);
			}
			return this;
		}

		Lander* Scale(const vec2& s) {
			body->Scale(s);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->Scale(s);
			}
			return this;
		}

		Lander* Translate(const vec2& t) {
			body->Translate(t);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->Translate(t);
			}
			return this;
		}

		Lander* Rotate(float angle) {
			body->Rotate(angle);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->Rotate(angle);
			}
			return this;
		}
		bool isAlive() {
			return body->isAlive();
		}
		void kill() {
			body->kill();
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->kill();
			}

		}
		void land() {
			setVelocity(0, 0);
			setAngular(0);
			body->setOrientation(0);
			for (int i = 0; i < burners.size(); i++) {
				burners[i]->setOrientation(0);
			}
			landing = true;
			
		}


		void Draw(int width, int height) {
			static int frame_count = 0;
			// draw lander body
			body->Draw(width, height);
			// now draw burnners
			for (int i = 0; i < burners.size(); i++) {
				if (burnerStatus[i]) {
					burners[i]->Draw(width,height);
				}
			}

			// every 10 frames the afterburnner turns off  - burnners dont disapper immediately 
			if (frame_count % 2 == 0) {
				burnerStatus[0] = false;
				burnerStatus[1] = false;
				burnerStatus[2] = false;
			}
			frame_count++;
		}

		void addGravity() {
			vec2 G = vec2(0.0, -0.002);
			addVelocity(G);
		}

		void move(float dt) {
			if (!landing) {
				addDrag(dt);
				addGravity();
				body->move(dt);
				for (int i = 0; i < burners.size(); i++) {
					burners[i]->move(dt);
				}
			}

		}
		bool takeOff() {
			return takeoff ;
		}
		void noLongerTakingOff() {
			takeoff = false;
		}

		void up() {
			if (landing) {
				landing = false;
				takeoff = true;
			}
			
			float acceleration = 0.006;
			float theta = body->getOrientation() + M_PI/2.0;
			addVelocity(vec2(acceleration*cos(theta), acceleration*sin(theta)));
			burnerStatus[0] = true;
		}

		void right() {
			
			float theta = body->getOrientation();
			addVelocity(vec2(0.001*cos(theta), 0.001*sin(theta)));
			addAngular(-0.1);
			burnerStatus[2] = true;  
		}

		void left() {
			float theta = body->getOrientation();
			addVelocity(vec2(-0.001*cos(theta), -0.001*sin(theta)));
			addAngular(0.1);
			burnerStatus[1] = true;
		}

};