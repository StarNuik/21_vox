#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

void main() {
	vec4 color = texture(screenTexture, fragPos);
	vec4 colorX = color;
	vec4 colorY = color;
	if (fragPos.x < 0.1) {
		float val = fragPos.x * 10;
		colorX = mix(vec4(0.0, 0.0, 0.0, 1.0), color, val);
	}
	if (fragPos.x > 0.9) {
		float val = (1.0 - abs(fragPos.x)) * 10;
		colorX = mix(vec4(0.0, 0.0, 0.0, 1.0), color, val);
	}
	if (fragPos.y < 0.1) {
		float val = fragPos.y * 10;
		colorY = mix(vec4(0.0, 0.0, 0.0, 1.0), color, val);
	}
	if (fragPos.y > 0.9) {
		float val = (1.0 - abs(fragPos.y)) * 10;
		colorY = mix(vec4(0.0, 0.0, 0.0, 1.0), color, val);
	}
	fragColor = mix(colorX, colorY, 0.5);
}