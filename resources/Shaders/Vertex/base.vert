#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;
layout (location = 3) in vec3 vertexTangent;

out VS_OUT {
	vec3 worldPos;
	vec3 normal;
	vec2 uv;
	mat3 TBN;
} vsOut;

uniform mat4 model;
uniform mat4 mvp;

void main() {
	vsOut.uv = vertexUV;
	vsOut.worldPos = mat3(model) * vertexPosition;
	vsOut.normal = normalize(mat3(model) * vertexNormal);
	gl_Position = mvp * vec4(vertexPosition, 1.0);

	vec3 T = normalize(mat3(model) * vertexTangent);
	vec3 B = cross(vsOut.normal, T);
	vsOut.TBN = mat3(T, B, vsOut.normal);
}