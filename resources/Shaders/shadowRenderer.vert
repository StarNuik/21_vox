#version 400
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 lightSpace;
uniform mat4 lightMVP;

void main() {
	gl_Position = lightMVP * vec4(position, 1.0);
}