#version 450 core
#extension GL_NV_shadow_samplers_cube : enable
precision highp float; 

uniform samplerCube  environmentMap; 

in vec4 worldPosition; 
in vec3 viewDir; 
out vec4 fragmentColor; 
	
void main() 
{ 
	vec3 texel = textureCube(environmentMap, viewDir).xyz; 
	fragmentColor = vec4(texel, 1); 
} 
