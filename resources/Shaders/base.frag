#version 400
in vec2 uvPos;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;
uniform int tick;

float sin01(float f) {
	return ((sin(f) + 1.0) * 0.5);
}

void main() {
	vec4 texColor = texture(diffuseTex, uvPos);
	fragColor = texColor;
}