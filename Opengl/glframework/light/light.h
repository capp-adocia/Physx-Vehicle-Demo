#pragma once
#include "../core.h"

class Light {
public:
	Light();
	~Light();
	inline float& GetIntensity() { return mIntensity; }

public:
	glm::vec3	mColor{ 1.0f };
	float		mSpecularIntensity{ 1.0f };
	float 		mIntensity{ 1.0f }; // 光强
};