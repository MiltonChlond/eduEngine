#pragma once

#include "PlayerQuestComponent.h"
#include "EventQueue.h"
#include "InputManager.hpp"

inline void QuestSystem(entt::registry& registry, const QuestEvent& event, InputManagerPtr& input)
{
	if (registry.any_of<QuestComponent, AnimationComponent>(event.player))
	{
		auto& questComponent = registry.get<QuestComponent>(event.player);
		auto& animComponent = registry.get<AnimationComponent>(event.player);

		using Key = eeng::InputManager::Key;
		bool E = input->IsKeyPressed(Key::E);
		
		if (event.type == QuestType::PlayerCanCollectFood && E && questComponent.hasFood == false)
		{
			questComponent.hasFood = true;
			animComponent.playAnimation = true;
			questComponent.questText = "Go to the horse and press 'E' to feed it";
		}

		if (event.type == QuestType::PlayerCanFeedHorse && questComponent.hasFood && E)
		{
			HorseFedEvent fedEvent = { event.target };
			eventQueue.QueueEvent(fedEvent);
			questComponent.hasFood = false;
			questComponent.questText = "The horse thanks you for your service";
		}
	}
}