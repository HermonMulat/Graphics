#version 450 core 
precision highp float; 

uniform sampler2D samplerUnit; 
uniform vec3 La, Le; 
uniform vec3 ka, kd, ks; 
uniform float shininess; 

in vec2 texCoord; 
in vec3 worldNormal; 
in vec3 worldView; 
in vec3 worldLight; 

out vec4 fragmentColor;

void main() { 
	vec3 N = normalize(worldNormal); 
	vec3 V = normalize(worldView); 
	vec3 L = normalize(worldLight);  
	vec3 H = normalize(V + N);
	vec3 texel = texture(samplerUnit, texCoord).xyz;
	vec3 color = La * ka + 
				 Le * kd * texel* max(0.0, dot(L, N)) +  
				 Le * ks * pow(max(0.0, dot(H, N)), shininess); 
	fragmentColor = vec4(color, 1); 
} 
