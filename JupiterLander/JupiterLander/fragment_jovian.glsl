#version 450 core

precision highp float; 

uniform sampler2D samplerUnit; 
uniform int subTexture; 

in vec2 TexCoord; 
out vec4 fragmentColor; 

void main() { 
	int i = subTexture % 10; 
	int j = subTexture / 10; 
	vec2 temp = vec2(i, j) + TexCoord;
	temp.x /= 10;
	temp.y /= 14;
	fragmentColor = texture(samplerUnit, temp); 
} 