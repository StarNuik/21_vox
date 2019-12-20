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
	vec3 diffuse;
	vec3 ambient;
};

uniform vec3 cameraPos;
uniform DirLight dirLight[2];
uniform Material material;

float sin01(float f) {
	return ((sin(f) + 1.0) * 0.5);
}

vec3 CalcDirLight(DirLight light, vec3 norm, vec3 viewDir) {
	vec3 lightDir = light.direction;

	// ambient
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, uvPos));

	// diffuse
	float diffPower = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffPower * vec3(texture(material.diffuse, uvPos));

	// specular: TBD
	vec3 specular = vec3(0.0);

	return ambient + diffuse + specular;
}

void main() {
	vec3 norm = normalize(normal);
	vec3 viewDir = normalize(cameraPos - fragPos);

	vec3 result = vec3(0.0);
	
	for (int i = 0; i < 2; i++)
		result += CalcDirLight(dirLight[i], norm, viewDir);

	fragColor = vec4(result, 1.0);
}
