#version 450 core 
precision highp float; 
	
layout(location = 0) in vec3 vertexPosition; 
layout(location = 1) in vec2 vertexTexCoord; 
layout(location = 2) in vec3 vertexNormal; 

uniform mat4 MInv, MVP; 
out vec2 texCoord; 
out vec4 worldNormal; 
	
void main() { 
	texCoord = vertexTexCoord; 
	worldNormal = MInv * vec4(vertexNormal, 0.0); 
	gl_Position = vec4(vertexPosition, 1) * MVP; 
} 
