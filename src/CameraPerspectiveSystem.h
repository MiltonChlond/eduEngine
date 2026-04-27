#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "InputManager.hpp"

inline void CameraPerspectiveSystem(entt::registry& registry, InputManagerPtr& input)
{
	auto cameras = registry.view<CameraComponent>();
	auto camera = cameras.front();

	using Key = eeng::InputManager::Key;
	bool T = input->IsKeyPressed(Key::T);
	bool F = input->IsKeyPressed(Key::F);

	if (!(T && F))
	{
		auto& cameraComponent = cameras.get<CameraComponent>(camera);
		if (T)
			cameraComponent.cameraMode = CameraMode::ThirdPersonCam;
		if (F)
			cameraComponent.cameraMode = CameraMode::FreeCam;
	}
}
