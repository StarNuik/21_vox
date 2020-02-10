#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

vec3 BloomColor(vec3 color) {
	const float threshold = 0.5;
	const float size = 0.4;
	const float antisize = 1.0 / 0.2;

	float lum = dot(color, vec3(0.2126, 0.7152, 0.0722));
	float diff = lum - threshold;
	vec3 bright;
	if (diff < 0.0) {
		bright = vec3(0.0);
	} else if (diff < size) {
		bright = color * diff * antisize;
	} else {
		bright = color;
	}
	return bright;
}

void main() {
	vec3 hdrColor = texture(screenTexture, fragPos).rgb;
	vec3 mapped = BloomColor(hdrColor);
	fragColor = vec4(mapped, 1.0);
}