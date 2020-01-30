#pragma once

#include <string>
#include <GL/glew.h>

#include "Types.h"

class Texture {
public:
	enum TexFormat {
		F_DEPTH = GL_DEPTH_COMPONENT,
		F_RGB = GL_RGB,
		F_RGBA = GL_RGBA,
		F_RGBA16 = GL_RGBA16F,
		F_RGBA32 = GL_RGBA32F,
		F_DEPTH_STENCIL = GL_DEPTH_STENCIL,
	};
	enum TexType {
		T_HALF_FLOAT = GL_HALF_FLOAT,
		T_FLOAT = GL_FLOAT,
		T_BYTE = GL_BYTE,
		T_UBYTE = GL_UNSIGNED_BYTE,
		T_UINT_24_8 = GL_UNSIGNED_INT_24_8,
	};
	Texture();
	~Texture();
	void Load(std::string);
	void Use();
	void Unbind();
	bool IsLoaded();
	void New(TexFormat format, uint width, uint height, void* data);
	void New(TexFormat format, TexType type, uint width, uint height);
	void New(TexFormat format, TexType type, uint width, uint height, void* data);
	void Resize(uint width, uint height);
	uint GetId();
private:
	uint _id;
	bool _loaded;
	TexFormat _format;
	TexType _type;
};
