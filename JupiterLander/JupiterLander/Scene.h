#pragma once
#define _USE_MATH_DEFINES
#include "Object.h"
#include "Triangle.h"
#include "Quads.h"
#include "Texture.h"
#include "TexturedQuads.h"
#include "ShaderManager.h"
#include "MixedObjects.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Scene {
	private:
		int lives;
		float MouseDir;
		int diamondsCollected, fireCoolDown, DiamondFallFactor;
		bool flamethrower, weHaveaMeteor, weHaveaJovian, MouseDown;
		map<string,Texture*> textures;
		vector<Object*> objects;
		Lander* lander;
		Object* Heart;
		Object* Diamond;
		Object* GameOver;

		map<unsigned char, bool> keyPresses;

	public:
		Scene() {
			keyPresses['w'] = false;
			keyPresses['a'] = false;
			keyPresses['s'] = false;

			MouseDown = false;
			weHaveaMeteor = false;
			weHaveaJovian = false;
			flamethrower = false;

			DiamondFallFactor = 1;
			fireCoolDown = 0;
			MouseDir = 0;
			lives = 3;
			diamondsCollected = 0;
			// add and load texture
			textures["AfterBurnner"] = new Texture("media/afterburner.png");
			textures["Platform"] = new Texture("media/platform.png");
			textures["PlatformEnd"] = new Texture("media/platformend.png");
			textures["Boom"] = new Texture("media/boom.png");
			textures["Lander"] = new Texture("media/lander.png");
			textures["Diamond"] = new Texture("media/diamond.png");
			textures["Plasma"] = new Texture("media/plasma.png");
			textures["PokeBall"] = new Texture("media/pokeball.png");
			textures["FlameThrower"] = new Texture("media/fire_ball.png");
			textures["Pokemon"] = new Texture("media/skuntank.png");
			textures["Jovians"] = new Texture("media/jovian.png");
			textures["Heart"] = new Texture("media/heart.png");
			textures["Meteor"] = new Texture("media/meteor.png");
			textures["GameOver"] = new Texture("media/GameOver.png");

			Heart = (new TexturedQuad(textures["Heart"], "TexturedObj"))->Scale(0.1, 0.1);
			Diamond = (new TexturedQuad(textures["Diamond"], "TexturedObj"))->Scale(0.08, 0.08);

			GameOver = (new TexturedQuad(textures["GameOver"], "GameOver"))->Scale(vec2(4,4));
			Object* pokemon = (new TexturedQuad(textures["Pokemon"], "TexturedObj"))->Scale(0.2, 0.2)->Translate(vec2(1.25, 1.25));
			pokemon->setName(POKEMON);
			objects.push_back(pokemon);

			
			createLander();
			createPlatform(1);		// left side
			createPlatform(-1);		// right side

		}
		bool gameOver() {
			return (lives < 0);
		}
		void createLander() {
			lander = new Lander();
			// main body of lander
			lander->addBody(textures["Lander"], "TexturedObj");
			// bottom afterburnner of lander
			float burnerVert[] = { -0.0625,0.125, 0.0625,0.125, 0.0625,-0.125, -0.0625,-0.125 };
			float burnerText[] = { 0,0, 0,1, 1,1, 1,0 };
			lander->addBurner(textures["AfterBurnner"], "TexturedObj", burnerVert, burnerText, sizeof(burnerText), 0.018, -0.285);
			// right afterburnner of lander
			float burnerVert2[] = { -0.125,0.0625, 0.125,0.0625, 0.125,-0.0625, -0.125,-0.0625 };
			float burnerText2[] = { 0,1, 1,1, 1,0, 0,0 };
			lander->addBurner(textures["AfterBurnner"], "TexturedObj", burnerVert2, burnerText2, sizeof(burnerText2), 0.245, 0.115);
			// left afterburnner of lander
			float burnerVert3[] = { -0.125,0.0625, 0.125,0.0625, 0.125,-0.0625, -0.125,-0.0625 };
			float burnerText3[] = { 1,1, 0,1, 0,0, 1,0 };
			lander->addBurner(textures["AfterBurnner"], "TexturedObj", burnerVert3, burnerText3, sizeof(burnerText3), -0.215, 0.105);

			lander->Scale(0.3, 0.3);
		}

		void createPlatform(float side) {
			// platform  
			Object* temp = new TexturedQuad(textures["Platform"], "TexturedObj");
			temp->Scale(0.5, 0.12);
			temp->setName(PLATFORM);
			temp->Translate(vec2(-0.6*side, -0.4));
			objects.push_back(temp); 

			// platform end 1
			temp = new TexturedQuad(textures["PlatformEnd"], "TexturedObj");
			temp->Scale(0.06, 0.12);
			temp->Translate(vec2(-0.4625*side, -0.4));
			if (side==1) temp->Rotate(M_PI);
			temp->setName(PLATFORM);
			objects.push_back(temp);

			// platform end 2
			temp = new TexturedQuad(textures["PlatformEnd"], "TexturedObj");
			temp->Scale(0.06, 0.12);
			temp->Translate(vec2(-0.7375*side, -0.4));
			if (side==-1) temp->Rotate(M_PI);
			temp->setName(PLATFORM);
			objects.push_back(temp);
		}



		void Draw(int windowWidth, int windowHeight) {
			vector<Object*> nextObjects;
			Object* pokemon = objects[0];
			static int counter = 0;
			vec2 landerPos = lander->getPosition();
			ShaderManager::SetCam(landerPos.x*-1.0, landerPos.y*-1.0);
			// random meteors
			/*
			if ((rand()*1.0) / RAND_MAX < 0.008 && !weHaveaMeteor) {
				weHaveaMeteor = true;
				Object* Meteor = (new Ephemeral(textures["Meteor"], "Transparent", 1000, 750))->Scale(0.35, 0.35);
				Meteor->Translate(vec2(-1.0, 1.0));
				Meteor->setAngular(0);
				Meteor->setOrientation(0);
				Meteor->setName(METEOR);
				Meteor->setVelocity(0.2, -0.2);
				objects.push_back(Meteor);
				
			} */

			// random diamonds
			if ((rand()*1.0 / RAND_MAX) < 0.002 * DiamondFallFactor) {
				Object* diamond = (new Ephemeral(textures["Diamond"], "Transparent",1000,500))->Scale(0.08, 0.08);
				float randx = ((rand()*1.0 / RAND_MAX) - 0.5)*2.0;
				diamond->Translate(vec2(randx, 0.75));
				diamond->setVelocity(vec2(0, -0.2));
				diamond->setOrientation(0);
				diamond->setAngular(0);
				diamond->setName(DIAMOND);
				objects.push_back(diamond);
			}

			if (!weHaveaJovian && (rand()*1.0/RAND_MAX) < 0.001) {
				Object* J = (new Jovian(textures["Jovians"], "Jovian"))->Scale(0.12, 0.12);
				J->setVelocity(vec2(0.015, 0));
				if ((rand()*1.0 / RAND_MAX) < 0.5) {
					J->Translate(vec2(0.5, -0.35));
				}
				else {
					J->Translate(vec2(-0.7, -0.35));
				}
				objects.push_back(J);
				weHaveaJovian = true;
			}

			for (int i = 0; i < objects.size(); i++){
				if (objects[i]->what() == JOVIAN && !objects[i]->isAlive()) {
					weHaveaJovian = false;
				}
				if (objects[i]->isAlive()) {
					objects[i]->Draw(windowWidth, windowHeight);
					nextObjects.push_back(objects[i]);
				}
				
				if (objects[i]->what() == POKEMON) {
					pokemon = objects[i];
				}

				if (objects[i]->what() == METEOR && lander->isAlive()) {
					vec2 c = objects[i]->GetPosition();
					if ((landerPos - c).length() < 0.23*objects[i]->GetScale().x) {
						Object* exp = (new Explosion(textures["Boom"], "Explosion"))->Translate(landerPos)->Scale(vec2(0.35, 0.35));
						nextObjects.push_back(exp);
						exp->Draw(windowWidth, windowHeight);
						lander->kill();
						lives--;
						counter = 150;
					}
				}

				if (objects[i]->what() == PLATFORM && lander->isAlive()) {
					float tolerance = 0.02;
					float realOrientation = fmod(lander->getOrientation(), 2 * M_PI);
					if (realOrientation < 0) realOrientation *= -1;
					if ( (abs(landerPos.x + 0.6)+ tolerance < 0.125+0.25*0.3+0.0125  ||
						  abs(landerPos.x - 0.6) + tolerance < 0.125 + 0.25*0.3 + 0.0125)
						  && abs(landerPos.y + 0.4)+ tolerance < 0.05 + 0.25*0.3){
						
						if (!lander->takeOff() 
							&& (realOrientation < tolerance*6)
							&& lander->getVelocity().length() < tolerance*24
							&& abs(landerPos.y + 0.3)<0.01 ) {
							lander->land();
						}
						else if (!lander->takeOff()){
							Object* exp = (new Explosion(textures["Boom"], "Explosion"))->Translate(landerPos)->Scale(vec2(0.35, 0.35));
							nextObjects.push_back(exp);
							exp->Draw(windowWidth, windowHeight);
							lander->kill();
							lives--;
							counter = 50;
						}
					}
					else {
						lander->noLongerTakingOff();
					}
					
				}
				if (objects[i]->what() == POKEBALL) {
					vec2 c1 = objects[i]->GetPosition();
					vec2 c2 = pokemon->GetPosition();
					if ((c1 - c2).length() < 0.05) {
						pokemon->kill();
						objects[i]->kill();
						flamethrower = true;
					}
				}

				if (objects[i]->what() == DIAMOND && objects[i]->isAlive()) {
					vec2 c1 = objects[i]->GetPosition();
					vec2 c2 = lander->getPosition();
					if ((c1 - c2).length() < 0.08) {
						objects[i]->kill();
						diamondsCollected++;
						if (diamondsCollected % 3 == 0) { lives++; }
					}
				}

				if (objects[i]->what() == METEOR  && !objects[i]->isAlive()) {
					weHaveaMeteor = false;
				}
			}

			int x, y;
			vec2 staticPos;
			for (int j = 0; j < (diamondsCollected); j++) {
				x = j % 5; y = j / 5;
				staticPos = landerPos + vec2(0.95, 0.5) - vec2(0.04*x, 0.04*y);
				Diamond->setPosition(staticPos.x, staticPos.y);
				Diamond->Draw(windowWidth, windowHeight);
			}

			for (int k = 0; k < lives; k++) {
				x = k % 5; y = k / 5;
				staticPos = landerPos + vec2(-0.95, 0.5) + vec2(0.044*x, -0.044*y);
				Heart->setPosition(staticPos.x, staticPos.y);
				Heart->Draw(windowWidth, windowHeight);
			}
			
			if (lander->isAlive()) {
				lander->Draw(windowWidth, windowHeight);
			}

			if (!lander->isAlive()) {
				counter--;
				if (counter <= 0 && lives>=0) {
					createLander();
				}
			}
			objects.swap(nextObjects);

			if (lives < 0) {
				DiamondFallFactor = 8;
				GameOver->setPosition(landerPos.x, landerPos.y);
				GameOver->Draw(windowWidth, windowHeight);
			}
			else {
				DiamondFallFactor = 1;
			}

		}

		void Move(double dt) {
			if (lander->isAlive()) {
				lander->move((float)dt);
			}
			
			for (int i = 0; i < objects.size(); i++) {
				objects[i]->move((float)dt);
				if (objects[i]->what() == METEOR) {
					vec2 meteorPos = objects[i]->GetPosition();
					if (meteorPos.length() < 1) {
						objects[i]->Scale(vec2(1.5-pow(meteorPos.length(),0.5), 1.5 - pow(meteorPos.length(), 0.5)));
						objects[i]->Scale(0.5, 0.5);
					}
				}
			}

			vec2 pos = lander->getPosition();
			float or = lander->getOrientation();
			Object* plasma;
			mat4 preMat;
			float randOr;

			if (keyPresses['w'] && lander->isAlive() ) {
				lander->up();
				for (int i = 0; i < 6; i++) {
					plasma = (new Ephemeral(textures["Plasma"], "Transparent"))->Scale(0.03, 0.03)->Translate(pos);
					
					randOr = ((rand()*1.0 / RAND_MAX) - 0.5)*(M_PI / 2.0) + M_PI / 2.0 ;
					preMat = mat4();
					preMat.m[3][0] = 0.018; preMat.m[3][1] = -1.6;
					plasma->setVelocity(vec2(-0.1*cos(randOr), -0.1*sin(randOr)));
					plasma->setOrientation(or);
					plasma->setAngular(lander->getAngular());

					plasma->setPreMatrix(preMat);
					objects.push_back(plasma);
				}
			}
			if (keyPresses['a'] && lander->isAlive()) {
				lander->left();
				for (int i = 0; i < 3; i++) {
					plasma = (new Ephemeral(textures["Plasma"], "Transparent"))->Scale(0.03, 0.03)->Translate(pos);

					randOr = ((rand()*1.0 / RAND_MAX) - 0.5)*(M_PI / 2.0) + M_PI ;
					preMat = mat4();
					preMat.m[3][0] = 1.215; preMat.m[3][1] = 0.8;
					plasma->setVelocity(vec2(-0.1*cos(randOr), -0.1*sin(randOr)));
					plasma->setOrientation(or);
					plasma->setAngular(lander->getAngular());

					plasma->setPreMatrix(preMat);
					objects.push_back(plasma);
				}
			}
			if (keyPresses['d'] && lander->isAlive()) {
				lander->right();
				for (int i = 0; i < 3; i++) {
					plasma = (new Ephemeral(textures["Plasma"], "Transparent"))->Scale(0.03, 0.03)->Translate(pos);

					randOr = ((rand()*1.0 / RAND_MAX) - 0.5);
					preMat = mat4();
					preMat.m[3][0] = -1.015; preMat.m[3][1] = 0.8;
					plasma->setVelocity(vec2(-0.1*cos(randOr), -0.1*sin(randOr)));
					plasma->setOrientation(or);
					plasma->setAngular(lander->getAngular());

					plasma->setPreMatrix(preMat);
					objects.push_back(plasma);
				}
			}		
			if (flamethrower && MouseDown) {
				for (int i = 0; i < 6; i++) {
					plasma = (new Ephemeral(textures["FlameThrower"], "Transparent",80,20))->Scale(0.01, 0.01)->Translate(pos);
					randOr = ((rand()*1.0 / RAND_MAX) - 0.5)*(0.6) + MouseDir +M_PI;
					preMat = mat4();
					preMat.m[3][0] = 1.215; preMat.m[3][1] = 0.8;
					plasma->setVelocity(vec2(-0.1*cos(randOr), -0.1*sin(randOr)));
					plasma->setOrientation(or);
					plasma->setAngular(lander->getAngular());

					plasma->setPreMatrix(preMat);
					objects.push_back(plasma);
				}
			}
			
			fireCoolDown--;
			
		}

		void press(unsigned char c) {
			keyPresses[c] = true;
		}

		void release(unsigned char c) {
			keyPresses[c] = false;
		}

		void MouseHeldDown() {
			MouseDown = true;
		}

		void MouseReleased() {
			MouseDown = false;
		}

		void mouseDir(int x, int y, int w, int h) {
			vec2 velo = vec2(x - (w*1.0 / 2.0), (h*1.0 / 2.0)-y);			
			MouseDir = velo.Orientation();
		}

		void Fire(int x, int y,int h, int w) {
			vec2 pos = lander->getPosition();
			vec2 velo = vec2(x - (w*1.0 / 2.0), (h*1.0 / 2.0) - y);
			velo.normalize();
			velo = velo * 0.5;
			Object* ball;

			if (!flamethrower && fireCoolDown <= 0) {
				ball = (new Ephemeral(textures["PokeBall"], "Transparent", 500,250))->Scale(0.136,0.1);
				ball->setName(POKEBALL);
				ball->setVelocity(velo);
				ball->setPosition(pos.x, pos.y);
				objects.push_back(ball);
				fireCoolDown = 50;
			}
			else {
				MouseDir = velo.Orientation();
			}


		}

		// destructor
		~Scene() {
			ShaderManager::Clear();
			//delete background;
			for (int i = 0; i < objects.size(); i++) {
				delete objects[i];
			}
			textures.clear();
			
		}
};