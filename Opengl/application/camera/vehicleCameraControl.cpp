#include "vehicleCameraControl.h"

VehicleCameraControl::VehicleCameraControl() {}

VehicleCameraControl::~VehicleCameraControl() {}

void VehicleCameraControl::update()
{
	//最终移动方向
	//glm::vec3 direction(0.0f);
	//auto front = glm::cross(mCamera->mUp, mCamera->mRight);
	//auto right = mCamera->mRight;

	//if (mKeyMap[GLFW_KEY_W]) {
	//	direction += front;
	//}

	//if (mKeyMap[GLFW_KEY_S]) {
	//	direction -= front;
	//}

	//if (mKeyMap[GLFW_KEY_A]) {
	//	direction -= right;
	//}

	//if (mKeyMap[GLFW_KEY_D]) {
	//	direction += right;
	//}
	assert(Position != glm::vec3(0.0f, 0.0f, 0.0f));
	assert(CameraTarget != glm::vec3(0.0f, 0.0f, 0.0f));
	mCamera->mPosition = Position;
    mCamera->mFront = glm::normalize(CameraTarget - Position);
}
