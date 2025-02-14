#pragma once
#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material
{
public:
	PhongMaterial();
	~PhongMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }

public:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
	std::shared_ptr<Texture> mSpecularMask{ nullptr };
	float mShiness{ 1.0f };
};