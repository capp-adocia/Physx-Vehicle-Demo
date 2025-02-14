#pragma once

#include "cameraControl.h"

class TrackBallCameraControl :public CameraControl {
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	//���൱�еĽӿں������Ƿ���Ҫ��д
	void onCursor(double xpos, double ypos)override;

	void onScroll(float offset)override;

private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float mMoveSpeed = 0.005f;
};