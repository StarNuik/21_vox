#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

vec3 FilmicTM(vec3 color)
{
    color = max(vec3(0.0f), color - vec3(0.004f));
    color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f) + 0.06f);

    return color;
}

void main() {
	const float gamma = 2.2;

	vec3 hdrColor = texture(screenTexture, fragPos).rgb;
	vec3 mapped = FilmicTM(hdrColor);

	mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped, 1.0);
}