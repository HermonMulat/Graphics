#version 450 core   
precision highp float;    
                    
layout(location = 0) in vec2 in_position;  //set the frist input on location (index) 0 ; in_position is our attribute 
layout (location = 1) in vec2 in_texCoord;
uniform mat4 MVP;

out vec2 TexCoord;
void main()
{
	TexCoord = in_texCoord;
	gl_Position = vec4(in_position, 0.0, 1.0)*MVP;	   //w is 1.0, also notice cast to a vec4
}