#include <GL/glew.h>

#include "Types.h"
#include "Utilities/Log.h"
#include "Render/Texture.h"
#include "Render/Framebuffer.h"

Framebuffer::Framebuffer() {};
Framebuffer::~Framebuffer() {};

void Framebuffer::NewColor(glm::ivec2 winSize) {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	_color = new Texture();
	#ifdef __APPLE__
		_color->New(Texture::F_RGB, Texture::T_UBYTE, winSize.x * 2, winSize.y * 2);
	#else
		_color->New(Texture::F_RGB, Texture::T_UBYTE, winSize.x, winSize.y);
	#endif
	_depth = new Texture();
	#ifdef __APPLE__
		_depth->New(Texture::F_DEPTH, Texture::T_FLOAT, winSize.x * 2, winSize.y * 2);
	#else
		_depth->New(Texture::F_DEPTH, Texture::T_FLOAT, winSize.x, winSize.y);
	#endif

	_color->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color->GetId(), 0);
	_depth->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->GetId(), 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error("[Framebuffer::NewColor]\nCouldn't generate.");
		exit(36);
	}
};

void Framebuffer::NewShadow() {};

void Framebuffer::Resize(glm::ivec2 newSize) {
	_color->Resize(newSize.x, newSize.y);
	_depth->Resize(newSize.x, newSize.y);
};

Texture* Framebuffer::GetColorTexture() {return _color;};
Texture* Framebuffer::GetDepthTexture() {return _depth;};
void Framebuffer::Bind() {glBindFramebuffer(GL_FRAMEBUFFER, _fbo);};
void Framebuffer::Unbind() {glBindFramebuffer(GL_FRAMEBUFFER, 0);};