#version 400
in vec3 skyCoords;

uniform samplerCube day;
uniform samplerCube night;
uniform float lerpVal;

out vec4 fragColor;

vec4 lerpColor(const vec4 col0, const vec4 col1, const float val) {
	float f = clamp(val, 0.0, 1.0);
	float f1 = 1.0 - f;
	vec4 res;
	res.x = col0.x * f1 + col1.x * f;
	res.y = col0.y * f1 + col1.y * f;
	res.z = col0.z * f1 + col1.z * f;
	res.w = col0.w * f1 + col1.w * f;
	return res;
}

void main() {
	// vec4 dayCol = texture(day, skyCoords);
	vec4 nightCol = texture(night, skyCoords);
	// fragColor = lerpColor(nightCol, dayCol, lerpVal);
	fragColor = nightCol;
}