#version 450 core 
precision highp float; 
	
uniform sampler2D samplerUnit; 
in vec2 texCoord; 
in vec4 worldNormal; 
out vec4 fragmentColor; 
	
void main() { 
	vec3 L = vec3(0.0, 0.0, 1.0); 
	vec3 N = normalize(worldNormal.xyz); 
	fragmentColor = texture(samplerUnit, texCoord) * max(0.0, dot(L, N)); 
}