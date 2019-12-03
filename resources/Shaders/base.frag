#version 400
in vec2 uvPos;
out vec4 fragColor;

uniform sampler2D tex;
uniform sampler2D normal;

void main() {
	fragColor = texture(tex, uvPos);
}