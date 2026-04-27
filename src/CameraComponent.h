#pragma once

#include "glmcommon.hpp"

enum class CameraMode
{
	FreeCam, ThirdPersonCam
};

struct CameraComponent
{
	CameraMode cameraMode = CameraMode::ThirdPersonCam;

	glm::vec3 lookAt = glm_aux::vec3_000;
	glm::vec3 up = glm_aux::vec3_010;

	float height = 10.0f;
	float distance = 15.0f;
	float sensitivity = 0.005f;
	float moveSpeed = 10.0f;

	const float nearPlane = 1.0f;
	const float farPlane = 500.0f;

	float yaw = 0.0f;
	float pitch = -glm::pi<float>() / 8;
	glm::ivec2 mouse_xy_prev{ -1, -1 };
};