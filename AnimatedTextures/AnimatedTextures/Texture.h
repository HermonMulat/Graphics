#pragma once
#include <iostream>
#include <string>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "ShaderManager.h"

// include the function from stb_image 
extern "C" unsigned char* stbi_load(
	char const *filename,
	int *x, int *y,
	int *comp, int req_comp);

using namespace std;

class Texture {
	unsigned int textureId;
public:
	Texture(const string& inputFileName) {
		unsigned char* data; int width; int height; int nComponents = 4;
		// load texture from file
		data = stbi_load(inputFileName.c_str(), &width, &height, &nComponents, 0);
		
		if (data == NULL) { 
			cout << "Can not read texture image" << endl;
			return; 
		}

		glGenTextures(1, &textureId);				// generate texture ID
		glBindTexture(GL_TEXTURE_2D, textureId);	// make the given texture active

		// upload the texture to the GPU memory
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set minification filter
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); // set magnification filter
		delete data; // delete the temporary buffer
	}

	void Bind(const string& shaderProgram) {
		int samplerUnit = 0; 
		int location = glGetUniformLocation(ShaderManager::GetShader(shaderProgram), "samplerUnit");
		glUniform1i(location, samplerUnit);
		glActiveTexture(GL_TEXTURE0 + samplerUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);

	}
};
