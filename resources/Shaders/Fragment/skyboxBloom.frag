#version 400
in vec3 skyCoords;

uniform samplerCube day;
uniform samplerCube night;
uniform float sunVal;
uniform float bloomCutoff;

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

vec3 BloomColor(vec3 color, float cutoff) {
	float lum = dot(color, vec3(0.2126, 0.7152, 0.0722));
	vec3 bright;
	if (lum >= bloomCutoff)
		bright = color;
	else
		bright = vec3(0.0);
	return bright;
}

void main() {
	vec3 dayCol = texture(day, skyCoords).rgb;
	vec3 nightCol = texture(night, skyCoords).rgb;
	vec3 mixed = mix(nightCol, dayCol, sunVal);
	vec3 bright = BloomColor(mixed, bloomCutoff);

	fragColor = vec4(mixed, 1.0);
	brightColor = vec4(bright, 1.0);
}
