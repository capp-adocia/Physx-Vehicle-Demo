#pragma once 
#include "core.h"

enum class ObjectType {
    Object,
    Mesh,
    Scene,
    InstanceMesh,
};

class Object : public std::enable_shared_from_this<Object> {
public:
    Object();
    virtual ~Object();

    void setPosition(glm::vec3 pos);

    // 增量旋转
    void rotateX(float angle);
    void rotateY(float angle);
    void rotateZ(float angle);

    // 设置旋转角度
    void setAngleX(float angle);
    void setAngleY(float angle);
    void setAngleZ(float angle);

    void setScale(glm::vec3 scale);

    glm::vec3 getPosition() const { return mPosition; }
    glm::mat4 getModelMatrix() const;

    // 父子关系
    void addChild(std::shared_ptr<Object> obj);  // 使用 shared_ptr
    std::vector<std::shared_ptr<Object>>& getChildren();  // 返回 shared_ptr 的 vector
    std::shared_ptr<Object> getParent() const;  // 返回 shared_ptr

    // 获取类型信息
    ObjectType getType() const { return mType; }
    void setParent(std::shared_ptr<Object> parent);

    glm::mat4& ModelMatrixV2();
    glm::mat4 getModelMatrixV2() const; // bullet修改modelmatrix
    void setModelMatrix(const glm::mat4& model);
protected:
    glm::mat4 model{ 1.0f };
    glm::vec3 mPosition{ 0.0f };

    // unity 旋转标准：pitch yaw roll
    float mAngleX{ 0.0f };
    float mAngleY{ 0.0f };
    float mAngleZ{ 0.0f };

    glm::vec3 mScale{ 1.0f };

    // 父子关系
    std::vector<std::shared_ptr<Object>> mChildren{};  // 存储 shared_ptr 对象
    std::weak_ptr<Object> mParent;  // 父对象是 shared_ptr

    // 类型记录
    ObjectType mType;
};
