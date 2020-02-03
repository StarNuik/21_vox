#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

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
	vec3 mapped = aces_approx(hdrColor);
	
	mapped *= vec3(0.75, 0.03, 1.0);
	mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped, 1.0);
}