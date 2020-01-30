#version 400

out vec4 fragColor;

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

float sin01(float f) {
	return ((sin(f) + 1.0) * 0.5);
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
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; x++) {
		for(int y = -1; y <= 1; y++) {
			float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;     
		}
	}
	shadow /= 9.0;
	return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = light.direction;

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, vsOut.uv));

	// diffuse
	float diffPower = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffPower * vec3(texture(material.diffuse, vsOut.uv));

	// specular
	vec3 reflectDir = reflect(-lightDir, normal);
	float specPower = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.diffuse * specPower * vec3(texture(material.specular, vsOut.uv));

	// shadows
	float shadow = 1.0 - ShadowCalculation();

	return ambient + shadow * (diffuse + specular);
}

void main() {
	vec3 normal = vsOut.normal;
	vec3 viewDir = normalize(cameraPos - vsOut.worldPos);

	vec3 result = vec3(0.0);
	
	for (int i = 0; i < 2; i++)
		result += CalcDirLight(dirLight[i], normal, viewDir);

	fragColor = vec4(result, 1.0);
}
