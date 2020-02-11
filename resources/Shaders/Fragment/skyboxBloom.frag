#version 400
in vec3 skyCoords;

uniform samplerCube day;
uniform samplerCube night;
uniform float sunVal;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

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
	vec3 dayCol = texture(day, skyCoords).rgb;
	vec3 nightCol = texture(night, skyCoords).rgb;
	vec3 mixed = mix(nightCol, dayCol, sunVal);
	vec3 bright = BloomColor(mixed);

	fragColor = vec4(mixed, 1.0);
	brightColor = vec4(bright, 1.0);
}
