#pragma once

#include <vector>
#include "Object.h"
#include "Texture.h"
#include "Mesh.h"
#include "Camera.h"
#include "Light.h"
#include "TexturedQuad.h"
#include "FullScreenQuad.h"

using namespace std;

class Scene{
	
	vector<Texture*> textures;
	vector<Mesh*> meshes;
	vector<Object*> objects;
	TextureCube* Env;

	vector<int> keyPressed;

	Light * light;
	Camera* camera;

	public:
		Scene():keyPressed(256,0) {}

		void Initialize(){
			
			light = new Light(vec3(0.5, 0.5, 0.5), vec3(1, 1, 1));
			camera = new Camera();
			vec3 lightPos = camera->GetEyePos() + vec3(0,2,0);
			light->SetPointLightSource(lightPos);

			// load environment
			Env = new TextureCube(	"../media/posx512.jpg", 
									"../media/negx512.jpg", 
									"../media/posy512.jpg", 
									"../media/negy512.jpg", 
									"../media/posz512.jpg", 
									"../media/negz512.jpg");

			ShaderManager::CreateProgram("MaterialLight", "MaterialLightVert.glsl", "MaterialLightFrag.glsl");
			ShaderManager::CreateProgram("Ground", "InfiniteVert.glsl", "InfiniteFrag.glsl");
			ShaderManager::CreateProgram("Shadow", "ShadowVert.glsl", "ShadowFrag.glsl");
			ShaderManager::CreateProgram("EnvMap", "EnvMapVert.glsl", "EnvMapFrag.glsl");
			ShaderManager::CreateProgram("Environ", "EnvironVert.glsl", "EnvironFrag.glsl");

			// Load textures
			textures.push_back(new Texture("../media/tigger.png"));
			textures.push_back(new Texture("../media/tree.png"));

			// Load meshes
			meshes.push_back(new Mesh("../media/tigger.obj"));
			meshes.push_back(new Mesh("../media/tree.obj"));

			MeshInstance* Tigger = new MeshInstance(textures[0], meshes[0], "EnvMap");
			Tigger->setOrientation(-50.0);
			Tigger->setAngularSpeed(0);

			objects.push_back(Tigger);

			Tree* t = new Tree(textures[1], meshes[1], "EnvMap");
			t->setPosition(vec3(0.65, 0.0, -0.1));
			Tree* t2 = new Tree(textures[1], meshes[1], "EnvMap");
			t2->setPosition(vec3(0.65, 0.0, -0.6));
			Tree* t3 = new Tree(textures[1], meshes[1], "EnvMap");
			t3->setPosition(vec3(-0.65, 0.0, -0.3));
			objects.push_back(t);
			objects.push_back(t2);
			objects.push_back(t3);
			objects.push_back(new FullScreenQuad("Environ"));
			//objects.push_back(new TexturedQuad((new Material()), textures[1], "Ground"));
			
		}

		~Scene(){
			for (int i = 0; i < textures.size(); i++) delete textures[i];
			for (int i = 0; i < meshes.size(); i++) delete meshes[i];
			for (int i = 0; i < objects.size(); i++) delete objects[i];
		}

		void KeyPress(unsigned char key) {
			keyPressed[key] = 1;
		}

		void KeyRelease(unsigned char key) {
			keyPressed[key] = 0;
		}

		void Draw(){
			for (int i = 0; i < objects.size(); i++) {
				objects[i]->Draw(light, camera, Env);
				if (i == 0) objects[i]->DrawShadow(light, camera,"Shadow");
			}
		}

		void Move(float dt){
			for (int i = 0; i < objects.size(); i++) objects[i]->Move(dt);
			if (keyPressed['w']) {
				camera->Up();
				light->SetPointLightSource(camera->GetEyePos() + vec3(0, 2, 0));
			}
			if (keyPressed['s']) {
				camera->Down();
				light->SetPointLightSource(camera->GetEyePos() + vec3(0, 2, 0));

			}
			if (keyPressed['a']) camera->Left(); 
			if (keyPressed['d']) camera->Right();
		}

		void Control(){
			for (int i = 0; i < objects.size(); i++) objects[i]->Control();
		}

		void SetAspectRatio(float ar) {
			camera->SetAspectRatio(ar);
		}
};