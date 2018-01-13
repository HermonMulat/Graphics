#version 450 core

precision highp float; 

uniform sampler2D samplerUnit; 
in vec2 TexCoord;					
out vec4 out_color;				

void main() { 
	out_color = texture(samplerUnit, TexCoord) ; 
}
