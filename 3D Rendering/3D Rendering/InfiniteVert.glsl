#version 450 core 
precision highp float; 
	
layout(location = 0) in vec4 vertexPosition; 
layout(location = 1) in vec2 vertexTexCoord; 
layout(location = 2) in vec3 vertexNormal; 
uniform mat4 M, MInv, MVP; 
	
out vec2 texCoord; 
out vec4 worldPosition; 
out vec3 worldNormal; 
	
void main() { 
		texCoord = vertexTexCoord; 
		worldPosition = vertexPosition * M; 
		worldNormal = (MInv * vec4(vertexNormal, 0.0)).xyz; 
		gl_Position = vertexPosition * MVP; 
} 
