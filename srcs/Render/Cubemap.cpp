#include "GL/glew.h"
#include "Render/Cubemap.h"
#include "stb_image.h"
#include "Utilities/Locator.hpp"

CubeMap::CubeMap(std::string paths[6]) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);

	uint width, height, channels;
	for (uint i = 0; i < 6; i++) {
		uint8 *data = stbi_load(paths[i].c_str(), (int*)&width, (int*)&height, (int*)&channels, 3);
		if (!data) {
			Locator::GetLogger()->LogError("[CubeMap::CubeMap]\nCouldn't load [" + paths[i] + "]");
			exit(1);
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
};

CubeMap::~CubeMap() {
	glDeleteTextures(1, &_id);
};

void CubeMap::Use() {
	glBindTexture(GL_TEXTURE_CUBE_MAP, _id);
};

