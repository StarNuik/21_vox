#version 400

out vec4 fragColor;

in VS_OUT {
	vec2 uv;
} vsOut;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
	float emission;
};

uniform Material material;

void main() {
	vec4 texColor = texture(material.diffuse, vsOut.uv);
	if (texColor.a == 0.0)
		discard;
	fragColor = vec4(vec3(texColor) * (1.0 + material.emission), 1.0);
}