#pragma once
#include "material.h"
#include "../texture.h"

class PhongEnvMaterial : public Material
{
public:
	PhongEnvMaterial();
	~PhongEnvMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }
	inline std::shared_ptr<Texture> getEnv() { assert(mEnv); return mEnv; }
	inline float getShiness() { return mShiness; }

public:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
	std::shared_ptr<Texture> mSpecularMask{ nullptr };
	std::shared_ptr<Texture> mEnv{ nullptr };
	float mShiness{ 1.0f };
};