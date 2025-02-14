#pragma once
#include "material.h"
#include "../texture.h"

class CubeMaterial : public Material {
public:
	CubeMaterial();
	~CubeMaterial();
	inline void setDiffuse(std::shared_ptr<Texture> diffuse) { mDiffuse = diffuse; }
	inline std::shared_ptr<Texture> getDiffuse() { return mDiffuse; }
private:
	std::shared_ptr<Texture> mDiffuse{ nullptr };
};