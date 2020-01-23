#pragma once

#include <string>
#include <GL/glew.h>

#include "Types.h"

class Texture {
public:
	enum TexFormat {
		F_DEPTH = GL_DEPTH_COMPONENT,
		F_RGB = GL_RGB,
		F_RGBA = GL_RGBA
	};
	enum TexType {
		T_FLOAT = GL_FLOAT,
		T_BYTE = GL_UNSIGNED_BYTE
	};
	Texture();
	~Texture();
	void Load(std::string);
	void Use();
	bool IsLoaded();
	void New(TexFormat format, uint width, uint height, void* data);
	void New(TexFormat format, TexType type, uint width, uint height);
	void New(TexFormat format, TexType type, uint width, uint height, void* data);
private:
	uint _id;
	bool _loaded;
};
