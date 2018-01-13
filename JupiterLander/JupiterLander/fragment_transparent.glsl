#version 450 core

precision highp float; 

uniform sampler2D samplerUnit; 
uniform float Transparency; 
in vec2 TexCoord;					
out vec4 out_color;				

void main() { 
	// the output color's transparency is modified based on the uniform variable
	out_color = (texture(samplerUnit, TexCoord) + vec4(0,0,0,Transparency-1));   
}
