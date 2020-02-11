#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;


vec3 Blur(vec3 color) {
	vec2 tex = vec2(1.0 / textureSize(screenTexture, 0).x, 1.0 / textureSize(screenTexture, 0).y);
	vec2 offsets[25] = vec2[](
		vec2(-2 * tex.x, 2 * tex.y),
		vec2(-tex.x, 2 * tex.y),
		vec2(tex.x, 2 * tex.y),
		vec2(tex.x, 2 * tex.y),
		vec2(2 * tex.x, 2 * tex.y),

		vec2(-2 * tex.x, tex.y),
		vec2(-tex.x, tex.y),
		vec2(0.0, tex.y),
		vec2(tex.x, tex.y),
		vec2(2 * tex.x, tex.y),


		vec2(-2 * tex.x, 0.0),
		vec2(-tex.x, 0.0),
		vec2(0.0, 0.0),
		vec2(tex.x, 0.0),
		vec2(2 * tex.x, 0.0),


		vec2(-2 * tex.x, -tex.y),
		vec2(-tex.x, -tex.y),
		vec2(0.0, -tex.y),
		vec2(tex.x, -tex.y),
		vec2(2 * tex.x, -tex.y),
	
		vec2(-2 * tex.x, -2 * tex.y),
		vec2(-tex.x, -2 * tex.y),
		vec2(0.0, -2 * tex.y),
		vec2(tex.x, -2 * tex.y),
		vec2(2 * tex.x, -2 * tex.y)
	);
	const float kernel[25] = float[] (
		1.0 / 273.0, 4.0 / 273.0, 7.0 / 273.0, 4.0 / 273.0, 1.0 / 273.0,
		4.0 / 273.0, 16.0 / 273.0, 26.0 / 273.0, 16.0 / 273.0, 4.0 / 273.0,
		7.0 / 273.0, 26.0 / 273.0, 41.0 / 273.0, 26.0 / 273.0, 7.0 / 273.0,
		4.0 / 273.0, 16.0 / 273.0, 26.0 / 273.0, 16.0 / 273.0, 4.0 / 273.0,
		1.0 / 273.0, 4.0 / 273.0, 7.0 / 273.0, 4.0 / 273.0, 1.0 / 273.0
	);
	vec3 col = vec3(0.0);
	for (int i = 0; i < 25; i++) {
		col += texture(screenTexture, fragPos + offsets[i]).rgb * kernel[i];
	}
	return col;
}

void main() {
	vec3 color = texture(screenTexture, fragPos).rgb;
	vec3 mapped = Blur(color);
	fragColor = vec4(mapped, 1.0);
}