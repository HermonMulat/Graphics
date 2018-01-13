#version 450 core

precision highp float;    
                    
layout(location = 0) in vec2 in_position;  
layout(location = 1) in vec3 in_color; 
uniform mat4 MVP;

out vec3 color;
void main()
{
  color = in_color;
  gl_Position = vec4(in_position, 0.0, 1.0)*MVP;	   
}