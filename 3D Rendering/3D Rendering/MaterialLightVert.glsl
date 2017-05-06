#version 450 core 
precision highp float;
 
layout(location = 0) in vec3 vertexPosition; 
layout(location = 1) in vec2 vertexTexCoord; 
layout(location = 2) in vec3 vertexNormal; 

uniform mat4 M, MInv, MVP;
uniform vec3 worldEye; 
uniform vec4 worldLightPosition; 

out vec2 texCoord; 
out vec3 worldNormal; 
out vec3 worldView; 
out vec3 worldLight;
 
void main() { 
	texCoord = vertexTexCoord; 
	vec4 worldPosition = vec4(vertexPosition, 1) * M; 
	worldLight = worldLightPosition.xyz * worldPosition.w - worldPosition.xyz * worldLightPosition.w; 
	worldView = worldEye - worldPosition.xyz; 
	worldNormal = (MInv * vec4(vertexNormal, 0.0)).xyz; 
	gl_Position = vec4(vertexPosition, 1) * MVP; 
} 