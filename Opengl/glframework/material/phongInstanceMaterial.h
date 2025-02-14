#pragma once
#include "material.h"
#include "../texture.h"

class PhongInstanceMaterial : public Material
{
public:
	PhongInstanceMaterial();
	~PhongInstanceMaterial();
	inline std::shared_ptr<Texture> getDiffuse() { assert(mDiffuse); return mDiffuse; }
	inline std::shared_ptr<Texture> getSpecularMask() { assert(mSpecularMask); return mSpecularMask; }
	inline float getShiness() { return mShiness; }
	inline void setDiffuse(std::shared_ptr<Texture> diffuse) { mDiffuse = diffuse; }
	inline void setSpecularMask(std::shared_ptr<Texture> specularMask) { mSpecularMask = specularMask; }

private:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
	std::shared_ptr<Texture> mSpecularMask{ nullptr };
	float mShiness{ 1.0f };
};