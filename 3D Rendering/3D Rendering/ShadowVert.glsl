#version 450 core 
precision highp float;
 
layout(location = 0) in vec3 vertexPosition; 
layout(location = 1) in vec2 vertexTexCoord; 
layout(location = 2) in vec3 vertexNormal; 

uniform mat4 M, VP,ShadowMat;
 
void main() { 
	vec4 worldPosition = vec4(vertexPosition, 1) * M * ShadowMat * VP;
	gl_Position = worldPosition*0.5; 
} 