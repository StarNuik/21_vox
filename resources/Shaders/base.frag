#version 400
in vec2 uvPos;
in vec3 normal;
in vec3 fragPos;
out vec4 fragColor;

uniform sampler2D diffuseTex;
uniform sampler2D normalTex;

const vec3 lightDir = vec3(0.0, 0.707, 0.707);

void main() {
	vec4 texColor = texture(diffuseTex, uvPos);
	fragColor = texColor;
}