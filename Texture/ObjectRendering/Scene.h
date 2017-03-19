#pragma once
#include "Object.h"
#include "Triangle.h"
#include "Quads.h"
#include "Texture.h"
#include "TexturedQuads.h"
#include "ShaderManager.h"

#include <vector>
#include <iostream>

using namespace std;

class Scene {
	private:
		ShaderManager* programs;
		vector<Texture*> textures;
		vector<Object*> objects;

	public:
		Scene() {
			// load shaders
			programs->CreateProgram("SimpleObject", "vertexShader.glsl", "fragmentShader.glsl");
			programs->CreateProgram("BlueObject", "vertexShader.glsl", "fs_blue.glsl");
			programs->CreateProgram("TexturedObj", "vertex_texture.glsl", "fragment_texture.glsl");

			// add and load texture
			textures.push_back(new Texture("asteroid.png"));
			textures.push_back(new Texture("asteroid1.png"));
			textures.push_back(new Texture("asteroid2.png"));

			for (int i = 0; i < 15; i++) {
				int k = rand() % 3;
				cout << k << endl;
				objects.push_back(new Asteroid(textures[k], "TexturedObj"));
			}

		}

		// destructor
		~Scene() {
			delete programs;
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
};