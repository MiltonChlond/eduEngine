#pragma once

#include "SphereColliderComponent.h"
#include "PlayerControllerComponent.h"
#include "NPCController.h"
#include "AABBColliderComponent.h"
#include "TransformComponent.h"
#include "Food.h"
#include "Horse.h"
#include <entt/fwd.hpp>
#include <entt/entt.hpp>
#include "EventQueue.h"

#include "Log.hpp"

inline void CollisionResponseSystem(entt::registry& registry, const CollisionEvent& event)
{
	if (registry.any_of<PlayerControllerComponent>(event.a) && registry.any_of<NPCController>(event.b)) //if player collides with npc, player takes damage
	{
		DamageEvent damageEvent = { event.a, 10 };
		eventQueue.QueueEvent(damageEvent);
	}
}

inline void TriggerResponseSystem(entt::registry& registry, const TriggerEvent& event)
{
	if (registry.any_of<FoodComponent>(event.trigger) && registry.any_of<PlayerControllerComponent>(event.collider)) //if player collides with npc, player takes damage
	{
		QuestEvent questEvent = { QuestType::PlayerCanCollectFood, event.collider, event.trigger};
		eventQueue.QueueEvent(questEvent);
	}

	if (registry.any_of<HorseComponent>(event.trigger) && registry.any_of<PlayerControllerComponent>(event.collider)) //if player collides with npc, player takes damage
	{
		QuestEvent questEvent = { QuestType::PlayerCanFeedHorse, event.collider, event.trigger };
		eventQueue.QueueEvent(questEvent);
	}
}