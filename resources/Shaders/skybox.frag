#version 400
in vec3 skyCoords;

uniform samplerCube day;
uniform samplerCube night;
uniform float lerpVal;

out vec4 fragColor;

void main() {
	vec4 dayCol = texture(day, skyCoords);
	vec4 nightCol = texture(night, skyCoords);
	fragColor = mix(nightCol, dayCol, lerpVal);
}

// void main() {
// 	vec4 nightCol = texture(night, skyCoords);
// 	fragColor = mix(nightCol, vec4(0.5, 0.5, 0.95, 1.0), lerpVal);
// }