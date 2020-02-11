#version 400

in VS_OUT {
	vec3 worldPos;
	vec3 normal;
	vec2 uv;
} vsOut;

out vec4 fragColor;

void main() {
	fragColor = vec4(1.0, 0.0, 0.0, 1.0);
}