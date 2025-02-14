#pragma once
#include "../core.h"
#include "../texture.h"

class FrameBuffer
{
public:
	FrameBuffer(unsigned int width, unsigned int height);
	~FrameBuffer();

	inline unsigned int getFBO() const { return mFBO; }
	inline unsigned int getWidth() const { return mWidth; }
	inline unsigned int getHeight() const { return mHeight; }
	inline std::shared_ptr<Texture> getColorAttachment() const { return mColorAttachment; }
	inline std::shared_ptr<Texture> getDepthStencilAttachment() const { return mDepthStencilAttachment; }
	inline void setFBO(unsigned int fbo) { mFBO = fbo; }
	inline void setWidth(unsigned int width) { mWidth = width; }
	inline void setHeight(unsigned int height) { mHeight = height; }
	inline void setColorAttachment(std::shared_ptr<Texture> colorAttachment) { mColorAttachment = colorAttachment; }
	inline void setDepthStencilAttachment(std::shared_ptr<Texture> depthStencilAttachment) { mDepthStencilAttachment = depthStencilAttachment; }

private:
	unsigned int mWidth{ 0 };
	unsigned int mHeight{ 0 };

	unsigned int mFBO = 0;
	std::shared_ptr<Texture> mColorAttachment{ nullptr };
	std::shared_ptr<Texture> mDepthStencilAttachment{ nullptr };
};