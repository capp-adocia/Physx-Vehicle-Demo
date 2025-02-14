#pragma once
#include <vector>
#include "../core.h"
#include "../../mesh/mesh.h"
#include "../../application/camera/camera.h"
#include "../light/directionalLight.h"
#include "../light/pointLight.h"
#include "../light/spotLight.h"
#include "../light/ambientLight.h"
#include "../shader.h"
#include "../scene.h"

class Renderer {
public:
    Renderer();
    ~Renderer();

    void render(
        std::shared_ptr<Scene> scene,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<DirectionalLight> dirLight,
        std::shared_ptr<AmbientLight> ambLight,
        unsigned int fbo = 0
    );

    void renderObject(
        std::shared_ptr<Object> object,
        std::shared_ptr<Camera> camera,
        std::shared_ptr<DirectionalLight> dirLight,
        std::shared_ptr<AmbientLight> ambLight
    );

    void setClearColor(glm::vec3 color);

    std::shared_ptr<Material> mGlobalMaterial{ nullptr };

private:
    void projectObject(std::shared_ptr<Object> obj);

    std::shared_ptr<Shader> pickShader(MaterialType type);

    void setDepthState(std::shared_ptr<Material> material);
    void setStencilState(std::shared_ptr<Material> material);
    void setBlendState(std::shared_ptr<Material> material);
    void setFaceCullState(std::shared_ptr<Material> material);
    void setPolygonOffsetState(std::shared_ptr<Material> material);
private:
    std::shared_ptr<Shader> mPhongShader{ nullptr };
    std::shared_ptr<Shader> mWhiteShader{ nullptr };
    std::shared_ptr<Shader> mDepthShader{ nullptr };
    std::shared_ptr<Shader> mOpacityMaskShader{ nullptr };
    std::shared_ptr<Shader> mScreenShader{ nullptr };
    std::shared_ptr<Shader> mCubeShader{ nullptr };
    std::shared_ptr<Shader> mPhongEnvShader{ nullptr };
    std::shared_ptr<Shader> mPhongInstShader{ nullptr };
    std::shared_ptr<Shader> mGrassInstShader{ nullptr };

    std::vector<std::shared_ptr<Mesh>> mOpacityObjects; // 半透明不透明物体
    std::vector<std::shared_ptr<Mesh>> mTransparentObjects; // 透明物体
};
