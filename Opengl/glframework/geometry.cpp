#include "geometry.h"
#include <vector>

Geometry::Geometry() {

}

Geometry::Geometry(
	const std::vector<float>& positions,
	const std::vector<float>& normals,
	const std::vector<float>& uvs,
	const std::vector<unsigned int>& indices
) {
	mIndicesCount = indices.size();

	glGenBuffers(1, &mPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size()*sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size()*sizeof(float), normals.data(), GL_STATIC_DRAW);

	//3 EBO创建
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

	glBindVertexArray(0);
}

Geometry::Geometry(const std::vector<float>& positions
	, const std::vector<float>& normals
	, const std::vector<float>& uvs
	, const std::vector<float>& colors
	, const std::vector<unsigned int>& indices)
{
	mIndicesCount = indices.size();

	glGenBuffers(1, &mPosVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mUvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &mNormalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);
	// colorVBO
	glGenBuffers(1, &mColorVbo);
	glBindBuffer(GL_ARRAY_BUFFER, mColorVbo);
	glBufferData(GL_ARRAY_BUFFER, colors.size() * sizeof(float), colors.data(), GL_STATIC_DRAW);

	//3 EBO创建
	glGenBuffers(1, &mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &mVao);
	glBindVertexArray(mVao);

	glBindBuffer(GL_ARRAY_BUFFER, mPosVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mUvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mNormalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, mColorVbo);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEbo);

	glBindVertexArray(0);
}

Geometry::~Geometry() {
	if (mVao != 0) {
		glDeleteVertexArrays(1, &mVao);
	}
	if (mPosVbo != 0) {
		glDeleteBuffers(1, &mPosVbo);
	}
	if (mUvVbo != 0) {
		glDeleteBuffers(1, &mUvVbo);
	}
	if (mEbo != 0) {
		glDeleteBuffers(1, &mEbo);
	}
	if (mNormalVbo != 0) {
		glDeleteBuffers(1, &mNormalVbo);
	}
	if (mColorVbo != 0)
	{
		glDeleteBuffers(1, &mColorVbo);
	}
}

std::shared_ptr<Geometry> Geometry::createBox(float size) {
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
	geometry->mIndicesCount = 36;

	float halfSize = size / 2.0f;

	float positions[] = {
		// Front face
		-halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize,
		// Back face
		-halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize,
		// Top face
		-halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,
		// Bottom face
		-halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize,
		// Right face
		halfSize, -halfSize, halfSize, halfSize, -halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, halfSize, halfSize,
		// Left face
		-halfSize, -halfSize, -halfSize, -halfSize, -halfSize, halfSize, -halfSize, halfSize, halfSize, -halfSize, halfSize, -halfSize
	};

	float uvs[] = {
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
		0.0, 0.0, 1.0, 0.0, 1.0, 1.0, 0.0, 1.0,
	};

	float normals[] = {
		//前面
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		//后面
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f,

		//上面
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,

		//下面
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		0.0f, -1.0f, 0.0f,

		//右面
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f, 0.0f,

		//左面
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
		-1.0f, 0.0f, 0.0f,
	};

	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19,  16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo ;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	//3 EBO创建
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}
// 长方体盒子 长 高 宽

