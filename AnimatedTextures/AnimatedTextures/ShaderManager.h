#pragma once

#include <iostream>
#include "GL/glew.h"		 
#include "GL/freeglut.h"
#include <map>


using namespace std;

class ShaderManager
{
private:
	static map<string, GLuint> programs;
	static string ReadShader(const string& filename);
	static GLuint CreateShader(GLenum shaderType, const string& source, const string& shaderName);

public:
	ShaderManager();
	static void CreateProgram(const string& shaderName, const string& vertexShaderFilename, const string& fragmentShaderFilename);
	static const GLuint GetShader(const string& name);
	static void DeleteShader(const string& name);
	static void Clear();
};


