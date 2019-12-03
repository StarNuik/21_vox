#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUvPos;

out vec2 uvPos;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {
	uvPos = inUvPos;
	vec4 worldPos = model * vec4(position, 1.0);
	fragPos = vec3(worldPos);
	gl_Position = projection * view * worldPos;
	normal = normalize(mat3(transpose(inverse(model))) * inNormal);
}