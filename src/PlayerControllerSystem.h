#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "LinearVelocityComponent.h"
#include "PlayerControllerComponent.h"
#include "TransformComponent.h"
#include "InputManager.hpp"

inline void PlayerControllerSystem(entt::registry& registry, InputManagerPtr& input)
{
	auto entities = registry.view<LinearVelocityComponent, TransformComponent, PlayerControllerComponent>();

	for (auto entity : entities)
	{
		auto& velocity = entities.get<LinearVelocityComponent>(entity);
		auto& transform = entities.get<TransformComponent>(entity);
		auto& playerController = entities.get<PlayerControllerComponent>(entity);
		
		using Key = eeng::InputManager::Key;
		bool W = input->IsKeyPressed(Key::W);
		bool A = input->IsKeyPressed(Key::A);
		bool S = input->IsKeyPressed(Key::S);
		bool D = input->IsKeyPressed(Key::D);
		
		glm::vec3 forwardVec = glm::vec3(glm_aux::R(transform.rot.y, glm_aux::vec3_010) * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f));
		glm::vec3 rightVec = glm::cross(forwardVec, glm_aux::vec3_010);

		glm::vec3 vel = forwardVec * ((W ? -1.0f : 0.0f) + (S ? 1.0f : 0.0f)) + rightVec * ((A ? 1.0f : 0.0f) + (D ? -1.0f : 0.0f));

		velocity.velocity = vel * playerController.speed;
	}
}