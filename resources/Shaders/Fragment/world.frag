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
uniform sampler2D theWorldGradient;
uniform float standRadius;

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

#define and &&
#define or ||
#define not !

vec3 invertColor(vec3 color) {
	return 1.0 - color;
}

vec3 SphereLine(vec3 inColor, vec3 fragPos, vec3 centerPos, float radius) {
	float radius3 = pow(radius, 3.0);
	const float biasPercent = 0.4;
	float bias = radius * biasPercent;
	float bias3 = radius3 * biasPercent;
	float antibias = 1.0 / bias;
	float antibias3 = 1.0 / bias3;
	float antismth3 = 1.0 / (radius3 - bias3);
	vec3 lineColor = vec3(0.3, 0.0, 1.0);

	vec3 local = fragPos - centerPos;
	
	float local3 = dot(local, local);
	if (local3 >= radius3) { // Outside the sphere
		return vec3(0.0);
	} else if (local3 > radius3 - bias3) { // Sphere line
		float f = (radius3 - local3) * antibias3;
		return mix(vec3(0.0), invertColor(inColor), f);
		// return lineColor;
	} else { // Inside the sphere
		float f = (radius3 - bias3 - local3) * antismth3;
		// float average = 0.2126 * inColor.r + 0.7152 * inColor.g + 0.0722 * inColor.b;
		// vec3 worldColor = mix(inColor, vec3(average), 0.5);
		vec4 texVal = texture(theWorldGradient, vec2(f, 0.5));
		vec3 texColor = vec3(texVal);
		float texFactor = texVal.a;
		float mixFactor = 0.3 * texFactor;
		return mix(inColor, texColor, mixFactor);
	}
}

const float whiteLength = 20.0;
const float inverseLength = 50.0;
const float colorLength = 200.0;
const float bias = 3.0;
const float wlen2 = whiteLength * whiteLength;
const float ilen2 = inverseLength * inverseLength;
const float clen2 = colorLength * colorLength;
const float antiwlen2 = 1.0 / wlen2;
const float antiilen2 = 1.0 / ilen2;
const float anticlen2 = 1.0 / clen2;
const float bias2 = bias * bias;
const float colorMixPercent = 0.3;
const float grayscaleMixPercent = 0.7;
vec3 TheWorld(vec3 inColor, vec3 fragPos, vec3 centerPos, float radius) {
	vec3 local = fragPos - centerPos;
	float dist2 = dot(local, local);
	float radius2 = pow(radius, 2.0);

	vec3 white = vec3(1.0);
	vec3 inverse = invertColor(inColor);
	float colf = (dist2 - (radius2 - wlen2 - ilen2 - clen2)) * anticlen2;
	vec4 texVal = texture(theWorldGradient, vec2(colf, 0.5));
	float texf = colorMixPercent * texVal.a;
	vec3 color = mix(inColor, texVal.rgb, texf);
	float average = 0.2126 * inColor.r + 0.7152 * inColor.g + 0.0722 * inColor.b;
	vec3 grayscale = mix(inColor, vec3(average), grayscaleMixPercent);

	if (dist2 >= radius2) { // Outside the sphere
		return inColor;
	} else if (dist2 > radius2 - bias2) { // Transition to white
		float f = (radius2 - dist2) * antiwlen2;
		return mix(inColor, white, f);
	} else if (dist2 > radius2 - wlen2) { // White part
		return white;
	} else if (dist2 > radius2 - wlen2 - bias2) { // Transition to inverse
		float f = (radius2 - wlen2 - dist2) * antiilen2;
		return mix(white, inverse, f);
	} else if (dist2 > radius2 - wlen2 - ilen2) { // Inverse part
		return inverse;
	} else if (dist2 > radius2 - wlen2 - ilen2 - bias2) { // Transition to color
		float f = (radius2 - wlen2 - ilen2 - dist2) * anticlen2;
		return mix(inverse, color, f);
	} else if (dist2 > radius2 - wlen2 - ilen2 - clen2) { // Color part
		return color;
	} else if (dist2 > radius2 - wlen2 - ilen2 - clen2 - bias2) { // Transition to grayscale
		float f = (radius2 - wlen2 - ilen2 - clen2 - dist2) / (radius2 - wlen2 - ilen2 - clen2);
		return mix(color, grayscale, f);
	} else { // Grayscale
		return grayscale;
	}
	return inColor;
}

void main() {
	vec4 diffuse = texture(material.diffuse, vsOut.uv);
	if (diffuse.a == 0.0) {
		discard;
	}
	vec3 normal = normalize(vsOut.normal);
	vec3 viewDir = normalize(cameraPos - vsOut.worldPos);
	float shadow = 1.0 - ShadowCalculation();

	// vec3 theWorldDiffuse = SphereLine(diffuse.rgb, vsOut.worldPos, cameraPos, 20.0);

	vec3 result = vec3(0.0);
	
	for (int i = 0; i < 2; i++)
		result += CalcDirLight(dirLight[i], normal, viewDir, diffuse.rgb, shadow);

	result = TheWorld(result, vsOut.worldPos, cameraPos, standRadius);

	vec3 bright = BloomColor(result);

	fragColor = vec4(result, 1.0);
	brightColor = vec4(bright, 1.0);
}
