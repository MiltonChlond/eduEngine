#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "CameraMovementController.h"
#include "InputManager.hpp"
#include "Log.hpp"

inline void CameraPerspectiveSystem(entt::registry& registry, InputManagerPtr& input)
{
	auto cameras = registry.view<CameraComponent, CameraMovementComponent>();
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
		eeng::Log("no camera found (CameraPerspectiveSystem)");
		return;
	}

	using Key = eeng::InputManager::Key;
	bool T = input->IsKeyPressed(Key::T);
	bool F = input->IsKeyPressed(Key::F);

	if (!(T && F))
	{
		auto& cameraMove = cameras.get<CameraMovementComponent>(activeCam);
		if (T)
			cameraMove.cameraMode = CameraMode::ThirdPersonCam;
		if (F)
			cameraMove.cameraMode = CameraMode::FreeCam;
	}
}
