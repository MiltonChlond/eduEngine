#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "CameraMovementComponent.h"
#include "FreeCameraSystem.h"
#include "ThirdPersonCameraSystem.h"
#include "Log.hpp"
#include "InputManager.hpp"

inline void CameraMovementController(entt::registry& registry, InputManagerPtr& input, float dt, TransformComponent& followTransform)
{
	auto cameras = registry.view<CameraComponent, CameraMovementComponent, TransformComponent>();
	entt::entity activeCam;
	bool foundCam = false;
	for (auto cam : cameras)
	{
		auto& camComp = cameras.get<CameraComponent>(cam);
		if (camComp.isActive)
		{
			activeCam = cam;
			foundCam = true;
			break;
		}
	}
	
	if (!foundCam)
	{
		eeng::Log("no camera found (CameraMovementSystem)");
		return;
	}

	auto& camComp = cameras.get<CameraComponent>(activeCam);
	TransformComponent& camTran = cameras.get<TransformComponent>(activeCam);
	auto& camMove = cameras.get<CameraMovementComponent>(activeCam);

	if (camMove.cameraMode == CameraMode::FreeCam)
	{
		FreeCameraSystem(camComp, camTran, camMove, input, dt);
	}
	else if(camMove.cameraMode == CameraMode::ThirdPersonCam)
	{
		ThirdPersonCameraSystem(camComp, camTran, camMove, followTransform);
	}
}