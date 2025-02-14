#include "InstanceMesh.h"

InstanceMesh::InstanceMesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material, unsigned int instanceCount)
	: Mesh(geometry, material)
	, mInstanceCount(instanceCount)
{
	assert(instanceCount > 0 && "instance count must be greater than 0!");
	mType = ObjectType::InstanceMesh;
	// 创建instanceCount个矩阵
	// reserver 改变的是容量，不会改变指针，所以size是0
	// resize	是分配10个空间，并且对其初始化，size是10
	mInstanceMatrices.resize(instanceCount);

	glGenBuffers(1, &mMatrixVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
	glBufferData(GL_ARRAY_BUFFER, mInstanceMatrices.size() * sizeof(glm::mat4), mInstanceMatrices.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(mGeometry->getVao());
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);

	// 将矩阵数据传递给着色器 第二种方式实例渲染
	for (int i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(3 + i);
		glVertexAttribPointer(3 + i, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(float) * i * 4));
		glVertexAttribDivisor(3 + i, 1);//逐实例更新
	}
	glBindVertexArray(0);
}

InstanceMesh::~InstanceMesh()
{
}

// 每一帧更新一次
void InstanceMesh::updateMatrices()
{
	glBindBuffer(GL_ARRAY_BUFFER, mMatrixVbo);
	// glbuffersubdata会覆盖掉之前的数据，不需要手动清空
	glBufferSubData(GL_ARRAY_BUFFER, 0, mInstanceMatrices.size() * sizeof(glm::mat4), mInstanceMatrices.data());
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceMesh::sortMatrices(const glm::mat4& viewMatrix)
{
	std::sort(mInstanceMatrices.begin(), mInstanceMatrices.end()
		, [viewMatrix](const glm::mat4& a, const glm::mat4& b) {
			// A
			auto& modelMatrixA = a;
			auto worldPositionA = modelMatrixA * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionA = viewMatrix * worldPositionA;
			// B
			auto& modelMatrixB = b;
			auto worldPositionB = modelMatrixB * glm::vec4(0.0, 0.0, 0.0, 1.0);
			auto cameraPositionB = viewMatrix * worldPositionB;

			return cameraPositionA.z < cameraPositionB.z; // z值越小，越在前面
		});
}
