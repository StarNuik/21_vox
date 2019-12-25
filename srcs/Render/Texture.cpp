// #include "Render/Objects.h"
// #include "Engine/Log.h"
#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Render/Texture.h"
#include "Utilities/Log.h"

Texture::Texture() {
	_loaded = false;
};

Texture::~Texture() {
	if (_loaded) {
		glDeleteTextures(1, &_id);
	}
};

void Texture::New(TexFormat format, uint width, uint height, void* data) {
	New(format, T_BYTE, width, height, data);
};

void Texture::New(TexFormat format, TexType type, uint width, uint height) {
	New(format, type, width, height, NULL);
};

void Texture::New(TexFormat format, TexType type, uint width, uint height, void* data) {
	glGenTextures(1, &_id);
	glBindTexture(GL_TEXTURE_2D, _id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
	_loaded = true;
};

void Texture::Load(std::string path) {
	uint channels, width, height;

	stbi_set_flip_vertically_on_load(1);
	uint8* data = stbi_load(path.c_str(), (int*)&width, (int*)&height, (int*)&channels, 4);
	if (!data) {
		stbi_image_free(data);
		return;
	}
	New(F_RGBA, width, height, data);
	stbi_image_free(data);
};

void Texture::Use() {
	glBindTexture(GL_TEXTURE_2D, _id);
};

bool Texture::IsLoaded() {return _loaded;};
