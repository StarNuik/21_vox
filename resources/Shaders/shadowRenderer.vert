#version 400
layout(location = 0) in vec3 position;

uniform mat4 model;
uniform mat4 lightSpace;

void main() {
	vec4 worldPos = model * vec4(position, 1.0);
	gl_Position = lightSpace * worldPos;
}