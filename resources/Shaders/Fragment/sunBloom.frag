#version 400

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

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
	vec4 texColor = texture(material.diffuse, vsOut.uv);
	if (texColor.a == 0.0)
		discard;
	vec3 result = vec3(texColor) * (1.0 + material.emission);
	vec3 bright = BloomColor(result);

	fragColor = vec4(result, 1.0);
	brightColor = vec4(bright, 1.0);
}