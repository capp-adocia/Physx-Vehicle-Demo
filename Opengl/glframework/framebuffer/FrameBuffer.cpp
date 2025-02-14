#include "FrameBuffer.h"

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height)
	: mWidth(width)
	, mHeight(height)
{
	// 创建一个帧缓冲对象
	glGenFramebuffers(1, &mFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	// 生成颜色附件
	setColorAttachment(Texture::createColorAttachmentTexture(getWidth(), getHeight(), 0));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D
		, getColorAttachment()->getTexture(), 0);
	// 生成depth Stencil附件，加入fbo
	setDepthStencilAttachment(Texture::createDepthStencilmentTexture(getWidth(), getHeight(), 0));
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT
		, GL_TEXTURE_2D, getDepthStencilAttachment()->getTexture(), 0);

	// 检查帧缓冲对象是否完整
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Error:FrameBuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer()
{
	if (getFBO())
	{
		glDeleteFramebuffers(1, &mFBO);
	}
}