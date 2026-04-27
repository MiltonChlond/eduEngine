#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "CameraComponent.h"
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"

inline void ThirdPersonCameraSystem(entt::registry& registry)
{
	auto cameras = registry.view<CameraComponent, TransformComponent>();
	auto players = registry.view<PlayerControllerComponent, TransformComponent>();

	if (players.begin() == players.end()) return;

	auto player = players.front();
	auto& playerTran = players.get<TransformComponent>(player);

	glm::vec3 playerForward = glm::vec3(glm_aux::R(playerTran.rot.y, glm_aux::vec3_010) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));

	for (auto camera : cameras)
	{
		auto& cameraComp = cameras.get<CameraComponent>(camera);
		auto& cameraTran = cameras.get<TransformComponent>(camera);

		if (cameraComp.cameraMode != CameraMode::ThirdPersonCam) continue;

		auto cameraPos = playerTran.pos + playerForward * cameraComp.distance + glm::vec3{ 0, cameraComp.height, 0 };

		cameraTran.pos = cameraPos;
		cameraComp.lookAt = playerTran.pos + glm::vec3(0, 3.0f, 0);
	}
}