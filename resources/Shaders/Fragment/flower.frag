#version 400

layout (location = 0) out vec4 fragColor;
layout (location = 1) out vec4 brightColor;

in VS_OUT {
	vec3 worldPos;
	vec4 lightPos;
	vec3 normal;
	vec2 uv;
} vsOut;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
	float emission;
};

struct DirLight {
	vec3 direction;
	vec3 diffuse;
	vec3 ambient;
};

uniform vec3 cameraPos;
uniform DirLight dirLight[2];
uniform Material material;
uniform sampler2D shadowMap;

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
		bright = color * diff * antisize * 0.5;
	} else {
		bright = color * 0.5;
	}
	return bright;
}

float ShadowCalculation() {
	const float bias = 0.0005;

	vec3 projCoords = vsOut.lightPos.xyz / vsOut.lightPos.w;
	projCoords = projCoords * 0.5 + 0.5;
	if (projCoords.z > 1.0)
		return 1.0;
	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;

	float shadow = 0.0;
	float depth = texture(shadowMap, projCoords.xy).r;
	shadow = currentDepth - bias > depth ? 1.0 : 0.0;
	return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 diffuseBase, float shadow) {
	vec3 lightDir = light.direction;

	// ambient
	vec3 ambient = light.ambient * diffuseBase;

	// diffuse
	float diffPower = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffPower * diffuseBase;

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specPower = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.diffuse * specPower * vec3(texture(material.specular, vsOut.uv));

	return ambient + shadow * (diffuse + specular);
}

void main() {
	vec4 diffuse = texture(material.diffuse, vsOut.uv);
	if (diffuse.a == 0.0) {
		discard;
	}
	vec3 normal = normalize(vsOut.normal);
	vec3 viewDir = normalize(cameraPos - vsOut.worldPos);
	float shadow = 1.0 - ShadowCalculation();

	vec3 result = vec3(0.0);
	
	for (int i = 0; i < 2; i++)
		result += CalcDirLight(dirLight[i], normal, viewDir, diffuse.rgb, shadow);

	vec3 bright = BloomColor(result);

	fragColor = vec4(result, 1.0);
	brightColor = vec4(bright, 1.0);
}
