#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

void main() {
	fragColor = texture(screenTexture, fragPos);
}