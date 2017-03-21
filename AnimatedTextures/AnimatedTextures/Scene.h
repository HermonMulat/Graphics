#pragma once
#include "Object.h"
#include "Triangle.h"
#include "Quads.h"
#include "Texture.h"
#include "TexturedQuads.h"
#include "ShaderManager.h"

#include <vector>
#include <iostream>
#include <string>

using namespace std;

class Scene {
	private:
		vector<Texture*> textures;
		vector<Object*> objects;

	public:
		Scene() {
			// load shaders
			ShaderManager::CreateProgram("SimpleObject", "vertexShader.glsl", "fragmentShader.glsl");
			ShaderManager::CreateProgram("TexturedObj", "vertex_texture.glsl", "fragment_texture.glsl");
			ShaderManager::CreateProgram("Explosion", "vertex_explosion.glsl", "fragment_explosion.glsl");
			// add and load texture

			textures.push_back(new Texture("asteroid.png"));
			textures.push_back(new Texture("cannon.png"));
			textures.push_back(new Texture("boom.png"));
			textures.push_back(new Texture("bullet2.png"));

			objects.push_back(new Cannon(textures[1], "TexturedObj"));
			for (int i = 0; i < 4; i++) {
				objects.push_back(new Asteroid(textures[0], "TexturedObj"));
			}
			objects.push_back((new Explosion(textures[2], "Explosion"))->Translate(vec2(0.5,0.5)));
			
		}

		// destructor
		~Scene() {
			ShaderManager::Clear();
			for (int i = 0; i < objects.size(); i++) {
				delete objects[i];
			}
			for (int j = 0; j < textures.size(); j++) {
				delete textures[j];
			}
		}

		void Draw(int windowWidth, int windowHeight) {
			
			for (int i = 0; i < objects.size(); i++){
				objects[i]->Draw(windowWidth, windowHeight);
			}
		}

		void Move(double dt) {
			for (int i = 0; i < objects.size(); i++) {
				objects[i]->move((float)dt);
			}
		}
		void Fire() {
			vec2 velo = ((Cannon*) objects[0])->GetBulletVelocity();
			float ori = ((Cannon*)objects[0])->GetOrientation();
			Bullet* b = new Bullet(textures[3], "TexturedObj");
			b->setVelocity(velo);
			b->setPosition(ori);
			objects.push_back(b);
		}
};