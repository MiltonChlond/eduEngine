#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "EventQueue.h"
#include "HealthComponent.h"

inline void DamageSystem(entt::registry& registry, const DamageEvent& event)
{
	if (registry.any_of<HealthComponent>(event.entity))
	{
		auto& health = registry.get<HealthComponent>(event.entity);
		health.TakeDamage(event.damage);
	}
}