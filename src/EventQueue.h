#pragma once

#include <entt/fwd.hpp>
#include <entt/entt.hpp>

#include <variant>
#include <queue>

enum class QuestType
{
	PlayerCanCollectFood,
	PlayerCanFeedHorse
};
struct QuestEvent
{
	QuestType type;
	entt::entity player = entt::null;
	entt::entity target = entt::null; //food or horse
};

struct HorseFedEvent
{
	entt::entity horse;
};

struct DamageEvent
{
	entt::entity entity = entt::null;
	int damage = 0;
};

struct CollisionEvent
{
	entt::entity a;
	entt::entity b;
};

struct TriggerEvent
{
	entt::entity trigger;
	entt::entity collider;
};


using EventType = std::variant<CollisionEvent, TriggerEvent, DamageEvent, QuestEvent, HorseFedEvent>;
using Listener = std::function<void(const EventType&)>;

class EventQueue
{
private:

	std::array<std::pair<int, Listener>, 256> listeners;
	std::queue<EventType> events;

public:
	EventQueue();

	int Subscribe(Listener listener);

	void UnSubscribe(int id);

	void QueueEvent(EventType event);

	void BroadCast();
};

extern EventQueue eventQueue;