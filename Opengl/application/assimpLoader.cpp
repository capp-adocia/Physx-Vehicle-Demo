#include "assimpLoader.h"
#include "../glframework/tools/tools.h"
#include "../glframework/material/phongMaterial.h"

std::shared_ptr<Object> AssimpLoader::load(const std::string& path) {
    std::size_t lastIndex = path.find_last_of("//");
    auto rootPath = path.substr(0, lastIndex + 1);

    // 创建根节点对象
    auto rootNode = std::make_shared<Object>();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        assert(false && "Error: Model Read Failed!");
        return nullptr;
    }

    processNode(scene->mRootNode, rootNode, scene, rootPath);

    return rootNode;
}

void AssimpLoader::processNode(aiNode* ainode, std::shared_ptr<Object> parent, const aiScene* scene, const std::string& rootPath) {
    auto node = std::make_shared<Object>();
    parent->addChild(node);
    glm::mat4 localMatrix = getMat4f(ainode->mTransformation);

    glm::vec3 position, eulerAngle, scale;
    Tools::decompose(localMatrix, position, eulerAngle, scale);
    node->setPosition(position);
    node->setAngleX(eulerAngle.x);
    node->setAngleY(eulerAngle.y);
    node->setAngleZ(eulerAngle.z);
    node->setScale(scale);
    // 处理当前节点的所有网格
    for (int i = 0; i < ainode->mNumMeshes; i++) {
        int meshID = ainode->mMeshes[i];
        aiMesh* aimesh = scene->mMeshes[meshID];
        auto mesh = processMesh(aimesh, scene, rootPath);
        node->addChild(mesh);
    }

    // 递归处理所有子节点
    for (int i = 0; i < ainode->mNumChildren; i++) {
        processNode(ainode->mChildren[i], node, scene, rootPath);
    }
}

std::shared_ptr<Mesh> AssimpLoader::processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath) {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    // 处理顶点数据
    for (int i = 0; i < aimesh->mNumVertices; i++) {
        positions.push_back(aimesh->mVertices[i].x);
        positions.push_back(aimesh->mVertices[i].y);
        positions.push_back(aimesh->mVertices[i].z);

        normals.push_back(aimesh->mNormals[i].x);
        normals.push_back(aimesh->mNormals[i].y);
        normals.push_back(aimesh->mNormals[i].z);

        if (aimesh->mTextureCoords[0]) {
            uvs.push_back(aimesh->mTextureCoords[0][i].x);
            uvs.push_back(aimesh->mTextureCoords[0][i].y);
        }
        else {
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
        }
    }

    // 处理面数据
    for (int i = 0; i < aimesh->mNumFaces; i++) {
        aiFace face = aimesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    // 创建几何体对象
    std::shared_ptr<Geometry> geometry = std::make_shared<Geometry>(positions, normals, uvs, indices);
    std::shared_ptr<PhongMaterial> material = std::make_shared<PhongMaterial>();
    //material->setDepthWrite(false);
    // 处理材质
    if (aimesh->mMaterialIndex >= 0)
    {
        std::shared_ptr<Texture> texture = nullptr;
        aiMaterial* aiMat = scene->mMaterials[aimesh->mMaterialIndex];
        // 读取diffuse贴图
        texture = processTexture(aiMat, aiTextureType_DIFFUSE, scene, rootPath);
        if (!texture)
        {
            texture = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
        }
        texture->setUnit(0);
        material->mDiffuse = texture;
        // 读取specular贴图
        auto specularMask = processTexture(aiMat, aiTextureType_SPECULAR, scene, rootPath);
        if (!specularMask)
        {
            specularMask = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
        }
        specularMask->setUnit(1);

        material->mSpecularMask = specularMask;
    }
    else
    {
        material->mDiffuse = Texture::createTexture("assets/textures/defaultTexture.jpg", 0);
    }

    // 创建并返回 Mesh 对象
    return std::make_shared<Mesh>(geometry, material);
}

std::shared_ptr<Texture> AssimpLoader::processTexture(
    const aiMaterial* aimat,
    const aiTextureType& type,
    const aiScene* scene,
    const std::string& rootPath
) 
{
    std::shared_ptr<Texture> texture = nullptr;

    // 获取纹理路径
    aiString aipath;
    aimat->Get(AI_MATKEY_TEXTURE(type, 0), aipath);
    if (!aipath.length) {
        return nullptr;
    }

    // 判断是否是内嵌纹理（例如 FBX 文件中的纹理）
    const aiTexture* aitexture = scene->GetEmbeddedTexture(aipath.C_Str());
    if (aitexture) {
        // 处理内嵌纹理
        unsigned char* dataIn = reinterpret_cast<unsigned char*>(aitexture->pcData);
        uint32_t widthIn = aitexture->mWidth; // 纹理的宽度
        uint32_t heightIn = aitexture->mHeight; // 纹理的高度
        texture = Texture::createTexture(aipath.C_Str(), 0, dataIn, widthIn, heightIn);
    }
    else {
        // 处理外部纹理文件
        std::string fullPath = rootPath + aipath.C_Str();
        texture = Texture::createTexture(fullPath, 0);
    }

    return texture;
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value) {
    glm::mat4 to(
        value.a1, value.a2, value.a3, value.a4,
        value.b1, value.b2, value.b3, value.b4,
        value.c1, value.c2, value.c3, value.c4,
        value.d1, value.d2, value.d3, value.d4
    );

    return to;
}
