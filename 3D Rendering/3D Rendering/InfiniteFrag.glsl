#version 130 
precision highp float; 

uniform sampler2D samplerUnit; 
uniform vec3 La, Le; 
uniform vec3 ka, kd, ks; 
uniform float shininess; 
uniform vec3 worldEye; 
uniform vec4 worldLightPosition;
 
in vec2 texCoord; 
in vec4 worldPosition; 
in vec3 worldNormal; 

out vec4 fragmentColor; 

void main() { 
	vec3 N = normalize(worldNormal); 
	vec3 V = normalize(worldEye * worldPosition.w - worldPosition.xyz);
	vec2 position = 2*worldPosition.xy / worldPosition.w;  // just multiplied it by 3 so the ground has more stripes
	vec3 L = normalize(worldLightPosition.xyz * worldPosition.w - worldPosition.xyz * worldLightPosition.w);
	vec3 H = normalize(V + L); 
	vec2 tex = (position.xy - floor(position.xy)); 
	vec3 texel = texture(samplerUnit, tex).xyz; 
	vec3 color = La * ka + Le * kd * texel* max(0.0, dot(L, N)) + 
					Le * ks * pow(max(0.0, dot(H, N)), shininess); 
	fragmentColor = vec4(color, 1); 
} 
