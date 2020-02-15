#version 400

layout (location = 0) out vec3 fragPos;
layout (location = 1) out vec3 fragNormal;
layout (location = 2) out vec4 fragAlbedoSpec;

in VS_OUT {
	vec3 worldPos;
	vec3 normal;
	vec2 uv;
	mat3 TBN;
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
	vec4 albedo = texture(material.diffuse, vsOut.uv);
	if (albedo.a == 0.0)
		discard;
	vec4 normal4 = texture(material.normal, vsOut.uv);
	vec3 normal;
	if (normal4.a == 0.0)
		normal = normalize(vsOut.normal);
	else
		normal = vsOut.TBN * normalize(normal4.rgb * 2.0 - 1.0);
	fragNormal = normal;
	fragPos = vsOut.worldPos;
	fragAlbedoSpec = vec4(albedo.rgb, texture(material.specular, vsOut.uv).r);
}