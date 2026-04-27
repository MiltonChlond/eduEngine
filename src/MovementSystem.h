#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "TransformComponent.h"
#include "LinearVelocityComponent.h"

inline void MovementSystem(entt::registry& registry, float dt)
{
	auto entities = registry.view<TransformComponent, LinearVelocityComponent>();

	for (auto entity : entities)
	{
		auto& transform = entities.get<TransformComponent>(entity);
		auto& velocity = entities.get<LinearVelocityComponent>(entity);

		transform.pos += velocity.velocity * dt;
	}
}