std::shared_ptr<Geometry> Geometry::createBox(const float length, const float height, const float width)
{
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
	geometry->mIndicesCount = 36;

	// Half dimensions for positioning vertices symmetrically
	float halfLength = length / 2.0f;
	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	// Define the positions for each face of the box
	float positions[] = {
		// Front face
		-halfLength, -halfHeight, halfWidth,  halfLength, -halfHeight, halfWidth,  halfLength, halfHeight, halfWidth, -halfLength, halfHeight, halfWidth,
		// Back face
		-halfLength, -halfHeight, -halfWidth, -halfLength, halfHeight, -halfWidth,  halfLength, halfHeight, -halfWidth,  halfLength, -halfHeight, -halfWidth,
		// Top face
		-halfLength, halfHeight, halfWidth,  halfLength, halfHeight, halfWidth,  halfLength, halfHeight, -halfWidth, -halfLength, halfHeight, -halfWidth,
		// Bottom face
		-halfLength, -halfHeight, -halfWidth,  halfLength, -halfHeight, -halfWidth,  halfLength, -halfHeight, halfWidth, -halfLength, -halfHeight, halfWidth,
		// Right face
		halfLength, -halfHeight, halfWidth,  halfLength, -halfHeight, -halfWidth,  halfLength, halfHeight, -halfWidth,  halfLength, halfHeight, halfWidth,
		// Left face
		-halfLength, -halfHeight, -halfWidth, -halfLength, -halfHeight, halfWidth, -halfLength, halfHeight, halfWidth, -halfLength, halfHeight, -halfWidth
	};

	// Define the texture coordinates (assuming a standard cube mapping)
	float uvs[] = {
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f
	};

	// Define the normals for each face (each face has one normal direction)
	float normals[] = {
		// Front
		0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		// Back
		0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f,
		// Top
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		// Bottom
		0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		// Right
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		// Left
		-1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f
	};

	// Define the indices for the 6 faces (each face consists of 2 triangles)
	unsigned int indices[] = {
		0, 1, 2, 2, 3, 0,   // Front face
		4, 5, 6, 6, 7, 4,   // Back face
		8, 9, 10, 10, 11, 8,  // Top face
		12, 13, 14, 14, 15, 12, // Bottom face
		16, 17, 18, 18, 19,  16, // Right face
		20, 21, 22, 22, 23, 20  // Left face
	};

	// Create VBOs for positions, UVs, and normals
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	// Create EBO for indices
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Create VAO and bind buffers
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	// Bind EBO to the VAO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}

std::shared_ptr<Geometry> Geometry::createSphere(float radius) {
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();

	//目标：1 位置 2 uv 3 索引
	//1 主要变量声明
	std::vector<GLfloat> positions{};
	std::vector<GLfloat> uvs{};
	std::vector<GLfloat> normals{};
	std::vector<GLuint> indices{};

	//声明纬线与经线的数量
	int numLatLines = 60;//纬线
	int numLongLines = 60;//经线

	//2 通过两层循环（纬线在外，经线在内）->位置、uv、法线
	for (int i = 0; i <= numLatLines; i++) {
		for (int j = 0; j <= numLongLines; j++) {
			float phi = i * glm::pi<float>() / numLatLines;
			float theta = j * 2 * glm::pi<float>() / numLongLines;

			float y = radius * cos(phi);
			float x = radius * sin(phi) * cos(theta);
			float z = radius * sin(phi) * sin(theta);

			positions.push_back(x);
			positions.push_back(y);
			positions.push_back(z);

			float u = 1.0 - (float)j / (float)numLongLines;
			float v = 1.0 - (float)i / (float)numLatLines;

			uvs.push_back(u);
			uvs.push_back(v);

			//注意：法线方向没有问题，法线的长度不为1
			normals.push_back(x);
			normals.push_back(y);
			normals.push_back(z);
		}
	}


	//3 通过两层循环（这里没有=号）->顶点索引
	for (int i = 0; i < numLatLines; i++) {
		for (int j = 0; j < numLongLines; j++) {
			int p1 = i * (numLongLines + 1) + j;
			int p2 = p1 + numLongLines + 1;
			int p3 = p1 + 1;
			int p4 = p2 + 1;

			indices.push_back(p1);
			indices.push_back(p2);
			indices.push_back(p3);

			indices.push_back(p3);
			indices.push_back(p2);
			indices.push_back(p4);
		}
	}


	//4 生成vbo与vao
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	geometry->mIndicesCount = indices.size();


	return geometry;
}


std::shared_ptr<Geometry> Geometry::createPlane(float width, float height) {
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
	geometry->mIndicesCount = 6;

	float halfW = width / 2.0f;
	float halfH = height / 2.0f;

	float positions[] = {
		-halfW, -halfH, 0.0f,
		halfW, -halfH, 0.0f,
		halfW, halfH, 0.0f,
		-halfW, halfH, 0.0f,
	};

	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f
	};

	float normals[] = {
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f,
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//2 VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo, normalVbo = geometry->mNormalVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);

	//3 EBO创建
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//4 VAO创建
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

	//5.4 加入ebo到当前的vao
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}

std::shared_ptr<Geometry> Geometry::createScreenPlane()
{
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
	geometry->mIndicesCount = 6;
	// 构建数据position uv
	float positions[] = {
		-1.0f, -1.0f,
		 1.0f, -1.0f,
		 1.0f,  1.0f,
		-1.0f,  1.0f,
	};
	float uvs[] = {
		0.0f, 0.0f,
		1.0f, 0.0f,
		1.0f, 1.0f,
		0.0f, 1.0f,
	};
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	// VBO创建
	GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	// EBO创建
	glGenBuffers(1, &geometry->mEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// VAO创建
	glGenVertexArrays(1, &geometry->mVao);
	glBindVertexArray(geometry->mVao);

	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

	glBindVertexArray(0);

	return geometry;
}

// 创建圆柱体 半径和高
std::shared_ptr<Geometry> Geometry::createCylinder(float radius, float height) {
	std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>();
	const int sectors = 36; // 控制圆柱体的细分程度

	std::vector<float> positions;
	std::vector<float> uvs;
	std::vector<float> normals; // 添加法线数据
	std::vector<unsigned int> indices;

	// 生成顶点数据
	// 顶面中心 (0)
	positions.insert(positions.end(), { 0.0f, height / 2, 0.0f });
	uvs.insert(uvs.end(), { 0.5f, 0.5f });
	normals.insert(normals.end(), { 0.0f, 1.0f, 0.0f }); // 顶面法线

	// 顶面周围顶点 (1 到 sectors+1)
	for (int i = 0; i <= sectors; ++i) {
		float theta = 2 * M_PI * i / sectors;
		float x = radius * cos(theta);
		float z = radius * sin(theta);
		positions.insert(positions.end(), { x, height / 2, z });
		// UV 坐标采用极坐标映射到 [0,1]
		uvs.insert(uvs.end(), { (cos(theta) + 1.0f) / 2.0f, (sin(theta) + 1.0f) / 2.0f });
		normals.insert(normals.end(), { 0.0f, 1.0f, 0.0f }); // 顶面法线
	}

	// 底面中心 (sectors+2)
	positions.insert(positions.end(), { 0.0f, -height / 2, 0.0f });
	uvs.insert(uvs.end(), { 0.5f, 0.5f });
	normals.insert(normals.end(), { 0.0f, -1.0f, 0.0f }); // 底面法线

	// 底面周围顶点 (sectors+3 到 2sectors+3)
	for (int i = 0; i <= sectors; ++i) {
		float theta = 2 * M_PI * i / sectors;
		float x = radius * cos(theta);
		float z = radius * sin(theta);
		positions.insert(positions.end(), { x, -height / 2, z });
		// UV 坐标采用极坐标映射到 [0,1]
		uvs.insert(uvs.end(), { (cos(theta) + 1.0f) / 2.0f, (sin(theta) + 1.0f) / 2.0f });
		normals.insert(normals.end(), { 0.0f, -1.0f, 0.0f }); // 底面法线
	}

	// 侧面顶点 (2sectors+4 开始)
	for (int i = 0; i <= sectors; ++i) {
		float theta = 2 * M_PI * i / sectors;
		float x = radius * cos(theta);
		float z = radius * sin(theta);

		// 顶面环顶点
		positions.insert(positions.end(), { x, height / 2, z });
		uvs.insert(uvs.end(), { static_cast<float>(i) / sectors, 1.0f });
		normals.insert(normals.end(), { x, 0.0f, z }); // 侧面法线 (x, 0, z)

		// 底面环顶点
		positions.insert(positions.end(), { x, -height / 2, z });
		uvs.insert(uvs.end(), { static_cast<float>(i) / sectors, 0.0f });
		normals.insert(normals.end(), { x, 0.0f, z }); // 侧面法线 (x, 0, z)
	}

	// 生成顶面三角形
	for (unsigned int i = 0; i < sectors; ++i) {
		indices.insert(indices.end(), { 0, 1 + i, 1 + i + 1 });
	}

	// 生成底面三角形
	unsigned int bottomCenter = sectors + 2;
	unsigned int bottomStart = sectors + 3;
	for (int i = 0; i < sectors; ++i) {
		indices.insert(indices.end(), { bottomCenter, bottomStart + i + 1, bottomStart + i });
	}

	// 生成侧面三角形
	int sideStart = 2 * (sectors + 2); // 侧面顶点起始索引
	for (int i = 0; i < sectors; ++i) {
		unsigned int topCurrent = sideStart + 2 * i;
		unsigned int bottomCurrent = topCurrent + 1;
		unsigned int topNext = sideStart + 2 * (i + 1);
		unsigned int bottomNext = topNext + 1;

		indices.insert(indices.end(), { topCurrent, bottomCurrent, topNext });
		indices.insert(indices.end(), { bottomCurrent, bottomNext, topNext });
	}

	// 设置索引数量
	geometry->mIndicesCount = indices.size();

	// 创建OpenGL对象
	GLuint posVbo, uvVbo, normalVbo, ebo, vao;
	glGenBuffers(1, &posVbo);
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &uvVbo);
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &normalVbo);
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(float), normals.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// 位置属性 (3D坐标)
	glBindBuffer(GL_ARRAY_BUFFER, posVbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// UV属性
	glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

	// 法线属性
	glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	// EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	glBindVertexArray(0);

	// 保存到geometry对象
	geometry->mPosVbo = posVbo;
	geometry->mUvVbo = uvVbo;
	geometry->mNormalVbo = normalVbo;
	geometry->mEbo = ebo;
	geometry->mVao = vao;

	return geometry;
}
