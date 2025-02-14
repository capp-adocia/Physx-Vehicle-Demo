#pragma once
#include "../glframework/object.h"
#include "../glframework/geometry.h"
#include "../glframework/material/material.h"

class Mesh : public Object {
public:
	Mesh() = default;
	explicit Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);
	~Mesh();
	inline std::shared_ptr<Geometry> getGeometry() { assert(mGeometry); return mGeometry; }
	inline std::shared_ptr<Material> getMaterial() { assert(mMaterial); return mMaterial; }
	inline void setGeometry(std::shared_ptr<Geometry> geometry) { mGeometry = geometry; }
	inline void setMaterial(std::shared_ptr<Material> material) { mMaterial = material; }

protected:
	std::shared_ptr<Geometry> mGeometry{ nullptr };
	std::shared_ptr<Material> mMaterial{ nullptr };
};