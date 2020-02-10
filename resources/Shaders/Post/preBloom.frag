#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;
uniform float bloomCutoff;

void main() {
	vec3 hdrColor = texture(screenTexture, fragPos).rgb;
	float lum = dot(hdrColor, vec3(0.2126, 0.7152, 0.0722));
	vec3 mapped;
	if (lum >= 0.7)
		mapped = hdrColor;
	else
		mapped = vec3(0.0);
	fragColor = vec4(mapped, 1.0);
}