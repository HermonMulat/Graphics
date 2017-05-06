#version 450 core 
#extension GL_NV_shadow_samplers_cube : enable
precision highp float; 

uniform sampler2D samplerUnit;
uniform samplerCube envMap; 
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

	vec3 RefAngle = N * dot(N, V)*2 - V;

	vec3 texel = texture(samplerUnit, texCoord).xyz;
	vec3 envRef = textureCube(envMap,RefAngle).xyz;

	vec3 color = La * ka + 
				 Le * kd * texel* max(0.0, dot(L, N)) +  
				 Le * ks * pow(max(0.0, dot(H, N)), shininess); 

	fragmentColor = vec4(color*0.75 + envRef*0.25, 1); 
} 
