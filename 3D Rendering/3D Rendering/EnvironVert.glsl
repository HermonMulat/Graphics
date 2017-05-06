#version 450 core
precision highp float; 

in vec4 vertexPosition; 
uniform mat4 viewDirMatrix; 
	
out vec4 worldPosition; 
out vec3 viewDir; 
	
void main() 
{ 
	worldPosition = vertexPosition; 
	viewDir = (vertexPosition * viewDirMatrix).xyz; 
	gl_Position = vertexPosition; 
	gl_Position.z = 0.999999; 
} 
