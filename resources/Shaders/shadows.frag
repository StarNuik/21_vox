
#version 400
in vec2 uvPos;
in vec3 normal;
in vec3 fragPos;
in vec4 fragPosLS;

out vec4 fragColor;

struct Material {
	sampler2D diffuse;
	sampler2D normal;
	sampler2D specular;
	float shininess;
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

float sin01(float f) {
	return ((sin(f) + 1.0) * 0.5);
}

float ShadowCalculation() {
	const float bias = 0.01;
	vec3 projCoords = fragPosLS.xyz / fragPosLS.w;
	projCoords = projCoords * 0.5 + 0.5;

	float closestDepth = texture(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir) {
	vec3 lightDir = light.direction;

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uvPos));

	// diffuse
	float diffPower = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffPower * vec3(texture(material.diffuse, uvPos));

	// specular
	vec3 reflectDir = reflect(-lightDir, norm);
	float specPower = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.diffuse * specPower * vec3(texture(material.specular, uvPos));

	// shadows
	float shadow = 1.0 - ShadowCalculation();

	return ambient + shadow * (diffuse + specular);
}

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 result = vec3(0.0);
	
	for (int i = 0; i < 2; i++)
		result += CalcDirLight(dirLight[i], norm, viewDir);

	fragColor = vec4(result, 1.0);
}
