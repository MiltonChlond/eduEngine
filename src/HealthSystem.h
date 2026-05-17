#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "HealthComponent.h"

inline void HealthSystem(entt::registry& registry, entt::entity entity, int damage)
{
	auto& health = registry.get<HealthComponent>(entity);

	health.currentHealth -= damage;

	//raise event
}