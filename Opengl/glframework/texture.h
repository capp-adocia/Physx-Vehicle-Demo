#pragma once
#include"core.h"
#include <string>
#include <unordered_map>

class Texture {
public:
	static std::shared_ptr<Texture> createTexture(const std::string& path, unsigned int unit);
	static std::shared_ptr<Texture> createTexture(
		const std::string& path,
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	// 创建颜色
	static std::shared_ptr<Texture> createColorAttachmentTexture(unsigned int width, unsigned int height, unsigned int unit);
	// 创建深度
	static std::shared_ptr<Texture> createDepthStencilmentTexture(unsigned int width, unsigned int height, unsigned int unit);
	Texture(const std::string& path, unsigned int unit);
	Texture(
		unsigned int unit,
		unsigned char* dataIn,
		uint32_t widthIn,
		uint32_t heightIn
	);
	Texture(unsigned int width, unsigned int height, unsigned int unit);

	// 天空盒
	Texture(const std::vector<std::string>& paths, unsigned int unit);

	Texture() = default;
	
	~Texture();
	void bind();
	inline void setUnit(unsigned int unit) { mUnit = unit; }
	inline unsigned int getUnit()const { return mUnit; }

	inline int getWidth()const { return mWidth; }
	inline int getHeight()const { return mHeight; }
	inline GLuint getTexture()const { return mTexture; }
	inline unsigned int getTextureTarget()const { return mTextureTarget; }
	inline void setWidth(int width) { mWidth = width; }
	inline void setHeight(int height) { mHeight = height; }
	inline void setTexture(GLuint texture) { mTexture = texture; }
	inline void setTextureTarget(unsigned int target) { mTextureTarget = target; }

private:
	GLuint mTexture{ 0 };
	int mWidth{ 0 };
	int mHeight{ 0 };
	unsigned int mUnit{ 0 };

	static std::unordered_map<std::string, std::shared_ptr<Texture>> mTextureCache;

	unsigned int mTextureTarget{ GL_TEXTURE_2D }; // 纹理目标 默认为2D纹理
};