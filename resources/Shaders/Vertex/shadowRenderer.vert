#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUV;

uniform mat4 model;
uniform mat4 lightSpace;
uniform mat4 lightMVP;

out vec2 uv;

void main() {
	uv = vertexUV;
	gl_Position = lightMVP * vec4(vertexPosition, 1.0);
}