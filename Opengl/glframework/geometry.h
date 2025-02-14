#pragma once

#include "core.h"

class Geometry {
public:
	Geometry();
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<unsigned int>& indices
	);
	Geometry(
		const std::vector<float>& positions,
		const std::vector<float>& normals,
		const std::vector<float>& uvs,
		const std::vector<float>& colors,
		const std::vector<unsigned int>& indices
	);
	~Geometry();

	static std::shared_ptr<Geometry> createBox(float size);
	static std::shared_ptr<Geometry> createBox(const float length, const float height, const float width);
	static std::shared_ptr<Geometry> createSphere(float radius);
	static std::shared_ptr<Geometry> createPlane(float width, float height);
	static std::shared_ptr<Geometry> createScreenPlane();
	static std::shared_ptr<Geometry> createCylinder(float radius, float height);

	GLuint getVao()const { return mVao; }
	uint32_t getIndicesCount()const { return mIndicesCount; }

private:
	GLuint mVao{ 0 };
	GLuint mPosVbo{ 0 };
	GLuint mUvVbo{ 0 };
	GLuint mNormalVbo{ 0 };
	GLuint mEbo{ 0 };
	GLuint mColorVbo{ 0 };

	uint32_t mIndicesCount{ 0 };
};