#pragma once

#include "entt/entt.hpp"
#include "entt/fwd.hpp"
#include "EventQueue.h"
#include "Horse.h"
#include "AnimationComponent.h"

inline void HorseSystem(entt::registry& registry, const HorseFedEvent& event)
{
	if (registry.all_of<HorseComponent, AnimationComponent>(event.horse))
	{
		auto& horseAnim = registry.get<AnimationComponent>(event.horse);
		horseAnim.playAnimation = true;
	}
}