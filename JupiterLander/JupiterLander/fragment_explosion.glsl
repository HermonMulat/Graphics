#version 450 core

precision highp float; 

uniform sampler2D samplerUnit; 
uniform int subTexture; 

in vec2 TexCoord; 
out vec4 fragmentColor; 

void main() { 
	int i = subTexture % 6; 
	int j = subTexture / 6; 
	fragmentColor = texture(samplerUnit, (vec2(i, j) + TexCoord) / 6.0); 
} 
