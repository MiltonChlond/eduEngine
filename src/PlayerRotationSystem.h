#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "TransformComponent.h"
#include "PlayerControllerComponent.h"
#include "glmcommon.hpp"
#include "InputManager.hpp"

inline void PlayerRotationSystem(entt::registry& registry, InputManagerPtr input)
{
	auto players = registry.view<PlayerControllerComponent, TransformComponent>();

	auto mouse = input->GetMouseState();
	glm::ivec2 mouseCurr = { mouse.x, mouse.y };

	for (auto player : players)
	{
		auto& playerCTRL = players.get<PlayerControllerComponent>(player);
		auto& playerTran = players.get<TransformComponent>(player);

		glm::ivec2 deltaMouse = {0, 0};
		if (playerCTRL.mouse_xy_prev.x >= 0 && mouse.leftButton)
		{
			deltaMouse = mouseCurr - playerCTRL.mouse_xy_prev;
		}
		
		playerCTRL.mouse_xy_prev = mouseCurr;

		playerTran.rot.y -= deltaMouse.x * playerCTRL.sensitivity;
	}
}