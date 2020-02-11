#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;


vec3 EdgeDetect(vec3 color) {
	vec2 texelSize = vec2(1.0 / textureSize(screenTexture, 0).x, 1.0 / textureSize(screenTexture, 0).y);
	vec2 offsets[9] = vec2[](
		vec2(-texelSize.x, texelSize.y),
		vec2(0.0, texelSize.y),
		vec2(texelSize.x, texelSize.y),
		vec2(-texelSize.x, 0.0),
		vec2(0.0, 0.0),
		vec2(texelSize.x, 0.0),
		vec2(-texelSize.x, -texelSize.y),
		vec2(0.0, -texelSize.y),
		vec2(texelSize.x, -texelSize.y)
	);
	const float kernel[9] = float[] (
		1.0, 1.0, 1.0,
		1.0, -8.0, 1.0,
		1.0, 1.0, 1.0
	);
	vec3 col = vec3(0.0);
	for (int i = 0; i < 9; i++) {
		col += texture(screenTexture, fragPos + offsets[i]).rgb * kernel[i];
	}
	return col;
}

vec3 aces_approx(vec3 v)
{
    v *= 0.6;
    float a = 2.51;
    float b = 0.03;
    float c = 2.43;
    float d = 0.59;
    float e = 0.14;
    return clamp((v*(a*v+b))/(v*(c*v+d)+e), 0.0, 1.0);
}

void main() {
	const float gamma = 2.2;

	vec3 hdrColor = texture(screenTexture, fragPos).rgb;
	hdrColor = EdgeDetect(hdrColor);
	vec3 mapped = aces_approx(hdrColor);
	mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped, 1.0);
}