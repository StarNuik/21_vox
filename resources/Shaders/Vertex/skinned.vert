#version 400
#define MAX_BONE_COUNT (256)
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;
layout(location = 3) in ivec4 vertexBoneIDs;
layout(location = 4) in vec4 vertexBoneWeights;

out VS_OUT {
	vec3 worldPos;
	vec4 lightPos;
	vec3 normal;
	vec2 uv;
} vsOut;

// out vs_out 



layout (std140) uniform boneData {
	mat4 boneTransform[MAX_BONE_COUNT];
} BoneData;

uniform mat4 model;
uniform mat4 lightSpace;
uniform mat4 mvp;

void main() {
	vec4 boneWeights = normalize(vertexBoneWeights);
	vec4 tempPos = vec4(vertexPosition, 1.0);
	vec4 animatedPos =
		boneWeights.x * BoneData.boneTransform[vertexBoneIDs.x] * tempPos +
		boneWeights.y * BoneData.boneTransform[vertexBoneIDs.y] * tempPos +
		boneWeights.z * BoneData.boneTransform[vertexBoneIDs.z] * tempPos +
		boneWeights.w * BoneData.boneTransform[vertexBoneIDs.w] * tempPos;
	

	vsOut.uv = vertexUV;
	vsOut.worldPos = vec3(model * animatedPos);
	vsOut.normal = normalize(mat3(transpose(inverse(model))) * vertexNormal);
	vsOut.lightPos = lightSpace * vec4(vsOut.worldPos, 1.0);
	gl_Position = mvp * animatedPos;
}
