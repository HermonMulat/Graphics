#version 450 
precision highp float; 

uniform sampler2D samplerUnit; 
in vec2 TexCoord;					// variable input: interpolated texture coordinates
out vec4 fragmentColor;				

void main() { 
	fragmentColor = texture(samplerUnit, TexCoord);  // the output color is taken from the texture instead 
}
