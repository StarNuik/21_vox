#version 400

out vec4 fragColor;
  
in vec2 fragPos;

uniform sampler2D screenTexture;

vec3 UnchartedTM(vec3 color)
{
  const float A = 0.15f;
  const float B = 0.50f;
  const float C = 0.10f;
  const float D = 0.20f;
  const float E = 0.02f;
  const float F = 0.30f;
  const float W = 11.2f;

  color = ((color * (A * color + C * B) + D * E) / (color * ( A * color + B) + D * F)) - E / F;

  return color;
}

void main() {
	const float gamma = 2.2;

	vec3 hdrColor = texture(screenTexture, fragPos).rgb;
	vec3 mapped = UnchartedTM(hdrColor);

	mapped = pow(mapped, vec3(1.0 / gamma));

	fragColor = vec4(mapped, 1.0);
}