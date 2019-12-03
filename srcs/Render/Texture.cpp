#include "Render/Objects.h"
#include "Engine/Locator.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path) {
	uint channels, width, height;

	stbi_set_flip_vertically_on_load(1);
	uint8* data = stbi_load(path.c_str(), (int*)&width, (int*)&height, (int*)&channels, 4);
	if (!data) {
		Locator::getLogger()->LogError("[Texture::Texture]\nCouldn't load [" + path + "] texture.");
		//! Something is wrong here!
		exit(1);
	}
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);
	Locator::getLogger()->LogSuccess("[Texture::Texture]\nLoaded: " + path);
}

Texture::~Texture() {
	glDeleteTextures(1, &_id);
}

void Texture::Use() {
	glBindTexture(GL_TEXTURE_2D, _id);
}