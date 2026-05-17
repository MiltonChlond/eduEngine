#pragma once

enum class CameraMode
{
	FreeCam, ThirdPersonCam
};

struct CameraMovementComponent
{
	CameraMode cameraMode = CameraMode::ThirdPersonCam;

	//thirdPerson camera variables
	float height = 10.0f;
	float distance = 15.0f;

	//freecam variables
	float sensitivity = 0.005f;
	float moveSpeed = 10.0f;
};