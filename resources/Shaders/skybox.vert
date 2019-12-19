#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 unused1;
layout(location = 2) in vec2 unused2;

out vec3 skyCoords;

uniform mat4 projection;
uniform mat4 view;

void main() {
	skyCoords = position;
	gl_Position = projection * view * vec4(position, 1.0);
}