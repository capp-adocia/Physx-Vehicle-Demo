#pragma once
#include <assert.h>
#include "mesh.h"
#include <algorithm>

class InstanceMesh : public Mesh {
public:
	explicit InstanceMesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, unsigned int instanceCount = 1);
	~InstanceMesh();
	inline unsigned int getInstanceCount() const { assert(mInstanceCount > 0); return mInstanceCount; }
	inline std::vector<glm::mat4>& getInstanceMatrices() { assert(!mInstanceMatrices.empty()); return mInstanceMatrices; }
	inline void setInstanceCount(unsigned int count) { mInstanceCount = count; }

	void updateMatrices();

	// 根据实例z排序
	void sortMatrices(const glm::mat4& viewMatrix);
	// 重载 [] 运算符，允许通过索引直接访问矩阵
	glm::mat4& operator[](unsigned int index) {
		// 检查索引是否在合法范围内
		assert(index < mInstanceMatrices.size());
		return mInstanceMatrices[index];
	}

	const glm::mat4& operator[](unsigned int index) const {
		// 检查索引是否在合法范围内
		assert(index < mInstanceMatrices.size());
		return mInstanceMatrices[index];
	}
private:
	unsigned int			mInstanceCount{ 0 };
	std::vector<glm::mat4>	mInstanceMatrices{};
	unsigned int			mMatrixVbo{ 0 };
};