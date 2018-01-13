#version 450 core

precision highp float;

in vec3 color;
out vec4 out_color;
 
void main()
{
  out_color = vec4(0.0,0.0,1.0, 1.0);
}