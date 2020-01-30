#version 400

out vec4 fragColor;

in VS_OUT {
	vec3 worldPos;
	vec3 normal;
	vec2 uv;
} vsOut;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
};

uniform Material material;

void main() {
	vec4 texColor = texture(material.diffuse, vsOut.uv);
	fragColor = texColor;
}