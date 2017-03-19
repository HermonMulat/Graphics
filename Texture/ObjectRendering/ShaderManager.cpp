#include "ShaderManager.h"
#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

map<string, GLuint> ShaderManager::programs; // initialize static member
ShaderManager::ShaderManager() {}			 // Nothing to do in constructor
ShaderManager::~ShaderManager() {			 // Need to delete programs 
	map<string, GLuint>::iterator iter;
	for (iter = programs.begin(); iter != programs.end(); iter++) {
		glDeleteProgram(iter->second);
	}
	programs.clear();
}

string ShaderManager::ReadShader(const string& filename) {
	string shaderCode;
	ifstream file(filename.c_str(), ios::in);

	if (!file.good()) {
		cout << "Can't open " << filename.c_str() << endl;
		terminate();
	}

	file.seekg(0, ios::end);
	shaderCode.resize((unsigned int)file.tellg());
	file.seekg(0, ios::beg);
	file.read(&shaderCode[0], shaderCode.size());
	file.close();

	return shaderCode;
}

GLuint ShaderManager::CreateShader(GLenum shaderType, const string& source, const string& shaderName) {

	int compile_result = 0;

	GLuint shader = glCreateShader(shaderType);
	const char *shader_code_ptr = source.c_str();
	// shader_code_size can also be set to null - that is read untill you reach null
	const int shader_code_size = source.size();

	glShaderSource(shader, 1, &shader_code_ptr, &shader_code_size);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_result);

	//check for errors
	if (compile_result == GL_FALSE) {
		int info_log_length = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_log_length);
		vector<char> shader_log(info_log_length);
		glGetShaderInfoLog(shader, info_log_length, NULL, &shader_log[0]);
		cout << "ERROR compiling shader: " << shaderName.c_str() << endl << &shader_log[0] << std::endl;
		return 0;
	}
	return shader;
}

void ShaderManager::CreateProgram(const string& shaderName, const string& vertexShaderFilename,
	const string& fragmentShaderFilename) {
	if (programs.count(shaderName) > 0) {
		cout << "A shader with that name already exists" << endl;
		return;
	}
	// Read the shader codes
	string vertex_shader_code = ReadShader(vertexShaderFilename);
	string fragment_shader_code = ReadShader(fragmentShaderFilename);

	GLuint vertex_shader = CreateShader(GL_VERTEX_SHADER, vertex_shader_code, shaderName + " (vertex)");
	GLuint fragment_shader = CreateShader(GL_FRAGMENT_SHADER, fragment_shader_code, shaderName + " (fragment)");

	int link_result = 0;

	// Create program handle
	GLuint program = glCreateProgram();
	// attach shaders
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
	// now link
	glLinkProgram(program);
	// Get status update on linking and store it to link_results
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);

	// Error linking
	if (link_result == GL_FALSE) {
		int info_log_length = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &info_log_length); // get length of log
		vector<char> program_log(info_log_length);
		glGetProgramInfoLog(program, info_log_length, NULL, &program_log[0]); // store log info
		cout << "Error creating a program : LINK ERROR" << endl << &program_log[0] << endl;
		return;
	}
	programs[shaderName] = program;
}

const GLuint ShaderManager::GetShader(const string& name) {
	if (programs.count(name) == 0) {
		cout << "Couldn't your find shader :(" << endl;
		return 0;
	}
	return programs.at(name);
}

void ShaderManager::DeleteShader(const string& name) {
	if (programs.count(name) == 0) {
		return;
	}
	programs.erase(name);
}
