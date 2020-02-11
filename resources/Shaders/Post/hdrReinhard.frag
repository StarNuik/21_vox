#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

void main() {
	vec3 hdrColor = texture(screenTexture, fragPos).rgb;

	vec3 mapped = hdrColor / (hdrColor + vec3(1.0));

	fragColor = vec4(mapped, 1.0);
}