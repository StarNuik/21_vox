#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;


void main() {
	const float gamma = 2.2;
	
	vec3 color = vec3(texture(screenTexture, fragPos));
	color = pow(color, vec3(1.0 / gamma));
	fragColor = vec4(color, 1.0);
}