#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "CameraMovementComponent.h"

enum class CameraMode;
struct CameraMovementComponent;

inline void ThirdPersonCameraSystem(CameraComponent& cameraComp, TransformComponent& cameraTran, CameraMovementComponent& cameraMove, TransformComponent& followTransform) //followTransform = the transform the camera
                                                                                                                                                                          //is following as a thirdperson perspective
{
	glm::vec4 forward4 = glm_aux::R(followTransform.rot.y, glm_aux::vec3_010) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
	glm::vec3 transformForward = glm::vec3(forward4.x, forward4.y, forward4.z);

	glm::vec3 cameraPos = followTransform.pos + transformForward * cameraMove.distance + glm::vec3(0.0f, cameraMove.height, 0.0f);

	cameraTran.pos = cameraPos;
	cameraComp.lookAt = followTransform.pos + glm::vec3(0.0f, 3.0f, 0.0f);
}