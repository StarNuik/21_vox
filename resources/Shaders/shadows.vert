#version 400
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inUvPos;

out vec2 uvPos;
out vec3 normal;
out vec3 fragPos;
out vec4 fragPosLS;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 lightSpace;

void main() {
	uvPos = inUvPos;
	vec4 worldPos = model * vec4(position, 1.0);
	fragPos = vec3(worldPos);
	normal = normalize(mat3(transpose(inverse(model))) * inNormal);
	fragPosLS = lightSpace * vec4(fragPos, 1.0);
	gl_Position = projection * view * worldPos;
}