#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;
uniform sampler2D bloomTexture;
uniform float runtime;

vec3 HueShift(vec3 color, float hueShift) {
	const vec3 kRGBToYPrime = vec3(0.299, 0.587, 0.114);
	const vec3 kRGBToI = vec3(0.596, -0.275, -0.321);
	const vec3 kRGBToQ = vec3(0.212, -0.523, 0.311);

	const vec3 kYIQToR = vec3(1.0, 0.956, 0.621);
	const vec3 kYIQToG = vec3(1.0, -0.272, -0.647);
	const vec3 kYIQToB = vec3(1.0, -1.107, 1.704);

	// Convert to YIQ
	float YPrime = dot(color, kRGBToYPrime);
	float I = dot(color, kRGBToI);
	float Q = dot(color, kRGBToQ);

	// Calculate the hue and chroma
	float hue = atan(Q, I);
	float chroma = sqrt(I * I + Q * Q);

	// Make the user's adjustments
	hue += hueShift;

	// Convert back to YIQ
	Q = chroma * sin(hue);
	I = chroma * cos(hue);

	// Convert back to RGB
	vec3 yIQ = vec3(YPrime, I, Q);
	color.r = dot(yIQ, kYIQToR);
	color.g = dot(yIQ, kYIQToG);
	color.b = dot(yIQ, kYIQToB);

	// Save the result
	return color;
}

float mySin(float offset) {
	return sin(runtime + offset) * 25.0;
}

vec3 EdgeDetect(vec3 color) {
	vec2 texelSize = vec2(1.0 / textureSize(screenTexture, 0).x, 1.0 / textureSize(screenTexture, 0).y);
	vec2 offsets[9] = vec2[](
		vec2(-texelSize.x * mySin(0.0), texelSize.y * mySin(1.0)),
		vec2(0.0, texelSize.y * mySin(2.0)),
		vec2(texelSize.x * mySin(3.0), texelSize.y * mySin(4.0)),
		vec2(-texelSize.x * mySin(5.0), 0.0),
		vec2(0.0, 0.0),
		vec2(texelSize.x * mySin(6.0), 0.0),
		vec2(-texelSize.x * mySin(7.0), -texelSize.y * mySin(8.0)),
		vec2(0.0, -texelSize.y * mySin(9.0)),
		vec2(texelSize.x * mySin(10.0), -texelSize.y * mySin(11.0))
	);
	const float kernel[9] = float[] (
		0.2, 0.2, 0.2,
		0.2, -1.6, 0.2,
		0.2, 0.2, 0.2
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

	vec3 hdrColor = texture(screenTexture, fragPos+ vec2(sin(runtime + fragPos.y) * 0.008, sin(runtime + 1.25 + fragPos.x) * 0.008)).rgb;
	vec3 bloomColor = texture(bloomTexture, fragPos).rgb;
	vec3 totalColor = hdrColor + bloomColor;
	totalColor = EdgeDetect(totalColor);
	vec3 mapped = aces_approx(totalColor);

	mapped = pow(mapped, vec3(1.0 / gamma));

	mapped = HueShift(mapped, 2.25);
	
	fragColor = vec4(mapped, 1.0);
}