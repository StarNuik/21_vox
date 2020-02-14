#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

out VS_OUT {
	vec3 worldPos;
	vec4 lightPos;
	vec3 normal;
	vec2 uv;
} vsOut;

// out vs_out 

uniform mat4 model;
uniform mat4 lightSpace;
uniform mat4 mvp;

void main() {
	vsOut.uv = vertexUV;
	vsOut.worldPos = vec3(model * vec4(vertexPosition, 1.0));
	vsOut.normal = normalize(mat3(model) * vertexNormal);
	vsOut.lightPos = lightSpace * vec4(vsOut.worldPos, 1.0);
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}