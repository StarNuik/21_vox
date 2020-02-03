#version 400
layout(location = 0) in vec3 vertexPosition;
layout(location = 2) in vec2 vertexUV;

out VS_OUT {
	vec2 uv;
} vsOut;

uniform mat4 mvp;

void main() {
	vsOut.uv = vertexUV;
	gl_Position = mvp * vec4(vertexPosition, 1.0);
}