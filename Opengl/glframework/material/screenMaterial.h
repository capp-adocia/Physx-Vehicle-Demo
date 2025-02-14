#pragma once
#include "material.h"
#include "../texture.h"

class ScreenMaterial : public Material {
public:
	ScreenMaterial();
	~ScreenMaterial();
	inline void setScreenTexture(std::shared_ptr<Texture> texture) { mScreenTexture = texture; }
	inline std::shared_ptr<Texture> getScreenTexture() { assert(mScreenTexture); return mScreenTexture; }
private:
	std::shared_ptr<Texture> mScreenTexture{ nullptr };
};