#include <GL/glew.h>

#include "Types.h"
#include "Utilities/Log.h"
#include "Render/Texture.h"
#include "Render/Framebuffer.h"

Framebuffer::Framebuffer() {
	_color0 = nullptr;
};
Framebuffer::~Framebuffer() {};

void Framebuffer::NewBloom(glm::ivec2 winSize) {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	_color0 = new Texture();
	_color0->New(Texture::F_RGB16, Texture::T_FLOAT, winSize.x / 4, winSize.y / 4);
	_color1 = new Texture();
	_color1->New(Texture::F_RGB16, Texture::T_FLOAT, winSize.x / 4, winSize.y / 4);

	_color0->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color0->GetId(), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	_color1->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _color1->GetId(), 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// _depth->Use();
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->GetId(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error("[Framebuffer::NewBloom]\nCouldn't generate.");
		exit(36);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Framebuffer::NewColor(glm::ivec2 winSize) {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);

	_color0 = new Texture();
	_color0->New(Texture::F_RGB16, Texture::T_FLOAT, winSize.x, winSize.y);
	_color1 = new Texture();
	_color1->New(Texture::F_RGB16, Texture::T_FLOAT, winSize.x, winSize.y);
	glGenRenderbuffers(1, &_depthRbo);
	glBindRenderbuffer(GL_RENDERBUFFER, _depthRbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, winSize.x, winSize.y);
	// _depth = new Texture();
	// _depth->New(Texture::F_DEPTH, Texture::T_FLOAT, winSize.x, winSize.y);

	_color0->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _color0->GetId(), 0);
	_color1->Use();
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, _color1->GetId(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRbo);
	// _depth->Use();
	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->GetId(), 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error("[Framebuffer::NewColor]\nCouldn't generate.");
		exit(36);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Framebuffer::NewShadow(glm::ivec2 winSize) {
	glGenFramebuffers(1, &_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	
	_depth = new Texture();
	_depth->New(Texture::F_DEPTH, Texture::T_FLOAT, winSize.x, winSize.y);
	_depth->Use();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = {1.f, 1.f, 1.f, 1.f};
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, 1.f);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depth->GetId(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Log::Error("[Framebuffer::NewShadow]\nCouldn't generate.");
		exit(36);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
};

void Framebuffer::Resize(glm::ivec2 newSize) {
	if (_color0)
		_color0->Resize(newSize.x, newSize.y);
	if (_color1)
		_color1->Resize(newSize.x, newSize.y);
	if (_color2)
		_color2->Resize(newSize.x, newSize.y);
	if (_depth)
		_depth->Resize(newSize.x, newSize.y);
};

Texture* Framebuffer::GetColorTexture(int n) {
	switch(n) {
		case 1:
			return _color1;
		case 2:
			return _color2;
		default:
			return _color0;
	}
};

void Framebuffer::Use() {
	glBindFramebuffer(GL_FRAMEBUFFER, _fbo);
	if (_color2) {
		const uint attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
		glDrawBuffers(3, attachments);
	} else if (_color1) {
		const uint attachments[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};
		glDrawBuffers(2, attachments);
	}
};

Texture* Framebuffer::GetColorTexture() {return _color0;};
Texture* Framebuffer::GetDepthTexture() {return _depth;};
void Framebuffer::Bind() {glBindFramebuffer(GL_FRAMEBUFFER, _fbo);};
void Framebuffer::Unbind() {glBindFramebuffer(GL_FRAMEBUFFER, 0);};
uint Framebuffer::GetId() {return _fbo;};