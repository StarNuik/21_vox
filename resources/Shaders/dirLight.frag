#version 400
in vec2 uvPos;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
};

struct DirLight {
	vec3 direction;
	vec3 color;
};

uniform DirLight dirLight[2];
uniform Material material;

float sin01(float f) {
	return ((sin(f) + 1.0) * 0.5);
}

void main() {
	vec4 texColor = texture(material.diffuse, uvPos);
	fragColor = texColor;
}