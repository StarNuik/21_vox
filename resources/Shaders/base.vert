#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out VS_OUT {
	vec3 worldPos;
	vec3 normal;
	vec2 uv;
} vsOut;

uniform mat4 model;
uniform mat4 mvp;

void main() {
	vsOut.uv = vertexUV;
	vsOut.worldPos = vec3(model * vec4(vertexPosition, 1.0));
	vsOut.normal = normalize(mat3(transpose(inverse(model))) * vertexNormal);
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}