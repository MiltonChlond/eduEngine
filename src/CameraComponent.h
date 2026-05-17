#pragma once

#include "glmcommon.hpp"

struct CameraComponent
{
	bool isActive = true;

	glm::vec3 lookAt = glm_aux::vec3_000;
	glm::vec3 up = glm_aux::vec3_010;

	const float nearPlane = 1.0f;
	const float farPlane = 500.0f;
	const float fov = 60.0f;

	float yaw = 0.0f;
	float pitch = -glm::pi<float>() / 8;
	glm::ivec2 mouse_xy_prev{ -1, -1 };
};