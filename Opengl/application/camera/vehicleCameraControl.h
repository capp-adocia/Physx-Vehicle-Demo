#pragma once

#include "cameraControl.h"

class VehicleCameraControl : public CameraControl {
public:
	VehicleCameraControl();
	~VehicleCameraControl();
	void update() override;

	inline void setCameraPosTarget(const glm::vec3& pos, const glm::vec3& target)
	{
		Position = pos;
		CameraTarget = target;
	}

	glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
};