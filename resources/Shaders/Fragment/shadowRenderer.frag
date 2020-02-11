#version 400

in vec2 uv;

uniform sampler2D diffuse;

void main() {
	if (texture(diffuse, uv).a == 0.0)
		discard;
	// frag = vec4((gl_FragCoord.z + 1.0) * 0.5, (gl_FragCoord.z + 1.0) * 0.5, (gl_FragCoord.z + 1.0) * 0.5, 1.0);
}