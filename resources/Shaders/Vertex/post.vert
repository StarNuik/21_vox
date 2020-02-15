#version 400
layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 vertexUv;
layout (location = 3) in vec3 vertexTangent;

out vec2 fragPos;

void main()
{
    gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0); 
    fragPos = vertexUv;
}