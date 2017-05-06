#pragma once
#include <iostream>
#include <string>
#include "GL\glew.h"
#include "GL\freeglut.h"
#include "ShaderManager.h"

// include the function from stb_image 
extern "C" unsigned char* stbi_load(char const *filename,int *x, int *y, int *comp, int req_comp);

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
			if (nComponents == 4) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);	
			if (nComponents == 3) glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // set minification filter
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // set magnification filter

			delete data; // delete the temporary buffer

		}

		void Bind(unsigned int shaderID) {
			int samplerUnit = 0; 
			int location = glGetUniformLocation(shaderID, "samplerUnit");
			if (location >= 0) {
				glUniform1i(location, samplerUnit);
				glActiveTexture(GL_TEXTURE0 + samplerUnit);
				glBindTexture(GL_TEXTURE_2D, textureId);
			}
			else cout << "Can not set uniform variable smapler\n";
		}
};

class TextureCube {
	unsigned int textureId;
	public:
		TextureCube(
			const std::string& inputFileName0, const std::string& inputFileName1, const std::string& inputFileName2,
			const std::string& inputFileName3, const std::string& inputFileName4, const std::string& inputFileName5) {
			unsigned char* data[6]; int width[6]; int height[6]; int nComponents[6]; std::string filename[6];
			filename[0] = inputFileName0; filename[1] = inputFileName1; filename[2] = inputFileName2;
			filename[3] = inputFileName3; filename[4] = inputFileName4; filename[5] = inputFileName5;
			for (int i = 0; i < 6; i++) {
				data[i] = stbi_load(filename[i].c_str(), &width[i], &height[i], &nComponents[i], 0);
				if (data == NULL) return;
			}
			glGenTextures(1, &textureId); glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
			for (int i = 0; i < 6; i++) {
				if (nComponents[i] == 4) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA,
					width[i], height[i], 0, GL_RGBA, GL_UNSIGNED_BYTE, data[i]);
				if (nComponents[i] == 3) glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
					width[i], height[i], 0, GL_RGB, GL_UNSIGNED_BYTE, data[i]);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Don't what they do - something to do with wrapping
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			for (int i = 0; i < 6; i++)  delete data[i];
		}

		void Bind(unsigned int shader) {
			int samplerCube = 1;
			int location = glGetUniformLocation(shader, "envMap");
			glUniform1i(location, samplerCube);
			glActiveTexture(GL_TEXTURE0 + samplerCube);
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureId);
		}
};
