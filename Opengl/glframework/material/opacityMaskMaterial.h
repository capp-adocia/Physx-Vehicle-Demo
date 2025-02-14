#pragma once
#include "material.h"
#include "../texture.h"

// 给材质上蒙版
class OpacityMaskMaterial : public Material
{
public:
	OpacityMaskMaterial();
	~OpacityMaskMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }

public:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
	std::shared_ptr<Texture> mSpecularMask{ nullptr }; // 透明度蒙版
	float mShiness{ 1.0f };
};