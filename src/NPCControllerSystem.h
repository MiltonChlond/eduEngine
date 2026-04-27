#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "NPCController.h"
#include "TransformComponent.h"
#include "LinearVelocityComponent.h"

inline glm::vec3 GetRandomPos() // gives a random position inside of a square defined by range, centered around {0,0}
{
	int range = 90;
	float x = -(range / 2) + static_cast<float>(rand()) / RAND_MAX * range;
	float z = -(range / 2) + static_cast<float>(rand()) / RAND_MAX * range;
	return glm::vec3{ x, 0, z };
}

inline void NPCContollerSystem(entt::registry& registry)
{
	auto entities = registry.view<NPCController, TransformComponent, LinearVelocityComponent>();

	for (auto entity : entities)
	{
		auto& npcController = entities.get<NPCController>(entity);
		auto& transform = entities.get<TransformComponent>(entity);
		auto& velocity = entities.get<LinearVelocityComponent>(entity);

		if (npcController.isMoving == false)
		{
			velocity.velocity = glm::vec3{ 0, 0, 0 };
			continue;
		}
			
		glm::vec3 toTarget = npcController.targetPos - transform.pos;
		float distanceToTarget = glm::length(toTarget);

		if (distanceToTarget <= npcController.reachDistance || npcController.targetPos == glm::vec3{0, 0, 0})
		{
			npcController.targetPos = GetRandomPos();
		}

		velocity.velocity = glm::normalize(toTarget) * npcController.speed;
	}
}

