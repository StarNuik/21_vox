#version 400
layout(location = 0) in vec3 position;

out vec3 skyCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

uniform mat4 mvp;

void main() {
	skyCoords = position;
	gl_Position = mvp * vec4(position, 1.0);
}
