#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "AnimationComponent.h"
#include "LinearVelocityComponent.h"

inline void AnimationSpeedSystem(entt::registry& registry)
{
	auto view = registry.view<AnimationComponent, LinearVelocityComponent>();
	for (auto entity : view)
	{
		auto& aniComp = view.get<AnimationComponent>(entity);
		auto& vel = view.get<LinearVelocityComponent>(entity);

		aniComp.speed = glm::length(vel.velocity);
		aniComp.maxSpeed = vel.maxSpeed;
	}
}