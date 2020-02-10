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
uniform float bloomCutoff;

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
	vec4 texColor = texture(material.diffuse, vsOut.uv);
	if (texColor.a == 0.0)
		discard;
	vec3 result = vec3(texColor) * (1.0 + material.emission);
	vec3 bright = BloomColor(result, bloomCutoff);

	fragColor = vec4(result, 1.0);
	brightColor = vec4(bright, 1.0);
}