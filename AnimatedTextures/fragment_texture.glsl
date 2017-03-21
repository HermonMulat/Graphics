#version 450 core
precision highp float; 

uniform sampler2D samplerUnit; 
in vec2 TexCoord;					// variable input: interpolated texture coordinates
out vec4 out_color;				

void main() { 
	out_color = texture(samplerUnit, TexCoord);  // the output color is taken from the texture instead 
}
